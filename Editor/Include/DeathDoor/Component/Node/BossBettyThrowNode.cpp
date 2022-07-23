#include "BossBettyThrowNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/Scene.h"
#include "../BossBettyDataComponent.h"

CBossBettyThrowNode::CBossBettyThrowNode()
{
	SetTypeID(typeid(CBossBettyThrowNode).hash_code());
}

CBossBettyThrowNode::CBossBettyThrowNode(const CBossBettyThrowNode& Node)
{
}

CBossBettyThrowNode::~CBossBettyThrowNode()
{
}

void CBossBettyThrowNode::Init()
{
	m_CallStart = true;

	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string AnimName = "Throw";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// ������ ������ Player �������� ȸ���� �� �ֵ��� �Ѵ�.
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);
	AnimInst->AddNotify(AnimName, "OnDisableTracePlayer", 26,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);

	AnimInst->AddNotify(AnimName, "MakeSnowBallAttackObj", 9, this, &CBossBettyThrowNode::MakeSnowBallAttackObj);
	AnimInst->AddNotify(AnimName, "ThrowSnowBallAttackObj", 27, this, &CBossBettyThrowNode::ThrowSnowBallAttackObj);
}

NodeResult CBossBettyThrowNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("Throw");

	return NodeResult::Node_True;
}

NodeResult CBossBettyThrowNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CBossBettyThrowNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

// Snow Ball Attack Obj ����� -> Socket ���·� �޾Ƴ���
void CBossBettyThrowNode::MakeSnowBallAttackObj()
{
	// �Ʒ��� ����, Component �� ��� �ִ�, Ư�� Object �� ���� ���̴�.
	// - Project Tile
	// - Mesh
	// - Particle
	// - Collider Component
	// - �װ���, ������ ���̴�. (�ش� ������ BossBettyDataComponent �� ��� �ְ� �� ���̴�)
}

// Snow Ball Attack Obj �� Player �������� ������ 
void CBossBettyThrowNode::ThrowSnowBallAttackObj()
{
	// BossBettyDataComponent �κ���, BossThrowBall Object �� ��������
	// �ű⿡ ������ �������� ���⿡ ���� ���̴�.
}