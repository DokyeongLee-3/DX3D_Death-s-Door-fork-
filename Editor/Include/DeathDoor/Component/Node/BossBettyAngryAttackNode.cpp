#include "BossBettyAngryAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "../ProjectileComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"
#include "Scene/Scene.h"
#include "../BossBettyDataComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "EngineUtil.h"
#include "ObjectPool.h"

CBossBettyAngryAttackNode::CBossBettyAngryAttackNode()
{
	SetTypeID(typeid(CBossBettyAngryAttackNode).hash_code());
}

CBossBettyAngryAttackNode::CBossBettyAngryAttackNode(const CBossBettyAngryAttackNode& Node)
{
}

CBossBettyAngryAttackNode::~CBossBettyAngryAttackNode()
{
}

void CBossBettyAngryAttackNode::Init()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	// 1) HPDamageAngry
	std::string AnimName = "FirstSlamAnger";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// ������ ������ Player �������� ȸ���� �� �ֵ��� �Ѵ�.
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);
	AnimInst->AddNotify(AnimName, "OnDisableTracePlayer", 5,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);

	// Snow Ball Falling
	AnimInst->AddNotify(AnimName, "OnStartFallingSnowBallEffect", 9, 
		this, &CBossBettyAngryAttackNode::OnBossBettyStartFallingSnowBallEffect);

	// ����
	AnimInst->AddNotify(AnimName, "OnTwoSideCloseAttackEffect", 25, 
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);

	// ����
	AnimInst->AddNotify(AnimName, "OnLeftAttackDownEffect", 34, 
		Data, &CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect);

	// ������
	AnimInst->AddNotify(AnimName, "OnRightAttackDownEffect", 43, 
		Data, &CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect);

	// ����
	AnimInst->AddNotify(AnimName, "OnLeftAttackDownEffect", 52, 
		Data, &CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect);

	// ������
	AnimInst->AddNotify(AnimName, "OnRightAttackDownEffect", 62,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect);

	// ����
	AnimInst->AddNotify(AnimName, "OnTwoSideCloseAttackEffect", 78,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);

	// ���¢��
	AnimInst->AddNotifyDeltaTimeFrameRange(AnimName, "OnBettyRoar", 100, 120,
		Data, &CBossBettyDataComponent::OnBossBettyRoarEffect);

	// End
	AnimInst->AddNotify(AnimName, "ResetHPState", 121,
		Data, &CBossBettyDataComponent::ResetBettyHPState);

	AnimInst->AddNotify(AnimName, "ResetCurrentNode", 121,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::SetCurrentNodeNull);
}

NodeResult CBossBettyAngryAttackNode::OnStart(float DeltaTime)
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	
	m_Owner->SetCurrentNode(this);

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	AnimInst->ChangeAnimation("FirstSlamAnger");

	return NodeResult::Node_True;
}

NodeResult CBossBettyAngryAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CBossBettyAngryAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

void CBossBettyAngryAttackNode::OnBossBettyStartFallingSnowBallEffect()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	CScene* CurrentScene = CSceneManager::GetInst()->GetScene();

	// ��濡�� ����ü �����̰� �������� �Ѵ�.
	// 1. Ư�� ��ġ�� ����ü ������ Object �� ����
	CGameObject* MapSurroundingObject = CurrentScene->FindObject("MapSurrounding");

	CColliderSphere* ColliderSphere = MapSurroundingObject->FindComponentFromType<CColliderSphere>();

	int SphereRadius = (int)ColliderSphere->GetSphereInfo().Radius;

	const Vector3& PlayerPos = CSceneManager::GetInst()->GetScene()->GetPlayerObject()->GetWorldPos();

	int XRand = rand() % SphereRadius;
	int YRand = SphereRadius + rand() % (SphereRadius * 3);
	int ZRand = rand() % SphereRadius;

	for (int i = 0; i < 10; ++i)
	{
		CGameObject* SnowFallingObject = CObjectPool::GetInst()->GetProjectile("BossBettySnowAttack", CurrentScene);

		SnowFallingObject->SetWorldPos(PlayerPos + Vector3(XRand, YRand, ZRand));
			
		CProjectileComponent* ProjTileComp = SnowFallingObject->FindComponentFromType<CProjectileComponent>();

		CGameObject* AfterEffectParticle = CObjectPool::GetInst()->GetParticle("BettyAttackAfterEffect", CSceneManager::GetInst()->GetScene());

		const Vector3& SnowObjectWorldPos = SnowFallingObject->GetWorldPos();

		ProjTileComp->ShootByTargetPos(SnowObjectWorldPos, 50.f + rand() % 20, 
			Vector3(SnowObjectWorldPos.x, PlayerPos.y + 2.f, SnowObjectWorldPos.z), AfterEffectParticle);
	}

	// 2. ������ ���ؼ�, �浹�� ���۽�ų �ݹ���� �����Ѵ�.
	// - ����ü�� ���, ThrowNode, AngryAttackNode �� �ٿ��� �����ϱ� ������ 
	// - BossBettyDataComponent::OnExplodeBettyThrowBallCallback �� ����
}

