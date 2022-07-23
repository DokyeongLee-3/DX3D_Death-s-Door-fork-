#include "BossBettyCloseAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../BossBettyDataComponent.h"

CBossBettyCloseAttackNode::CBossBettyCloseAttackNode()
{
	SetTypeID(typeid(CBossBettyCloseAttackNode).hash_code());
}

CBossBettyCloseAttackNode::CBossBettyCloseAttackNode(const CBossBettyCloseAttackNode& Node)
{
}

CBossBettyCloseAttackNode::~CBossBettyCloseAttackNode()
{
}

void CBossBettyCloseAttackNode::Init()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	// 1) Slash Left
	std::string AnimName = "SlashLeft";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	AnimInst->AddNotify(AnimName, "OnTracePlayer", 13,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);

	AnimInst->AddNotify(AnimName, "OnDisableTracePlayer", 20,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);

	AnimInst->AddNotify(AnimName, "OnSlashLeft", 19,
		this, &CBossBettyCloseAttackNode::OnBossBettySlashLeftEffect);
	AnimInst->AddNotify(AnimName, "CameraShake", 19,
		Data, &CBossBettyDataComponent::OnBossBettyNormalShakeCamera);

	// >> End
	AnimInst->SetEndFunction(AnimName, 
		this, &CBossBettyCloseAttackNode::OnBossBettyCommonEndFunctionOfCloseAttack);

	// 2) Slash Right
	AnimName = "SlashRight";

	AnimInst->AddNotify(AnimName, "OnTracePlayer", 13,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);

	AnimInst->AddNotify(AnimName, "OnDisableTracePlayer", 20,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);


	AnimInst->AddNotify(AnimName, "OnSlashRight", 18,
		this, &CBossBettyCloseAttackNode::OnBossBettySlashRightEffect);
	AnimInst->AddNotify(AnimName, "CameraShake", 18,
		Data, &CBossBettyDataComponent::OnBossBettyNormalShakeCamera);

	// >> End
	AnimInst->SetEndFunction(AnimName,
		this, &CBossBettyCloseAttackNode::OnBossBettyCommonEndFunctionOfCloseAttack);

	// 3) PunchLeft
	AnimName = "PunchLeft";

	AnimInst->AddNotify(AnimName, "OnPunchLeft", 18,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect);
	AnimInst->AddNotify(AnimName, "CameraShake", 18,
		Data, &CBossBettyDataComponent::OnBossBettyNormalShakeCamera);

	AnimInst->SetEndFunction(AnimName,
		this, &CBossBettyCloseAttackNode::OnBossBettyCommonEndFunctionOfCloseAttack);

	// 4) PunchLeft
	AnimName = "PunchRight";

	AnimInst->AddNotify(AnimName, "OnPunchRight", 18,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect);
	AnimInst->AddNotify(AnimName, "CameraShake", 18,
		Data, &CBossBettyDataComponent::OnBossBettyNormalShakeCamera);

	AnimInst->SetEndFunction(AnimName,
		this, &CBossBettyCloseAttackNode::OnBossBettyCommonEndFunctionOfCloseAttack);

	// 5) Two Side Attack
	AnimName = "FirstSlam";

	AnimInst->AddNotify(AnimName, "OnFirstSlam", 20,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);
	AnimInst->AddNotify(AnimName, "CameraShake", 20,
		Data, &CBossBettyDataComponent::OnBossBettyNormalShakeCamera);

	AnimInst->SetEndFunction(AnimName,
		this, &CBossBettyCloseAttackNode::OnBossBettyCommonEndFunctionOfCloseAttack);

}

NodeResult CBossBettyCloseAttackNode::OnStart(float DeltaTime)
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// �ٰŸ� ���� Ÿ���� üũ�Ѵ�.
	m_CloseAttackType = DetermineBettyCloseAttackType();

	m_Owner->SetCurrentNode(this);

	if (!m_CloseAttackAnimChangeEnable)
		return NodeResult::Node_True;

	m_CloseAttackAnimChangeEnable = false;

	switch (m_CloseAttackType)
	{
	case BossBettyCloseAttackType::PunchLeft:
	{
		AnimInst->ChangeAnimation("PunchLeft");
	}
	break;
	case BossBettyCloseAttackType::PunchRight:
	{
		AnimInst->ChangeAnimation("PunchRight");
	}
	break;
	case BossBettyCloseAttackType::SlashLeft:
	{
		AnimInst->ChangeAnimation("SlashLeft");
	}
	break;
	case BossBettyCloseAttackType::SlashRight:
	{
		AnimInst->ChangeAnimation("SlashRight");
	}
	break;
	case BossBettyCloseAttackType::TwoSideFront:
	{
		AnimInst->ChangeAnimation("FirstSlam");
	}
	break;
	}

	return NodeResult::Node_True;
}

NodeResult CBossBettyCloseAttackNode::OnUpdate(float DeltaTime)
{
	if (m_CloseAttackType == BossBettyCloseAttackType::NotInCloseRange)
	{
		m_Owner->SetCurrentNode(nullptr);

		m_CloseAttackAnimChangeEnable = true;

		return NodeResult::Node_False;
	}

	return NodeResult::Node_True;
}

NodeResult CBossBettyCloseAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

BossBettyCloseAttackType CBossBettyCloseAttackNode::DetermineBettyCloseAttackType()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	const Vector3* const& PunchLeftSquarePoses = Data->GetPunchLeftSquarePoses();

	const Vector3& RelativeXAxis = m_Object->GetRootComponent()->GetRelativeAxis(AXIS_X) * -1;
	const Vector3& RelativeZAxis = m_Object->GetRootComponent()->GetRelativeAxis(AXIS_Z) * -1;
	const Vector3& WorldScale = m_Object->GetRootComponent()->GetWorldScale();
	const Vector3& WorldPos = m_Object->GetRootComponent()->GetWorldPos();

	float AnglePlayer = Data->GetAnglePlayer();
	float DistToPlayer = Data->DistToPlayer();
	float MeleeAttackRange = Data->GetMeleeAttackRange();

	// Player ����
	CGameObject* PlayerObj = CSceneManager::GetInst()->GetScene()->GetPlayerObject();
	Vector2 Player2DWorldPos = Vector2(PlayerObj->GetWorldPos().x, PlayerObj->GetWorldPos().z);
	Vector2 Betty2DWorldPos = Vector2(WorldPos.x, WorldPos.z);

	// 1) Box2DInfo m_PunchLeftSquareBox;
	if (DistToPlayer < MeleeAttackRange * 0.5f && AnglePlayer < 45.f && Data->IsPlayerLeftBasedInLookDir() == false)
	{
		return BossBettyCloseAttackType::PunchLeft;
	}
	// 2) Box2DInfo m_PunchRightSquareBox;
	else if (DistToPlayer < MeleeAttackRange * 0.5f && AnglePlayer < 45.f && Data->IsPlayerLeftBasedInLookDir())
	{
		return BossBettyCloseAttackType::PunchRight;
	}
	// 3) Box2DInfo m_SlashLeftSquareBox;
	else if (DistToPlayer < MeleeAttackRange * 0.75f&& 
		AnglePlayer >= 45.f && 
		AnglePlayer < 135 &&
		Data->IsPlayerLeftBasedInLookDir())
	{
		return BossBettyCloseAttackType::SlashRight;
	}
	// 4) Box2DInfo m_SlashRightSquareBox;
	else if (DistToPlayer < MeleeAttackRange * 0.75f && 
		AnglePlayer >=  45.f &&
		AnglePlayer < 135 &&
		Data->IsPlayerLeftBasedInLookDir() == false)
	{
		return BossBettyCloseAttackType::SlashLeft;
	}
	// 5) Two Down
	else if (DistToPlayer < MeleeAttackRange &&
		DistToPlayer >= MeleeAttackRange * 0.5f &&
		AnglePlayer < 45.f)
	{
		return BossBettyCloseAttackType::TwoSideFront;
	}

	return BossBettyCloseAttackType::NotInCloseRange;
}

void CBossBettyCloseAttackNode::OnBossBettySlashLeftEffect()
{
	// �������� 45 �� ȸ���ϱ� (�̰��� �ٸ� Callback ���� �������̴�)
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	const Vector3& SlashLeftSpawnPoint = Data->GetRelativeSlashLeftPos();
	
	// 1.�ٷ� ���� �ش� ��ġ�� Collider �����ϱ� 
	// 2. ���������� Particle �����ϱ� (3��)
}

void CBossBettyCloseAttackNode::OnBossBettySlashRightEffect()
{
	// ���������� 45�� ȸ���ϱ� (�̰��� �ٸ� Callback ���� �������̴�)
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	const Vector3& SlashRightSpawnPoint = Data->GetRelativeSlashRightPos();
	
	// 1.�ٷ� ���� �ش� ��ġ�� Collider �����ϱ� 
	// 2. ���������� Particle �����ϱ� (3��)
}

void CBossBettyCloseAttackNode::OnBossBettyEnableCloseAttackChangeAnim()
{
	m_CloseAttackAnimChangeEnable = true;
}

void CBossBettyCloseAttackNode::OnBossBettyDisableCloseAttackChangeAnim()
{
	m_CloseAttackAnimChangeEnable = false;
}

void CBossBettyCloseAttackNode::OnBossBettyCommonEndFunctionOfCloseAttack()
{
	// Close Attack Anim ��ȯ
	OnBossBettyEnableCloseAttackChangeAnim();

	// Player �� ���� ȸ���ϴ� �� ����
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	Data->OnDisableLookPlayer();

	// �����ؼ� Close Attack Animation ���� �ٲ� �� �ְ� �� IdleBeast Animation �� Change ���ش�.
	// m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("IdleBeast");

	// Current Node �� nullptr �� �Ͽ�, �ٸ� Node �� �˻��� �� �ְ� �Ѵ�.
	m_Owner->SetCurrentNode(nullptr);
}

void CBossBettyCloseAttackNode::OnBossBettyCommonStartFunctionOfCloseAttack()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	Data->OnBossBettyNormalShakeCamera();
}
