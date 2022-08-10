#include "ArrowCollisionFireCollider.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "ArrowComponent.h"
#include "GameObject/GameObject.h"
#include "../DDUtil.h"

CArrowCollisionFireCollider::CArrowCollisionFireCollider()
{
	SetTypeID<CArrowCollisionFireCollider>();
}

CArrowCollisionFireCollider::CArrowCollisionFireCollider(const CArrowCollisionFireCollider& com) :
	CColliderSphere(com)
{
	CColliderSphere::Start();

	// m_Mesh = m_Object->FindComponentFromType<CStaticMeshComponent>();

	AddCollisionCallback(Collision_State::Begin, this, &CArrowCollisionFireCollider::OnCollidePlayerArrow);
}

CArrowCollisionFireCollider::~CArrowCollisionFireCollider()
{
}

void CArrowCollisionFireCollider::Start()
{
	CColliderSphere::Start();

	AddCollisionCallback(Collision_State::Begin, this, &CArrowCollisionFireCollider::OnCollidePlayerArrow);

	m_Info.Radius = 2.0f;

	SetRelativePos(0.f, 2.f, 0.f);

	// Collision Profile �� ��� , Monster �� �����Ѵ�.
	SetCollisionProfile("Monster");

	// ��� Particle Component ���� Enable False ó���صд�.
	std::vector<CParticleComponent*> vecParticleComponents;
	m_Object->FindAllSceneComponentFromType<CParticleComponent>(vecParticleComponents);

	size_t ComponentSize = vecParticleComponents.size();

	for (size_t i = 0; i < ComponentSize; ++i)
	{
		vecParticleComponents[i]->Enable(false);
	}
}

void CArrowCollisionFireCollider::Update(float DeltaTime)
{
	CColliderSphere::Update(DeltaTime);
}

void CArrowCollisionFireCollider::OnCollidePlayerArrow(const CollisionResult& Result)
{
	if (Result.Dest->GetTypeID() == typeid(CArrowComponent).hash_code())
	{
		// ��� Particle Component ���� Start ó�����ش�.
		m_Object->StartParticle(m_Object->GetWorldPos());
	}
}
