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
	AnimInst->AddNotify(AnimName, "CameraShake", 9,
		Data, &CBossBettyDataComponent::OnBossBettyNormalShakeCamera);

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

	AnimInst->AddNotify(AnimName, "ActivateRoarParticle", 100,
		Data, &CBossBettyDataComponent::OnBossBettyActivateRoarParticle);
	
	AnimInst->AddNotify(AnimName, "CameraShake", 100,
		Data, &CBossBettyDataComponent::OnBossBettyNormalShakeCamera);

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

	const Vector3& MapSurroundingObjectWorldPos = MapSurroundingObject->GetWorldPos();

	CColliderSphere* ColliderSphere = MapSurroundingObject->FindComponentFromType<CColliderSphere>();

	int SphereRadius = (int)ColliderSphere->GetInfo().Radius;

	const Vector3& PlayerPos = CSceneManager::GetInst()->GetScene()->GetPlayerObject()->GetWorldPos();

	for (int i = 0; i < 7; ++i)
	{
		int XRand = rand() % SphereRadius;
		int YRand = SphereRadius + rand() % (SphereRadius * 2);
		int ZRand = rand() % SphereRadius;

		float RandV = ((float)rand() / (RAND_MAX)) + 1;
		float RandomAngle = 360.f * RandV;

		CGameObject* SnowFallingObject = CObjectPool::GetInst()->GetProjectile("BossBettySnowAttack", CurrentScene);

		SnowFallingObject->SetWorldPos(MapSurroundingObjectWorldPos + Vector3(
			XRand * cos(RandomAngle), (float)YRand, ZRand * sin(RandomAngle)));
			
		CProjectileComponent* ProjTileComp = SnowFallingObject->FindComponentFromType<CProjectileComponent>();

		CGameObject* AfterEffectParticle = CObjectPool::GetInst()->GetParticle("BettyAttackAfterEffect", CSceneManager::GetInst()->GetScene());

		const Vector3& SnowObjectWorldPos = SnowFallingObject->GetWorldPos();

		ProjTileComp->ShootByTargetPos(SnowObjectWorldPos, 30.f + rand() % 10, 
			Vector3(SnowObjectWorldPos.x, PlayerPos.y + 2.f, SnowObjectWorldPos.z), AfterEffectParticle);
	}

	// 2. ������ ���ؼ�, �浹�� ���۽�ų �ݹ���� �����Ѵ�.
	// - ����ü�� ���, ThrowNode, AngryAttackNode �� �ٿ��� �����ϱ� ������ 
	// - BossBettyDataComponent::OnExplodeBettyThrowBallCallback �� ����
}

