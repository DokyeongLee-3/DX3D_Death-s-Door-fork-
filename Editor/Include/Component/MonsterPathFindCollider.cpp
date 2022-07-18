
#include "MonsterPathFindCollider.h"
#include "Collision/CollisionManager.h"
#include "GameObject/GameObject.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Navigation3DManager.h"

CMonsterPathFindCollider::CMonsterPathFindCollider()	:
	m_PathFindEnable(true),
	m_PathFindCoolStart(true),
	m_AccTime(0.f),
	m_PathFindCoolTime(2.f)
{
	SetTypeID<CMonsterPathFindCollider>();
	m_ComponentType = Component_Type::SceneComponent;

	m_LayerName = "Collider";
	m_Render = true;

	m_Profile = CCollisionManager::GetInst()->FindProfile("MonsterPathFind");

	AddCollisionCallback<CMonsterPathFindCollider>(Collision_State::Begin, this, &CMonsterPathFindCollider::SetPathFindFalse);
	AddCollisionCallback<CMonsterPathFindCollider>(Collision_State::End, this, &CMonsterPathFindCollider::SetPathFindCoolStart);

	// �ٸ� ���Ϳ� �浹�Ѵٸ� ��ġ�� �ʰ� �о��
	AddCollisionCallback<CMonsterPathFindCollider>(Collision_State::Stay, this, &CMonsterPathFindCollider::PushEachOther);
}

CMonsterPathFindCollider::CMonsterPathFindCollider(const CMonsterPathFindCollider& com)	:
	CColliderSphere(com)
{
}

CMonsterPathFindCollider::~CMonsterPathFindCollider()
{
}

void CMonsterPathFindCollider::SetPathFindFalse(const CollisionResult& Result)
{
	if (Result.Dest->GetGameObject()->GetObjectType() == Object_Type::Player)
	{
		m_PathFindEnable = false;
		m_PathFindCoolStart = false;
		m_AccTime = 0.f;
	}
}
void CMonsterPathFindCollider::PushEachOther(const CollisionResult& Result)
{
	if (Result.Dest->GetCollisionProfile()->Name == "MonsterPathFind")
	{
		CGameObject* DestObject = Result.Dest->GetGameObject();

		Vector3 SrcPos = m_Object->GetRootComponent()->GetWorldPos();
		Vector3 DestObjectPos = DestObject->GetWorldPos();
		CNavAgent* DestObjNavAgent = DestObject->FindObjectComponentFromType<CNavAgent>();

		if (!m_NavAgent || !DestObjNavAgent)
			return;

		std::list<Vector3> DestPathList = DestObjNavAgent->GetPathList();

		if (!m_NavAgent->IsEmptyPathList() && !DestPathList.empty())
		{
			auto iter = DestPathList.back();

			if (iter.Distance(SrcPos) > 4.f)
			{
				DestPathList.pop_back();
				Vector3 Dir = DestObjectPos - SrcPos;
				Dir.Normalize();
				DestPathList.push_back(DestObjectPos + Dir * 10.f);
				m_NavAgent->AddPath(SrcPos);
				return;
			}
		}

		// �տ� �ִ� ���Ͱ� �÷��̾�� �浹�ؼ� �����ְ�, ���� ���� ��ã�Ⱑ ���� ��� ���� ������ ���ݾ� ȸ�����ش�
		else if(!m_NavAgent->IsEmptyPathList())
		{
			std::list<Vector3> PathList = m_NavAgent->GetPathList();
			Vector3 FaceDir = m_NavAgent->GetCurrentFaceDir();

			Vector3 DestPos = PathList.back();



			PathList.pop_back();
		}

	}
}

void CMonsterPathFindCollider::Start()
{
	CColliderSphere::Start();

	m_NavAgent = m_Object->FindObjectComponentFromType<CNavAgent>();
}

bool CMonsterPathFindCollider::Init()
{
	if (!CColliderSphere::Init())
		return false;

	return true;
}

void CMonsterPathFindCollider::Update(float DeltaTime)
{
	CColliderSphere::Update(DeltaTime);

	if (m_PathFindCoolStart)
	{
		m_AccTime += DeltaTime;

		if (m_AccTime >= m_PathFindCoolTime)
		{
			m_AccTime = 0.f;
			m_PathFindEnable = true;
		}
	}
}

void CMonsterPathFindCollider::PostUpdate(float DeltaTime)
{
	CColliderSphere::PostUpdate(DeltaTime);

}

void CMonsterPathFindCollider::PrevRender()
{
	CColliderSphere::PrevRender();
}

void CMonsterPathFindCollider::Render()
{
	CColliderSphere::Render();
}

void CMonsterPathFindCollider::PostRender()
{
	CColliderSphere::PostRender();
}

CMonsterPathFindCollider* CMonsterPathFindCollider::Clone()
{
	return new CMonsterPathFindCollider(*this);
}

bool CMonsterPathFindCollider::Save(FILE* File)
{
	return CColliderSphere::Save(File);

}

bool CMonsterPathFindCollider::Load(FILE* File)
{
	return CColliderSphere::Load(File);
}

bool CMonsterPathFindCollider::SaveOnly(FILE* File)
{
	return false;
}

bool CMonsterPathFindCollider::LoadOnly(FILE* File)
{
	return false;
}
