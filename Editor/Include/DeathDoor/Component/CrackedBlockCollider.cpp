#include "CrackedBlockCollider.h"
#include "Component/StaticMeshComponent.h"
#include "GameObject/GameObject.h"
#include "../DDUtil.h"

CCrackedBlockCollider::CCrackedBlockCollider()	:
	m_Reverse(false),
	m_Mesh(nullptr),
	m_Timer(1.f),
	m_ColorChangeTimer(5.f)
{
	SetTypeID<CCrackedBlockCollider>();
}

CCrackedBlockCollider::CCrackedBlockCollider(const CCrackedBlockCollider& com)	:
	CColliderBox3D(com)
{
}

CCrackedBlockCollider::~CCrackedBlockCollider()
{
}

void CCrackedBlockCollider::Start()
{
	CColliderBox3D::Start();

	m_Mesh = m_Object->FindComponentFromType<CStaticMeshComponent>();
}

void CCrackedBlockCollider::Update(float DeltaTime)
{
	CColliderBox3D::Update(DeltaTime);

	m_Timer += DeltaTime;

	float Time = m_Timer;
	if (m_Reverse)
	{
		Time = m_ColorChangeTimer - Time;
	}

	float Ratio = Time / m_ColorChangeTimer;
	m_Mesh->SetSpecularColor(Vector4(Ratio, Ratio, Ratio, Ratio));

	if (m_Timer >= m_ColorChangeTimer)
	{
		m_Reverse = !m_Reverse;
		m_Timer = 0.f;
	}
}
