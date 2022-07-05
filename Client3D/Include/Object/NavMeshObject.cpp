
#include "NavMeshObject.h"
#include "Scene/Scene.h"
#include "Resource/Mesh/NavMesh.h"

CNavMeshObject::CNavMeshObject()
{
	SetTypeID<CNavMeshObject>();
}

CNavMeshObject::CNavMeshObject(const CNavMeshObject& obj)	:
	CGameObject(obj)
{
}

CNavMeshObject::~CNavMeshObject()
{
}

bool CNavMeshObject::Init()
{
	m_NavMeshComponent = CreateComponent<CNavMeshComponent>("NavMeshComponent");
	m_Scene->GetNavigation3DManager()->SetNavMeshData(m_NavMeshComponent);

	SetWorldScale(2.f, 1.f, 2.f);

	// 
	m_NavMeshComponent->GetTransform()->ForceUpdateMat();

	// ���� �÷��̾ �ʱ⿡ ��ġ�� polygon�� index�� ã���ֱ� ���� ���ʷ� �ѹ� CheckPlayerNavMeshPoly�� ȣ�����ְ�
	// �� �����Ӹ��� CheckPlayerNavMeshPoly�� ȣ���ؼ� �÷��̾ �̵������� polygon�� �̵��Ϸ����� üũ�ϴµ� �׶� �̹� NavMesh�� transform ��� ������
	// �ϼ��Ǿ� �־�� �Ѵ�. �׷��� transform::PostUpdate���� ��� ������ �ϼ��ϴµ� player���� ���� NavMeshComponent�� PostUpdate�ȴٴ� ������ �����Ƿ�
	// ������ ������ transform������ �ϼ����ִ� ForceUpdateMat�� ȣ�����ش�
	m_Scene->GetNavigation3DManager()->CheckPlayerNavMeshPoly();

	//m_NavMeshComponent->CreateNavMesh("NavMesh");
	//CNavMesh* Mesh = (CNavMesh*)m_Scene->GetResource()->FindMesh("NavMesh");

	//m_NavMeshComponent->CopyNavMeshPolygonInfo(Mesh->GetNavMeshPolygonVec());
	//m_NavMeshComponent->CheckAdjInfo();

	return true;
}

void CNavMeshObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CNavMeshObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CNavMeshObject* CNavMeshObject::Clone()
{
	return new CNavMeshObject(*this);
}
