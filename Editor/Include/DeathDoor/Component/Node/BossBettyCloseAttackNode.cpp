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

	AnimInst->AddNotifyDeltaTimeFrameRange(AnimName, "OnTracePlayer", 0, 5,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnLookPlayer);

	AnimInst->AddNotify(AnimName, "OnSlashLeft", 18,
		this, &CBossBettyCloseAttackNode::OnBossBettySlashLeftEffect);

	// 2) Slash Right
	AnimName = "SlashRight";

	AnimInst->AddNotify(AnimName, "OnSlashRight", 18,
		this, &CBossBettyCloseAttackNode::OnBossBettySlashRightEffect);

	AnimInst->AddNotifyDeltaTimeFrameRange(AnimName, "OnTracePlayer", 0, 5,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnLookPlayer);

	// 3) PunchLeft
	AnimName = "PunchLeft";

	AnimInst->AddNotify(AnimName, "OnPunchLeft", 18,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect);

	// 4) PunchLeft
	AnimName = "PunchRight";

	AnimInst->AddNotify(AnimName, "OnPunchRight", 18,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect);

	// 5) Two Side Attack
	AnimName = "FirstSlam";

	AnimInst->AddNotify(AnimName, "OnFirstSlam", 20,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);

	// 6) Player ������ ���� �ٲٱ� -> �Ʒ��� Init ������, ChangeAttackDir ��忡�� ���ְ� �ִ�.
	//AnimName = "BackUpStep";
	//
	//AnimInst->AddNotifyDeltaTimeFrameRange(AnimName, "OnTracePlayer", 0, 19,
	//		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnLookPlayer);
}

NodeResult CBossBettyCloseAttackNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// �ٰŸ� ���� Ÿ���� üũ�Ѵ�.
	m_CloseAttackType = DetermineBettyCloseAttackType();

	switch (m_CloseAttackType)
	{
	case BossBettyCloseAttackType::PunchLeft :
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
	case BossBettyCloseAttackType::NotInCloseRange:
	{
		AnimInst->ChangeAnimation("BackUpStep"); 
	}
	break;
	}

	return NodeResult::Node_True;
}

NodeResult CBossBettyCloseAttackNode::OnUpdate(float DeltaTime)
{
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

	const Vector3& RelativeXAxis = m_Object->GetRootComponent()->GetRelativeAxis(AXIS_X);
	const Vector3& RelativeZAxis = m_Object->GetRootComponent()->GetRelativeAxis(AXIS_Z);
	const Vector3& WorldScale = m_Object->GetRootComponent()->GetWorldScale();
	const Vector3& WorldPos = m_Object->GetRootComponent()->GetWorldPos();

	// Player ����
	CGameObject* PlayerObj = CSceneManager::GetInst()->GetScene()->GetPlayerObject();
	Vector2 Player2DWorldPos = Vector2(PlayerObj->GetWorldPos().x, PlayerObj->GetWorldPos().z);

	// ���� ������ ���Ѵ�.
	Box2DInfo CheckBoxInfo;
	CheckBoxInfo.Axis[0] = Vector2(RelativeXAxis.x, RelativeXAxis.z);
	CheckBoxInfo.Axis[1] = Vector2(RelativeZAxis.x, RelativeZAxis.z);

	// 1) Box2DInfo m_PunchLeftSquareBox;
	Vector2 BoxLeftDown = Vector2(WorldPos.x + PunchLeftSquarePoses[0].x * RelativeXAxis.x,
		WorldPos.z + PunchLeftSquarePoses[0].z * RelativeXAxis.z);
	Vector2 BoxRightUp = Vector2(WorldPos.x + PunchLeftSquarePoses[2].x * RelativeXAxis.x,
		WorldPos.z + PunchLeftSquarePoses[2].z * RelativeXAxis.z);

	CheckBoxInfo.Center = (BoxLeftDown + BoxRightUp) / 2.f;

	CollisionResult SrcResult;
	CollisionResult DestResult;
	if (CCollision::CollisionBox2DToPoint(SrcResult, DestResult, CheckBoxInfo, Player2DWorldPos))
		return BossBettyCloseAttackType::PunchLeft;

	// 2) Box2DInfo m_PunchRightSquareBox;
	const Vector3* const& PunchRightSquarePoses = Data->GetPunchRightSquarePoses();

	BoxLeftDown = Vector2(WorldPos.x + PunchRightSquarePoses[0].x * RelativeXAxis.x,
		WorldPos.z + PunchRightSquarePoses[0].z * RelativeXAxis.z);
	BoxRightUp = Vector2(WorldPos.x + PunchRightSquarePoses[2].x * RelativeXAxis.x,
		WorldPos.z + PunchRightSquarePoses[2].z * RelativeXAxis.z);

	CheckBoxInfo.Center = (BoxLeftDown + BoxRightUp) / 2.f;

	if (CCollision::CollisionBox2DToPoint(SrcResult, DestResult, CheckBoxInfo, Player2DWorldPos))
		return BossBettyCloseAttackType::PunchRight;

	// 3) Box2DInfo m_SlashLeftSquareBox;
	const Vector3* const& SlashLeftSquarePoses = Data->GetSlashLeftSquarePoses();

	BoxLeftDown = Vector2(WorldPos.x + SlashLeftSquarePoses[0].x * RelativeXAxis.x,
		WorldPos.z + SlashLeftSquarePoses[0].z * RelativeXAxis.z);
	BoxRightUp = Vector2(WorldPos.x + SlashLeftSquarePoses[2].x * RelativeXAxis.x,
		WorldPos.z + SlashLeftSquarePoses[2].z * RelativeXAxis.z);

	CheckBoxInfo.Center = (BoxLeftDown + BoxRightUp) / 2.f;

	if (CCollision::CollisionBox2DToPoint(SrcResult, DestResult, CheckBoxInfo, Player2DWorldPos))
		return BossBettyCloseAttackType::SlashLeft;

	// 4) Box2DInfo m_SlashRightSquareBox;
	const Vector3* const& SlashRightSquarePoses = Data->GetSlashRightSquarePoses();

	BoxLeftDown = Vector2(WorldPos.x + SlashRightSquarePoses[0].x * RelativeXAxis.x,
		WorldPos.z + SlashRightSquarePoses[0].z * RelativeXAxis.z);
	BoxRightUp = Vector2(WorldPos.x + SlashRightSquarePoses[2].x * RelativeXAxis.x,
		WorldPos.z + SlashRightSquarePoses[2].z * RelativeXAxis.z);

	CheckBoxInfo.Center = (BoxLeftDown + BoxRightUp) / 2.f;

	if (CCollision::CollisionBox2DToPoint(SrcResult, DestResult, CheckBoxInfo, Player2DWorldPos))
		return BossBettyCloseAttackType::SlashRight;
	
	// 5) Box2DInfo m_TwoSideFrontSquareBox;
	const Vector3* const& TwoSideFrontSquarePoses = Data->GetTwoSideFrontSquarePoses();

	BoxLeftDown = Vector2(WorldPos.x + TwoSideFrontSquarePoses[0].x * RelativeXAxis.x,
		WorldPos.z + TwoSideFrontSquarePoses[0].z * RelativeXAxis.z);
	BoxRightUp = Vector2(WorldPos.x + TwoSideFrontSquarePoses[2].x * RelativeXAxis.x,
		WorldPos.z + TwoSideFrontSquarePoses[2].z * RelativeXAxis.z);

	CheckBoxInfo.Center = (BoxLeftDown + BoxRightUp) / 2.f;

	if (CCollision::CollisionBox2DToPoint(SrcResult, DestResult, CheckBoxInfo, Player2DWorldPos))
		return BossBettyCloseAttackType::TwoSideFront;

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
