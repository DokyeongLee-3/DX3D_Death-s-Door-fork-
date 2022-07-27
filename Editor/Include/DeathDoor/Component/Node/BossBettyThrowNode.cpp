#include "BossBettyThrowNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "../ProjectileComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "ObjectPool.h"
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

	// Start
	// ������ ������ Player �������� ȸ���� �� �ֵ��� �Ѵ�.
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);
	AnimInst->AddNotify(AnimName, "DisableZMove", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableMoveZ);

	// Middle
	AnimInst->AddNotify(AnimName, "MakeSnowBallAttackObj", 9, this, &CBossBettyThrowNode::MakeSnowBallAttackObj);

	// End
	AnimInst->AddNotify(AnimName, "ThrowSnowBallAttackObj", 27, this, &CBossBettyThrowNode::ThrowSnowBallAttackObj);



	AnimInst->SetEndFunction(AnimName, 
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::SetCurrentNodeNull);
}

NodeResult CBossBettyThrowNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("Throw");

	m_Owner->SetCurrentNode(this);

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
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// �Ʒ��� ����, Component �� ��� �ִ�, Ư�� Object �� ���� ���̴�.
	// - Project Tile
	// - Mesh
	// - Particle
	// - Collider Component
	// - �װ���, ������ ���̴�. (�ش� ������ BossBettyDataComponent �� ��� �ְ� �� ���̴�)
	CScene* CurrentScene = CSceneManager::GetInst()->GetScene();

	CGameObject* ThrowBall = CObjectPool::GetInst()->GetProjectile("BossBettySnowAttack", CurrentScene);

	if (ThrowBall == nullptr)
		return;

	Data->SetBettyThrowBallObject(ThrowBall);

	// Particle Component �� ã�Ƽ�, Bazier �̵��� ��Ų��.
	CParticleComponent* ParticleComp = ThrowBall->FindComponentFromType<CParticleComponent>();

	// Betty �ٷ� �տ� �����Ѵ�.
	Vector3 ZLookDir = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
	Vector3 YLookDir = m_Object->GetWorldAxis(AXIS::AXIS_Y);

	const Vector3& InitBallPos = m_Object->GetWorldPos() + ZLookDir * 7.f + YLookDir * 2.f;
	ThrowBall->SetWorldPos(InitBallPos);

	// Bazier �� ���� �̵��� �� �ְ� �����Ѵ�.
	const Vector3& D2 = m_Object->GetWorldPos() + ZLookDir * 8.f + YLookDir * 7.f;
	const Vector3& D3 = m_Object->GetWorldPos() + ZLookDir * 4.5f + YLookDir * 13.f;
	const Vector3& D4 = m_Object->GetWorldPos() + YLookDir * 11.f;

	ParticleComp->SetParticleMoveSpeed(75.f);
	ParticleComp->SetBazierTargetPos(D2, D3, D4, 30);
	ParticleComp->SetBazierMoveEffect(true);
}

// Snow Ball Attack Obj �� Player �������� ������ 
void CBossBettyThrowNode::ThrowSnowBallAttackObj()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// BossBettyDataComponent �κ���, BossThrowBall Object �� ��������
	// �ű⿡ ������ �������� ���⿡ ���� ���̴�.
	CGameObject* ThrowBall = Data->GetBossBettyThrowObject();
	CProjectileComponent* ProjTileComp = ThrowBall->FindComponentFromType<CProjectileComponent>();

	CGameObject* AfterEffectParticle = CObjectPool::GetInst()->GetParticle("BettyAttackAfterEffect", CSceneManager::GetInst()->GetScene());
	AfterEffectParticle->SetLifeSpan(5.f);
	AfterEffectParticle->Enable(false);

	const Vector3& PlayerPos = CSceneManager::GetInst()->GetScene()->GetPlayerObject()->GetWorldPos();

	ProjTileComp->ShootByTargetPos(ThrowBall->GetWorldPos(), 70.f, PlayerPos, AfterEffectParticle);
		
	// Throw Attack Enable �� �ٽ� False �� �ٲ��ش�
	Data->SetThrowAttackEnable(false);
}
