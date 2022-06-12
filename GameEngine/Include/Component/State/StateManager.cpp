
#include "StateManager.h"
#include "State.h"
#include "../ColliderBox3D.h"
#include "../ColliderSphere.h"
#include "../../GameObject/GameObject.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/CollisionSection.h"
#include "../StateComponent.h"
#include "DamageState.h"
#include "AirBorneState.h"

CStateManager::CStateManager()
{

}

CStateManager::~CStateManager()
{
	auto iter = m_mapState.begin();
	auto iterEnd = m_mapState.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CStateManager::Init()
{
	CAirBorneState* Airborne = new CAirBorneState;

	AddState("Airborne", Airborne);

	CDamageState* DamageState = new CDamageState;

	AddState("Damage", DamageState);
}

void CStateManager::AddState(const std::string& Name, CState* State)
{
	m_mapState.insert(std::make_pair(Name, State));
}

CState* CStateManager::FindState(const std::string& Name)
{
	auto iter = m_mapState.find(Name);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}

void CStateManager::CheckAirborneTarget(const NotifyParameter& Param)
{
	// Param.MyPos�� �������� �˻��� Collision Section�� ã��, �� Section������ Param.Range���� �ִ� ������Ʈ�� StateList�� Airborne State�� �־��ش�
	CColliderBox3D* ColliderBox = Param.Object->FindComponentFromType<CColliderBox3D>();

	CColliderSphere* ColliderSphere = nullptr;

	bool ColliderBoxExist = false;
	bool ColliderSphereExist = false;

	if (ColliderBox)
		ColliderBoxExist = true;

	else
	{
		ColliderSphere = Param.Object->FindComponentFromType<CColliderSphere>();
		if (ColliderSphere)
			ColliderSphereExist = true;
	}

	// 2������ �浹ü ��� ���� ���� �ʴ� object�� ��� ó�� X
	if (!ColliderBoxExist && !ColliderSphereExist)
		return;

	CColliderComponent* Collider = nullptr;
	if (ColliderBoxExist)
		Collider = ColliderBox;
	else
		Collider = ColliderSphere;

	Vector3 ColliderPos = Collider->GetWorldPos();

	CSceneCollision* Collision = CSceneManager::GetInst()->GetScene()->GetCollision();

	int MinIndexX = Collision->GetSectionIndexX(Vector3(ColliderPos.x - Param.Range, ColliderPos.y, ColliderPos.z));
	int MinIndexZ = Collision->GetSectionIndexZ(Vector3(ColliderPos.x, ColliderPos.y, ColliderPos.z - Param.Range));
	int MaxIndexX = Collision->GetSectionIndexX(Vector3(ColliderPos.x + Param.Range, ColliderPos.y, ColliderPos.z));
	int MaxIndexZ = Collision->GetSectionIndexZ(Vector3(ColliderPos.x, ColliderPos.y, ColliderPos.z + Param.Range));

	int SectionCountX = Collision->GetSectionCountX();

	std::vector<int> vecTargetSection;

	for (size_t i = MinIndexZ; i <= MaxIndexZ; ++i)
	{
		for (size_t j = MinIndexX; j <= MaxIndexZ; ++j)
		{
			vecTargetSection.push_back(i * SectionCountX + j);
		}
	}

	size_t Count = vecTargetSection.size();

	for (size_t i = 0; i < Count; ++i)
	{
		// PrevSectionCollider�� �޾ƿ;��Ѵ�(������ CScene::Collsion ȣ�� ���̶� �� Section���� Collider�� ������ ���� ��Ȳ�̶� ���� ������ ���� Section��
		// Collider�� ����ִ� PrevSectionCollider�� �޾ƿ;� �Ѵ�
		CCollisionSection* Section = Collision->GetCollisionSectionInfo()->vecSection[vecTargetSection[i]];
		size_t PrevSectionSize = Section->GetPrevColliderSize();

		for (size_t j = 0; j < PrevSectionSize; ++j)
		{
			CColliderComponent* PrevSectionCollider = Section->GetPrevCollider(j);
			if (PrevSectionCollider == Collider)
				continue;

			Vector3 CandidatePos = PrevSectionCollider->GetWorldPos();
			CGameObject* CandidateObject = PrevSectionCollider->GetGameObject();
			if (CandidatePos.Distance(Collider->GetWorldPos()) <= Param.Range)
			{
				CStateComponent* StateComp = CandidateObject->FindComponentFromType<CStateComponent>();

				if (StateComp)
				{
					StateComp->AddState("Airborne", Param.Object);
					CState* DamageState = StateComp->AddState("Damage", Param.Object);

					((CDamageState*)DamageState)->SetDamage(1234); // TODO : ���� csv�� ��ų ������ ��Ȯ�ϰ� �о�ͼ� �ٲ��ֱ�
					CandidateObject->SetOntHit(true);
				}
			}
		}
	}

	// �浹ü�� ���� Section��
	//std::vector<int> vecColliderSection = Collider->GetPrevSections();
	//size_t Count = vecColliderSection.size();

	//std::vector<CGameObject*>	vecAirborneTarget;
	//CSceneCollision* Collision = CSceneManager::GetInst()->GetScene()->GetCollision();

	//for (size_t i = 0; i < Count; ++i)
	//{
	//	CCollisionSection* Section = Collision->GetCollisionSectionInfo()->vecSection[vecColliderSection[i]];

	//	size_t Size = Section->GetPrevColliderSize();

	//	for (size_t i = 0; i < Size; ++i)
	//	{
	//		if (Collider == Section->GetPrevCollider(i))
	//			continue;

	//		Vector3 CandidatePos = Section->GetPrevCollider(i)->GetWorldPos();
	//		CGameObject* CandidateObject = Section->GetPrevCollider(i)->GetGameObject();

	//		// ��� ���� ����̴�
	//		if (abs(CandidatePos.Distance(Collider->GetWorldPos())) < Param.Range)
	//		{
	//			CStateComponent* StateComp = CandidateObject->FindComponentFromType<CStateComponent>();

	//			if (StateComp)
	//				StateComp->AddState("Airborne");
	//		}
	//	}
	//}



}

void CStateManager::FindRestraintTarget(const NotifyParameter& Param)
{
}

void CStateManager::FindStunTarget(const NotifyParameter& Param)
{
}

void CStateManager::FindKnockBackTarget(const NotifyParameter& Param)
{
}

