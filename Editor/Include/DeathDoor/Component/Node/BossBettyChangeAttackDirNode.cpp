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

	// 공중으로 뛰어 오르면서, Player 가 있는 쪽으로 공격 방향을 바꾼다.
	std::string AnimName = "BackUpStep";

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	
	// Middle
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 6,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);
	AnimInst->AddNotify(AnimName, "NoTracePlayer", 18,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);
	AnimInst->AddNotify(AnimName, "SetCurrentNodeNull", 20,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::SetCurrentNodeNull);
	AnimInst->AddNotify(AnimName, "DiableZMove", 20,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableMoveZ);

	// End
	AnimInst->SetEndFunction(AnimName, (CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);
}

void CBossBettyChangeAttackDirNode::OnResetOriginalRotSpeed()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	Data->SetCurRotSpeed(Data->GetOriginRotSpeed());
}

NodeResult CBossBettyChangeAttackDirNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("BackUpStep");

	m_Owner->SetCurrentNode(this);
		
	return NodeResult::Node_True;
}

NodeResult CBossBettyChangeAttackDirNode::OnUpdate(float DeltaTime)
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	float AngleToPlayer = Data->GetAnglePlayer();

	if (AngleToPlayer > 90.f)
		Data->SetCurRotSpeed(Data->GetOriginRotSpeed() * 2.f);
	else
		Data->SetCurRotSpeed(Data->GetOriginRotSpeed());

	return NodeResult::Node_True;
}

NodeResult CBossBettyChangeAttackDirNode::OnEnd(float DeltaTime)
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	Data->SetCurRotSpeed(Data->GetOriginRotSpeed());

	return NodeResult::Node_True;
}
