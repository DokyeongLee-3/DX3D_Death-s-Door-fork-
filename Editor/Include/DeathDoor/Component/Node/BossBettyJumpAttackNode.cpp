#include "BossBettyJumpAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../BossBettyDataComponent.h"

CBossBettyJumpAttackNode::CBossBettyJumpAttackNode()
{
}

CBossBettyJumpAttackNode::CBossBettyJumpAttackNode(const CBossBettyJumpAttackNode& Node)
{
}

CBossBettyJumpAttackNode::~CBossBettyJumpAttackNode()
{
}

void CBossBettyJumpAttackNode::Init()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// PunchDouble
	std::string AnimName = "PunchDouble";

	// JumpSpin Animation �� ������ �ش� ��ġ�� 2�� ����
	AnimInst->SetEndFunction(AnimName,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);

	// ���ư��� �߰�����, Player �� ���� ���� ȸ���ϱ� 
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 9,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);
	AnimInst->AddNotify(AnimName, "OnDisableTracePlayer", 22,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);
}

NodeResult CBossBettyJumpAttackNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	AnimInst->ChangeAnimation("PunchDouble");

	return NodeResult::Node_True;
}

NodeResult CBossBettyJumpAttackNode::OnUpdate(float DeltaTime)
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// �ڽ��� �� �������� ���ư��� �ϱ� 
	Vector3 MyAxisZ = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
	m_Object->AddWorldPos(MyAxisZ * Data->GetMoveSpeed() * DeltaTime);

	return NodeResult::Node_True;
}

NodeResult CBossBettyJumpAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
