
#include "MonsterDataComponent.h"
#include "GameObject/GameObject.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "../Component/GameStateComponent.h"
#include "MonsterNavAgent.h"
#include "Scene/Scene.h"
#include "../Component/PlayerDataComponent.h"
#include "Component/PaperBurnComponent.h"
#include "../DDUtil.h"

CMonsterDataComponent::CMonsterDataComponent() :
	m_AnimMesh(nullptr),
	m_HitBox(nullptr),
	m_HitEffectFlag(0),
	m_HitEffectTimer(0.f),
	m_HitEffectMax(0.15f),
	m_IsCombat(false),
	m_LookPlayer(false),
	m_CurMoveSpeed(0.f),
	m_DeathColorStart(Vector4::Red),
	m_DeathColorEnd(Vector4::White),
	m_LeftLookPlayer(false),
	m_RightLookPlayer(false),
	m_CurRotSpeed(0.f),
	m_AttackCoolDelayTimeMax(0.5f)
{
	SetTypeID<CMonsterDataComponent>();

	m_ComponentType = Component_Type::ObjectComponent;
}

CMonsterDataComponent::CMonsterDataComponent(const CMonsterDataComponent& com)	:
	CObjectDataComponent(com)
{
}

CMonsterDataComponent::~CMonsterDataComponent()
{
}

void CMonsterDataComponent::Start()
{
	m_MonsterNavAgent = m_Object->FindObjectComponentFromType<CMonsterNavAgent>();
	m_State = m_Object->FindObjectComponentFromType<CGameStateComponent>();

	std::vector<CAnimationMeshComponent*> vecAnim;
	std::vector<CColliderBox3D*> vecCollider;

	m_Object->FindAllSceneComponentFromType<CAnimationMeshComponent>(vecAnim);
	m_Object->FindAllSceneComponentFromType<CColliderBox3D>(vecCollider);

	if (vecAnim.size() == 1)
	{
		m_AnimMesh = vecAnim[0];
	}
	else
	{
		size_t Size = vecAnim.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if ("AnimComp" == vecAnim[i]->GetName())
			{
				m_AnimMesh = vecAnim[i];
				break;
			}
		}
	}

	if (vecCollider.size() == 1)
	{
		m_HitBox = vecCollider[0];
	}
	else
	{
		m_HitBox = vecCollider[0];
		size_t Size = vecCollider.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if ("HitBox" == vecCollider[i]->GetName())
			{
				m_HitBox = vecCollider[i];
				break;
			}
		}
	}

	// Hit Effect�� ���� ���� �÷� ����
	if (m_AnimMesh)
	{
		m_MeshMatSize = m_AnimMesh->GetMaterialSlotCount();
		m_vecOriginDiffuse.resize(m_MeshMatSize);
		m_vecOriginAmbient.resize(m_MeshMatSize);
		m_vecOriginSpecluar.resize(m_MeshMatSize);
		m_vecOriginEmissive.resize(m_MeshMatSize);

		for (int i = 0; i < m_MeshMatSize; ++i)
		{
			m_vecOriginDiffuse[i] = m_AnimMesh->GetMaterial(i)->GetBaseColor();
			m_vecOriginAmbient[i] = m_AnimMesh->GetMaterial(i)->GetAmbientColor();
			m_vecOriginSpecluar[i] = m_AnimMesh->GetMaterial(i)->GetSpecularColor();
			m_vecOriginEmissive[i] = m_AnimMesh->GetMaterial(i)->GetEmissiveColor();
		}
	}

	// Melee Attack Collider CallBack
	m_MeleeAttackCollider = (CColliderBox3D*)m_Object->FindComponent("MeleeAttackCollider");

	if (m_MeleeAttackCollider)
	{
		m_MeleeAttackCollider->AddCollisionCallback(Collision_State::Begin, this, &CMonsterDataComponent::OnHitMeleeAttack);
	}
	
	// PaperBurn �� Death
	m_PaperBurn = m_Object->FindComponentFromType<CPaperBurnComponent>();
	
	if (m_PaperBurn)
	{
		m_PaperBurn->SetFinishCallback(this, &CMonsterDataComponent::OnDeadPaperBurnEnd);
	}

	CAnimationSequenceInstance* AnimInst = m_AnimMesh->GetAnimationInstance();
	AnimInst->AddNotify("Death", "DeathStart", 0, this, &CMonsterDataComponent::OnDeadAnimStart);
	AnimInst->SetEndFunction("Death", this, &CMonsterDataComponent::OnDeadAnimEnd);

	// CutScene ���� ( Enter Trigger, CutScene Cam, Collider CallBack)
	m_PlayerEnterZoneTrigger = (CColliderBox3D*)m_Object->FindComponent("PlayerEnterTrigger");
	m_CutSceneCam = m_Object->FindComponentFromType<CCameraComponent>();

	if (m_PlayerEnterZoneTrigger)
	{
		m_PlayerEnterZoneTrigger->AddCollisionCallback(Collision_State::Begin, this, &CMonsterDataComponent::OnPlayerEnterZone);
	}

	// MonsterNavAgent ����
	m_MonsterNavAgent = m_Object->FindObjectComponentFromType<CMonsterNavAgent>();
}

void CMonsterDataComponent::Update(float DeltaTime)
{
	CObjectDataComponent::Update(DeltaTime);

	if (m_HitEffectStart)
	{
		ActiveHitEffect(DeltaTime);
	}

	if (m_LookPlayer)
	{
		LookPlayer(DeltaTime);
	}
	else
	{
		if (m_RightLookPlayer)
		{
			// ���������� ���ƾ� �ϴµ� �������� ���� ������ true ��� ����
			if (m_LeftLookPlayer)
				assert(false);

			RightLookPlayer(DeltaTime);
		}
		else  if (m_LeftLookPlayer)
		{
			// ���������� ���ƾ� �ϴµ� �������� ���� ������ true ��� ����
			if (m_RightLookPlayer)
				assert(false);

			LeftLookPlayer(DeltaTime);
		}
	}

	if (m_MoveZEnableMaxTime > 0.f)
	{
		m_MoveZ = true;
		m_MoveZEnableMaxTime -= DeltaTime;

		if (m_MoveZEnableMaxTime < 0.f)
		{
			m_MoveZ = false;
			m_MoveZEnableMaxTime = 0.f;
		}
	}

	if (m_MoveZ)
	{
		MoveZ(DeltaTime);
	}
}

void CMonsterDataComponent::PostUpdate(float DeltaTime)
{
	CObjectDataComponent::PostUpdate(DeltaTime);

	if (m_AttackCoolTimeEnable)
	{
		m_AttackCoolDelayTime -= DeltaTime;

		if (m_AttackCoolDelayTime < 0.f)
		{
			m_AttackCoolTimeEnable = false;
		}
	}
}

CMonsterDataComponent* CMonsterDataComponent::Clone()
{
	return new CMonsterDataComponent(*this);
}

void CMonsterDataComponent::LookPlayer(float DeltaTime)
{
	// �÷��̾� �������� ȸ�� ������ŭ ȸ��
	float AnglePlayer = GetAnglePlayer();

	CGameObject* MyObj = m_Object;

	float RotSpeed = m_Data.RotateSpeedPerSec;

	// ���� m_CurRotSpeed �� ������ ������ ���¶��
	if (m_CurRotSpeed != 0.f)
	{
		RotSpeed = m_CurRotSpeed;
	}

	// (OBJ) ���������� �̼��ϰ� ������ ����
	// if (abs(AnglePlayer) < m_Data.RotateSpeedPerSec * DeltaTime)
	if (abs(AnglePlayer) < 2.f)
	{
		// MyObj->AddWorldRotationY(AnglePlayer * DeltaTime);
	}
	else 
	{
		bool IsLeft = IsPlayerLeftBasedInLookDir();

		if (IsLeft)
		{
			MyObj->AddWorldRotationY(RotSpeed * DeltaTime);
		}
		else
		{
			MyObj->AddWorldRotationY(-1.f * RotSpeed * DeltaTime);
		}
	}
}

void CMonsterDataComponent::RightLookPlayer(float DeltaTime)
{
	m_Object->AddWorldRotationY(-1.f * m_Data.RotateSpeedPerSec * DeltaTime);
}

void CMonsterDataComponent::LeftLookPlayer(float DeltaTime)
{
	m_Object->AddWorldRotationY(m_Data.RotateSpeedPerSec * DeltaTime);
}

void CMonsterDataComponent::MoveZ(float DeltaTime)
{
	m_Object->AddWorldPosByLocalAxis(AXIS::AXIS_Z, -m_CurMoveSpeed * DeltaTime);
}

void CMonsterDataComponent::ChangeColorBossDeath(float DeltaTime)
{
	if (m_DeathColorChangeStart)
	{
		m_DeathTimer += DeltaTime;

		// �÷� ���� ����
		Vector4 Color = CDDUtil::LerpColor(m_DeathColorStart, m_DeathColorEnd, m_DeathTimer, m_DeathColorChangeTimeMax);

		for (size_t i = 0; i < m_MeshMatSize; ++i)
		{
			m_AnimMesh->SetMetallic(false, 0);

			// Base
			// m_AnimMesh->SetBaseColor(Color, i);

			// Ambient
			// m_AnimMesh->SetAmbientColor(Color, i);

			// Specular
			m_AnimMesh->SetSpecularColor(Vector4::Black, (int)i);

			// Emmisive
			m_AnimMesh->SetEmissiveColor(Color, (int)i);
		}

		if (m_DeathTimer >= m_DeathColorChangeTimeMax)
		{
			m_DeathColorChangeStart = false;
		}
	}
}

void CMonsterDataComponent::OnEndAnimPostAttackDelayOff()
{
	m_PostAttackDelaying = false;

	SetCurrentNodeNull();
}

void CMonsterDataComponent::OnDeadPaperBurnEnd()
{
	// TODO : Monster Death ���� -> ���� Object Pool ���Ϳ� ���� ó�� �ʿ�
	m_Object->Destroy();
}

void CMonsterDataComponent::OnDeadAnimStart()
{
	m_HitBox->Enable(false);
	m_MeleeAttackCollider->Enable(false);

	// DeathChangeColor() �� ����ϴ� ���
	m_DeathColorChangeStart = true;
}

void CMonsterDataComponent::OnDeadAnimEnd()
{
	// Death �ִϸ��̼��� ������ PaperBurn�� �Ҵ�.
	m_PaperBurn->StartPaperBurn();
}

void CMonsterDataComponent::OnPlayerEnterZone(const CollisionResult& Result)
{
	if (m_PlayerEnterZoneTrigger)
	{
		m_PlayerEnterZoneTrigger->Enable(false);
	}

	m_PlayerEnterZone = true;

 //	if (m_CutSceneCam)
 //	{
 //		m_Scene->GetCameraManager()->KeepCamera();
 //		m_Scene->GetCameraManager()->SetCurrentCamera(m_CutSceneCam);
 //	}
 }

void CMonsterDataComponent::OnStartCutScene()
{
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	// �÷��̾� �Է� ���� �ʰ� �ϰ�, �ִϸ��̼� ���� ��ȭ
	if (PlayerObj)
	{
		CGameStateComponent* PlayerState = PlayerObj->FindComponentFromType<CGameStateComponent>();
		CAnimationSequenceInstance* PlayerAnim = PlayerObj->FindComponentFromType<CAnimationMeshComponent>()->GetAnimationInstance();

		PlayerAnim->ChangeAnimation("PlayerIdle");
		PlayerState->SetTreeUpdate(false);
	}

	m_IsCutScenePlaying = true;

	// �ƽ� ī�޶� �̵� ������ �ִ���
	bool IsCutSceneMove = m_CutSceneCam->GetMoveDataCount() > 0 ? true : false;

	if (IsCutSceneMove)
	{
		m_CutSceneCam->StartMove();
	}
	else
	{
		m_Scene->GetCameraManager()->KeepCamera();
		m_Scene->GetCameraManager()->SetCurrentCamera(m_CutSceneCam);
	}
}

void CMonsterDataComponent::OnEndCutScene()
{
	m_IsCutScenePlaying = false;

	// ���� ���� on
	OnCombatStart();

	if (m_CutSceneCam)
	{
		m_Scene->GetCameraManager()->ReturnCamera();
	}

	// �÷��̾� ���� On
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	if (PlayerObj)
	{
		CGameStateComponent* PlayerState = PlayerObj->FindComponentFromType<CGameStateComponent>();

		PlayerState->SetTreeUpdate(true);
	}
}

void CMonsterDataComponent::SetIsHit(bool Hit)
{
	CObjectDataComponent::SetIsHit(Hit);

	if (Hit)
	{
		if (!m_HitEffectStart)
		{
			m_HitEffectFlag = 0;
			m_HitEffectTimer = 0.f;
			m_HitEffectStart = true;
		}
	}
}

void CMonsterDataComponent::SetCurrentNodeNull()
{
	m_State->GetBehaviorTree()->SetCurrentNode(nullptr);
}

void CMonsterDataComponent::OnActiveMeleeAttackCollider()
{
	m_MeleeAttackCollider->Enable(true);
}

void CMonsterDataComponent::OnInActiveMeleeAttackCollider()
{
	m_MeleeAttackCollider->Enable(false);

	// Player Hit False ó��
	CGameObject* Player = m_Scene->GetPlayerObject();
	if (Player)
	{
		CPlayerDataComponent* PlayerData = Player->FindComponentFromType<CPlayerDataComponent>();
		PlayerData->SetIsHit(false);
	}
}

void CMonsterDataComponent::OnHitMeleeAttack(const CollisionResult& Result)
{
	CGameObject* Player = m_Scene->GetPlayerObject();

	CPlayerDataComponent* PlayerData = Player->FindObjectComponentFromType<CPlayerDataComponent>();

	if (PlayerData)
	{
		PlayerData->DecreaseHP(1);
		PlayerData->SetIsHit(true);
	}
}

void CMonsterDataComponent::OnEndAnimPostAttackDelayOn()
{
	SetCurrentNodeNull();

	m_PostAttackDelaying = true;
}

float CMonsterDataComponent::GetAnglePlayer()
{
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	if (!PlayerObj)
	{
		return 0.f;
	}

	Vector3 PlayerPos = PlayerObj->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();

	Vector3 ToPlayer = PlayerPos - MyPos;

	Vector3 MyAxisZ = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;

	float Angle = ToPlayer.Angle(MyAxisZ);
	return Angle;
}

bool CMonsterDataComponent::IsPlayerLeftBasedInLookDir()
{
	/*
	Vector3 vToPlayer = ToPlayer();
	// Vector3 vWorldLook = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
	Vector3 vLook = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
	Vector3 vWorldLook = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
	Vector3 vWorldRight = m_Object->GetWorldAxis(AXIS::AXIS_X) * -1.f;

	// Monster �� �����ʿ� Player ������ ���͸� ����
	float ProjToMonsterWorldRight = vToPlayer.Dot(vWorldRight);

	if (ProjToMonsterWorldRight < 0.f)
		return true;
	else
		return false;
		*/

	Vector3 vToPlayer = ToPlayer();
	Vector3 vLook = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;

	Vector3 vCross = vToPlayer.Cross(vLook);

	if (vCross.y < 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CMonsterDataComponent::OnEnableLookPlayer()
{
	m_LookPlayer = true;
}

void CMonsterDataComponent::OnDisableLookPlayer()
{
	m_LookPlayer = false;
}

void CMonsterDataComponent::OnEnableRightLookPlayer()
{
	m_RightLookPlayer = true;
}

void CMonsterDataComponent::OnDisableRightLookPlayer()
{
	m_RightLookPlayer = false;
}

void CMonsterDataComponent::OnEnableLeftLookPlayer()
{
	m_LeftLookPlayer = true;
}

void CMonsterDataComponent::OnDisableLeftLookPlayer()
{
	m_LeftLookPlayer = false;
}

void CMonsterDataComponent::OnEnableMoveZ()
{
	m_MoveZ = true;
}

void CMonsterDataComponent::OnDisableMoveZ()
{
	m_MoveZ = false;
}

bool CMonsterDataComponent::IsPlayerInMeleeAttackRange()
{
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	if (!PlayerObj)
	{
		return false;
	}

	Vector3 MyPos = m_Object->GetWorldPos();
	Vector3 PlayerPos = PlayerObj->GetWorldPos();
	float Dist = MyPos.Distance(PlayerPos);

	if (Dist <= m_Data.MeleeAttackRange)
	{
		return true;
	}

	return false;
}

Vector3 CMonsterDataComponent::ToPlayer()
{
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	if (!PlayerObj)
	{
		return Vector3::Zero;
	}

	Vector3 PlayerPos = PlayerObj->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();

	Vector3 ToP = PlayerPos - MyPos;
	ToP.Normalize();
	return ToP;
}

float CMonsterDataComponent::DistToPlayer()
{
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	if (!PlayerObj)
	{
		return -1.f;
	}

	Vector3 MyPos = m_Object->GetWorldPos();
	Vector3 PlayerPos = PlayerObj->GetWorldPos();
	float Dist = MyPos.Distance(PlayerPos);

	return MyPos.Distance(PlayerPos);;
}

bool CMonsterDataComponent::Save(FILE* File)
{
	CObjectDataComponent::Save(File);

	return true;
}

bool CMonsterDataComponent::Load(FILE* File)
{
	CObjectDataComponent::Load(File);

	return true;
}

bool CMonsterDataComponent::SaveOnly(FILE* File)
{
	CObjectDataComponent::SaveOnly(File);

	return true;
}

bool CMonsterDataComponent::LoadOnly(FILE* File)
{
	CObjectDataComponent::LoadOnly(File);

	return true;
}

void CMonsterDataComponent::ActiveHitEffect(float DeltaTime)
{
	m_HitEffectTimer += DeltaTime;

	// ������ �Ǿ��� �� �÷� ü������ �� ���� ����
	if ((m_HitEffectFlag & HIT_EFFECT_FIRST) == 0 && ((m_HitEffectTimer <= m_HitEffectMax / 5.f)))
	{
		m_HitEffectFlag |= HIT_EFFECT_FIRST;
		ChangeHitColor(HIT_EFFECT_FIRST);
		return;
	}
	else if ((m_HitEffectFlag & HIT_EFFECT_SECOND) == 0 && (m_HitEffectTimer >= ((m_HitEffectMax * 2.f / 5.f))))
	{
		m_HitEffectFlag |= HIT_EFFECT_SECOND;
		ChangeHitColor(HIT_EFFECT_SECOND);
		return;
	}
	else if ((m_HitEffectFlag & HIT_EFFECT_THIRD) == 0  && (m_HitEffectTimer >= ((m_HitEffectMax * 3.f / 5.f))))
	{
		m_HitEffectFlag |= HIT_EFFECT_THIRD;
		ChangeHitColor(HIT_EFFECT_THIRD);
		return;
	}
	else if ((m_HitEffectFlag & HIT_EFFECT_FOURTH) == 0 && m_HitEffectTimer >= ((m_HitEffectMax * 4.f / 5.f)))
	{
		m_HitEffectFlag |= HIT_EFFECT_FOURTH;
		ChangeHitColor(HIT_EFFECT_FOURTH);
		return;
	}
	else if ((m_HitEffectFlag & HIT_EFFECT_FIFTH) == 0 && m_HitEffectTimer >= m_HitEffectMax)
	{
		m_HitEffectFlag |= HIT_EFFECT_FIFTH;
		ChangeHitColor(HIT_EFFECT_FIFTH);
		return;
	}
	else if (m_HitEffectTimer >= m_HitEffectMax + (m_HitEffectMax / 5.f))
	{
		m_HitEffectStart = false;
		m_HitEffectTimer = 0.f;
		m_HitEffectFlag = 0;

		// ���� �÷��� ���ƿ´�.
		for (int i = 0; i < m_MeshMatSize; ++i)
		{
			m_AnimMesh->GetMaterial(i)->SetBaseColor(m_vecOriginDiffuse[i]);
			m_AnimMesh->GetMaterial(i)->SetAmbientColor(m_vecOriginAmbient[i]);
			m_AnimMesh->GetMaterial(i)->SetSpecularColor(m_vecOriginSpecluar[i]);
			m_AnimMesh->GetMaterial(i)->SetEmissiveColor(m_vecOriginEmissive[i]);
		}
	}
}

void CMonsterDataComponent::ChangeHitColor(int EffectNum)
{
	// ��
	if (EffectNum == HIT_EFFECT_FIRST)
	{
		for (int i = 0; i < m_MeshMatSize; ++i)
		{
			m_AnimMesh->GetMaterial(i)->SetEmissiveColor(Vector4::White);
			m_AnimMesh->GetMaterial(i)->SetBaseColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetAmbientColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetSpecularColor(Vector4::Black);
		}
	}
	// ��
	else if (EffectNum == HIT_EFFECT_SECOND || EffectNum == HIT_EFFECT_FOURTH)
	{
		for (int i = 0; i < m_MeshMatSize; ++i)
		{
			m_AnimMesh->GetMaterial(i)->SetEmissiveColor(Vector4::Black); //
			m_AnimMesh->GetMaterial(i)->SetBaseColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetAmbientColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetSpecularColor(Vector4::Black);
		}
	}
	// ��
	else if (EffectNum == HIT_EFFECT_THIRD || EffectNum == HIT_EFFECT_FIFTH)
	{
		for (int i = 0; i < m_MeshMatSize; ++i)
		{
			m_AnimMesh->GetMaterial(i)->SetEmissiveColor(Vector4::Red);
			m_AnimMesh->GetMaterial(i)->SetBaseColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetAmbientColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetSpecularColor(Vector4::Black);
		}
	}
}

CMonsterNavAgent* CMonsterDataComponent::GetMonsterNavAgent() const
{
	return m_MonsterNavAgent;
}

CColliderBox3D* CMonsterDataComponent::GetHitBox() const
{
	return m_HitBox;
}
