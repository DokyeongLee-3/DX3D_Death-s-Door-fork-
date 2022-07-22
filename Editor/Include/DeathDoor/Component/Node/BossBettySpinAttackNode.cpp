#include "BossBettySpinAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../BossBettyDataComponent.h"

CBossBettySpinAttackNode::CBossBettySpinAttackNode()
{
	SetTypeID(typeid(CBossBettySpinAttackNode).hash_code());
}

CBossBettySpinAttackNode::CBossBettySpinAttackNode(const CBossBettySpinAttackNode& Node)
{
}

CBossBettySpinAttackNode::~CBossBettySpinAttackNode()
{
}

void CBossBettySpinAttackNode::Init()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// 1) JumpSpin
	std::string AnimName = "JumpSpin";

	// JumpSpin Animation �� ������ �ٷ� Spin Animation ���� �ٲ۴�.
	AnimInst->SetEndFunction(AnimName,
		this, &CBossBettySpinAttackNode::OnBossBettyChangeToSpinAnimation);

	// 2) Spin
	AnimName = "Spin";

	// Spin �� �ϴ� ����, ó���� ColliderBox �� Ư�� Callback �� �Ǵ�.
	// - �� ��� �浹 �ÿ� SpinCollide �� �ٲ�� �Ѵ�.
	// - �ش� ColliderBox ��, �ݹ��Լ��� �̹� BossBettyDataComponent ���� ���õ� �����̴�.
	// - �̸� �ٽ� Enable �� �����ָ� �ȴ�.
	AnimInst->AddNotify(AnimName, "EnableSpinCollider", 0,
		Data, &CBossBettyDataComponent::OnBossBettyEnableSpinCollider);

	// 2) Spin Collide
	AnimName = "SpinCollide";

	// ó�� �����ϴ� ����, �ٽ� Spin Collide �� ��Ȱ��ȭ ��Ų��.
	AnimInst->AddNotify(AnimName, "DisableSpinCollider", 0, 
		Data, &CBossBettyDataComponent::OnBossBettyDisableSpinCollider);

	// ���߿��� Player �� ���� ������ ����.
	// ������ ������ Player �������� ȸ���� �� �ֵ��� �Ѵ�.
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);
	AnimInst->AddNotify(AnimName, "OnDisableTracePlayer", 16,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);

	// ������ ������ ������ �ٴ��� �����Ѵ�.
	AnimInst->SetEndFunction(AnimName,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);

	// ���߿��� ȸ���ϴ� �߰��� Player ������ ������ ������ ư��.
}

NodeResult CBossBettySpinAttackNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	AnimInst->ChangeAnimation("JumpSpin");

	return NodeResult::Node_True;
}

NodeResult CBossBettySpinAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CBossBettySpinAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

void CBossBettySpinAttackNode::OnBossBettyChangeToSpinAnimation()
{
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	AnimInst->ChangeAnimation("Spin");
}

