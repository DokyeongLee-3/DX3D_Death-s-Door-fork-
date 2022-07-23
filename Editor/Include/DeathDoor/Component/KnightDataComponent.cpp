#include "KnightDataComponent.h"
#include "../DataManager.h"
#include "Component/ColliderBox3D.h"
#include "Component/CameraComponent.h"
#include "Component/PaperBurnComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CKnightDataComponent::CKnightDataComponent()	:
	m_JumpAttackRange(0.f),
	m_MeleeAttackCount(0)
{
	SetTypeID<CKnightDataComponent>();
}

CKnightDataComponent::CKnightDataComponent(const CKnightDataComponent& com)	:
	CMonsterDataComponent(com)
{
	m_JumpAttackRange = com.m_JumpAttackRange;
	m_MeleeAttackCount = 0;
}

CKnightDataComponent::~CKnightDataComponent()
{
}

void CKnightDataComponent::Start()
{
	CMonsterDataComponent::Start();

	m_Data = CDataManager::GetInst()->GetObjectData("BossKnight");

	m_JumpAttackRange = m_Data.JumpAttackRange;

	m_PlayerEnterZoneTrigger = (CColliderBox3D*)m_Object->FindComponent("PlayerEnterTrigger");
	m_CutSceneCam = m_Object->FindComponentFromType<CCameraComponent>();

	m_PlayerEnterZoneTrigger->AddCollisionCallback(Collision_State::Begin, this, &CKnightDataComponent::OnPlayerEnterZone);
	
	m_MeleeAttackCollider->Enable(false);
}

void CKnightDataComponent::Update(float DeltaTime)
{
	CMonsterDataComponent::Update(DeltaTime);

	if (m_DeathColorChangeStart)
	{
		ChangeColorBossDeath(DeltaTime);
	}
}

void CKnightDataComponent::OnLookPlayerMove(float DeltaTime)
{
	// �÷��̾� �������� ȸ�� ������ŭ ȸ��
	float AnglePlayer = GetAnglePlayer();

	CGameObject* MyObj = m_Object;

	if (abs(AnglePlayer) < m_Data.RotateSpeedPerSec)
	{
		MyObj->AddWorldRotationY(-AnglePlayer * DeltaTime);
	}
	else
	{
		MyObj->AddWorldRotationY(m_Data.RotateSpeedPerSec * DeltaTime);
	}

	// Walk Speed �ι�� �̵�
	MyObj->AddWorldPosByLocalAxis(AXIS::AXIS_Z, -1.f * m_Data.MoveSpeed * 2.f);

	// ���� ���� �� ���� ����
	Vector3 FinalZAxis = ToPlayer();
	m_FinalTraceZAxis = FinalZAxis;
	m_FinalTraceZAxis.Normalize();
}

// ���� ����, �÷��̾���� ������ ������ �̾��, ������ �Ǵ��ϴ� �Լ�
void CKnightDataComponent::OnEndAnimJudgeContinueAttack()
{
	if (m_MeleeAttackCount >= 2)
	{
		SetContinueAttack(false);
		SetPostAttckDelaying(false);
	}
	// �ִϸ��̼� ���� ���� �÷��̾ ���� ���� ���� ���� ���� ���
	else if (IsPlayerInMeleeAttackRange())
	{
		// ���� �̾�� ���·�
		SetContinueAttack(true);

		// ���� ���� ���� �Ǵ�
		float AnglePlayer = GetAnglePlayer();

		// �÷��̾���� ������ ���� ������ ���
		if (abs(AnglePlayer) <= 45.f)
		{
			// ���� ��带 Null��
			SetCurrentNodeNull();
			SetAttackRot(Knight_Attack_Rot_Type::Front);
			return;
		}

		CGameObject* Player = m_Object->GetScene()->GetPlayerObject();

		if (!Player)
		{
			return;
		}

		bool IsPlayerLeft = IsPlayerLeftBasedInLookDir();

		if (IsPlayerLeft)
		{
			SetAttackRot(Knight_Attack_Rot_Type::CCW);
		}
		else
		{
			SetAttackRot(Knight_Attack_Rot_Type::CW);
		}
	}
	else
	{
		// �ĵ����� ���·� ��ȯ
		SetPostAttckDelaying(true);
		SetContinueAttack(false);
	}

	// ���� ��带 Null��
	SetCurrentNodeNull();
}

void CKnightDataComponent::OnEnableLookAndMove()
{
	m_MoveZ = true;
	m_LookPlayer = true;
	m_CurMoveSpeed = m_Data.MoveSpeed;
}

void CKnightDataComponent::OnDisableLookAndMove()
{
	m_MoveZ = false;
	m_LookPlayer = false;
	m_CurMoveSpeed = 0.f;
}

void CKnightDataComponent::OnWalk(float DeltaTime)
{
	LookPlayer(DeltaTime);

	m_Object->AddWorldPosByLocalAxis(AXIS::AXIS_Z, -m_Data.MoveSpeed * DeltaTime);
}

void CKnightDataComponent::OnPlayerEnterZone(const CollisionResult& Result)
{
	m_PlayerEnterZoneTrigger->Enable(false);

	m_PlayerEnterZone = true;

	if (m_CutSceneCam)
	{
		m_Scene->GetCameraManager()->KeepCamera();
		m_Scene->GetCameraManager()->SetCurrentCamera(m_CutSceneCam);
	}
}

void CKnightDataComponent::OnEndCutScenePlaying()
{
	m_IsCutScenePlaying = false;
	OnCombatStart();

	if (m_CutSceneCam)
	{
		m_Scene->GetCameraManager()->ReturnCamera();
	}
}

void CKnightDataComponent::OnStartJumpAttackMove()
{
	m_CurMoveSpeed = m_Data.MoveSpeed * 5.f;
	m_MoveZ = true;
	m_LookPlayer = true;
}

void CKnightDataComponent::OnEndJumpAttackMove()
{
	m_CurMoveSpeed = m_Data.MoveSpeed * 3.f;
	m_LookPlayer = false;
}

void CKnightDataComponent::OnEndJumpAttack()
{
	m_CurMoveSpeed = 0.f;
	m_MoveZ = false;
}

void CKnightDataComponent::OnEndContinueAttack()
{
	SetCurrentNodeNull();
	m_ContinueAttack = false;
}

void CKnightDataComponent::OnDeadAnimStart()
{
	CMonsterDataComponent::OnDeadAnimStart();

	m_AnimMesh->GetAnimationInstance()->GetCurrentAnimation()->SetPlayScale(0.5f);
	m_DeathColorChangeTimeMax = m_AnimMesh->GetAnimationInstance()->GetCurrentAnimation()->GetAnimationPlayTime() * 0.5f;
}

void CKnightDataComponent::OnDeadPaperBurnEnd()
{
	CMonsterDataComponent::OnDeadPaperBurnEnd();

	// TODO : Boss Knight - �����۹� �Ϸ�Ǹ� Portal On
}

void CKnightDataComponent::OnActiveMeleeAttackCollider()
{
	CMonsterDataComponent::OnActiveMeleeAttackCollider();

	// TODO : Boss Knight - Particle
	m_Scene->GetCameraManager()->ShakeCamera(0.5f, 2.f);
}
