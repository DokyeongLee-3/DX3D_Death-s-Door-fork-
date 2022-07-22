#include "BossBettyChangeAttackDirNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/Scene.h"
#include "../BossBettyDataComponent.h"

CBossBettyChangeAttackDirNode::CBossBettyChangeAttackDirNode()
{
	SetTypeID(typeid(CBossBettyChangeAttackDirNode).hash_code());
}

CBossBettyChangeAttackDirNode::CBossBettyChangeAttackDirNode(const CBossBettyChangeAttackDirNode& Node)
{
}

CBossBettyChangeAttackDirNode::~CBossBettyChangeAttackDirNode()
{
}

void CBossBettyChangeAttackDirNode::Init()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	// �������� �پ� �����鼭, Player �� �ִ� ������ ���� ������ �ٲ۴�.
	std::string AnimName = "BackUpStep";

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);
	AnimInst->AddNotify(AnimName, "OnDisableTracePlayer", 19,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);
}

NodeResult CBossBettyChangeAttackDirNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("BackUpStep");

	return NodeResult::Node_True;
}

NodeResult CBossBettyChangeAttackDirNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CBossBettyChangeAttackDirNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
