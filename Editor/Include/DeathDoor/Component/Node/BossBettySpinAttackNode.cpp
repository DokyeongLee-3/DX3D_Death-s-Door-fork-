#include "BossBettySpinAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../BossBettyDataComponent.h"

CBossBettySpinAttackNode::CBossBettySpinAttackNode() :
	m_JumpSpinChangeEnable(true)
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

	// �ſ� ������ ������ �� �� �ֵ��� �Ѵ�.
	// Move Speed �� ��� BossDataComponent ���� ó�����ְ� �ִ�.
	AnimInst->AddNotify(AnimName, "EnableZMove", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableMoveZ);
	AnimInst->AddNotify(AnimName, "SlowMoveSpeed", 0,
		Data, &CBossBettyDataComponent::OnBossBettyApplyOutOfMapSurroundingColliderMoveSpeed);

	// End
	// JumpSpin Animation �� ������ �ٷ� Spin Animation ���� �ٲ۴�.
	AnimInst->SetEndFunction(AnimName,
		this, &CBossBettySpinAttackNode::OnBossBettyChangeToSpinAnimation);
	AnimInst->AddNotify(AnimName, "CameraShake", 17,
		Data, &CBossBettyDataComponent::OnBossBettyNormalShakeCamera);

	// 2) Spin
	AnimName = "Spin";

	// Spin �� �ϴ� ����, ó���� ColliderBox �� Ư�� Callback �� �Ǵ�.
	// - �� ��� �浹 �ÿ� SpinCollide �� �ٲ�� �Ѵ�.
	// - �ش� ColliderBox ��, �ݹ��Լ��� �̹� BossBettyDataComponent ���� ���õ� �����̴�.
	// - �̸� �ٽ� Enable �� �����ָ� �ȴ�.
	AnimInst->AddNotify(AnimName, "EnableSpinCollider", 0,
		Data, &CBossBettyDataComponent::OnBossBettyEnableSpinCollider);
	AnimInst->AddNotify(AnimName, "ResetMoveSpeed", 0,
		Data, &CBossBettyDataComponent::OnBossBettyResetOriginalMoveSpeed);
	AnimInst->AddNotify(AnimName, "EnableAttackCollider", 2,
		Data, &CBossBettyDataComponent::OnBossBettyEnableAttackCollider);
	AnimInst->AddNotify(AnimName, "AttackColliderToOriginalPs", 0,
		Data, &CBossBettyDataComponent::OnSetBossBettyAttackColliderPosToBettyBody);

	// 2) Spin Collide
	AnimName = "SpinCollide";

	// ó�� �����ϴ� ����, �ٽ� Spin Collide �� ��Ȱ��ȭ ��Ų��.
	AnimInst->AddNotify(AnimName, "DisableSpinCollider", 0, 
		Data, &CBossBettyDataComponent::OnBossBettyDisableSpinCollider);
	// ������ ���� ���� ��ȿȭ
	AnimInst->AddNotify(AnimName, "DisableZMove", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableMoveZ);
	// ���Ÿ� ���� Ƚ�� 1 ����
	AnimInst->AddNotify(AnimName, "AddFarAttakCount", 0,
		Data, &CBossBettyDataComponent::IncFarAttackCount);
	// ���߿��� Player �� ���� ������ ����.
	// ������ ������ Player �������� ȸ���� �� �ֵ��� �Ѵ�.
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);

	// �߰�
	AnimInst->AddNotify(AnimName, "CameraShake", 20,
		Data, &CBossBettyDataComponent::OnBossBettyNormalShakeCamera);

	// >> EndFunctions
	AnimInst->AddNotify(AnimName, "DisablePlayerLook", 26,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);
	// ������ ������ ������ �ٴ��� �����Ѵ�.
	AnimInst->AddNotify(AnimName, "AttackDown", 26,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);
	// ��� Spin�� ������ ������ ��μ�, CurrentNode �� nullptr �� �����Ѵ�.
	AnimInst->AddNotify(AnimName, "SetCurrentNodeNull", 26,
		Data, &CBossBettyDataComponent::OnBossBettySetCurrentNodeNullPtr);
	// Jump Spin ������ ��ȯ�� ����Ѵ�.
	AnimInst->AddNotify(AnimName, "EnableJumpSpinChange", 26,
		this, &CBossBettySpinAttackNode::OnBossBettyEnableSpinChange);

	AnimInst->AddNotify(AnimName, "DisableAttackCollider", 26,
		Data, &CBossBettyDataComponent::OnBossBettyDisableAttackCollider);

}

NodeResult CBossBettySpinAttackNode::OnStart(float DeltaTime)
{
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// Spin ���̶��, Animation �� �ٲ㼭�� �ȵȴ�.
	if (m_JumpSpinChangeEnable)
	{
		AnimInst->ChangeAnimation("JumpSpin");
		m_JumpSpinChangeEnable = false;
	}

	m_Owner->SetCurrentNode(this);

	return NodeResult::Node_True;
}

NodeResult CBossBettySpinAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CBossBettySpinAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

void CBossBettySpinAttackNode::OnBossBettyChangeToSpinAnimation()
{
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	AnimInst->ChangeAnimation("Spin");
}

void CBossBettySpinAttackNode::OnBossBettyEnableSpinChange()
{
	m_JumpSpinChangeEnable = true;
}

void CBossBettySpinAttackNode::OnBossBettyDisableSpinChange()
{
	m_JumpSpinChangeEnable = false;
}
