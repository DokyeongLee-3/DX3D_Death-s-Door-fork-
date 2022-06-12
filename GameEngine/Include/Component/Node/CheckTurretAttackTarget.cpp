
#include "CheckTurretAttackTarget.h"
#include "../ColliderBox3D.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/CollisionSection.h"
#include "../StateComponent.h"
#include "../State/State.h"

CCheckTurretAttackTarget::CCheckTurretAttackTarget()
{
	SetTypeID(typeid(CCheckTurretAttackTarget).hash_code());
}

CCheckTurretAttackTarget::CCheckTurretAttackTarget(const CCheckTurretAttackTarget& Node)	:
	CConditionNode(Node)
{
}

CCheckTurretAttackTarget::~CCheckTurretAttackTarget()
{
}

NodeResult CCheckTurretAttackTarget::OnStart(float DeltaTime)
{
	CGameObject* Target = m_Object->GetNormalAttackTarget();

	CGameObject* PriorTarget = CheckPriorAttackCandidate();

	if (PriorTarget)
	{
		m_Object->SetNormalAttackTarget(PriorTarget);

		m_TargetChampionList.clear();
		m_FriendChampionList.clear();

		return NodeResult::Node_True;
	}

	else
	{
		// ��ž�� ������ �����ߴ� ���� ����� ���� ��
		if (Target && Target->IsActive())
		{
			Vector3 TargetPos = Target->GetWorldPos();
			Vector3 MyPos = m_Object->GetWorldPos();

			float Dist = TargetPos.Distance(MyPos);

			if (Dist <= 30.f)
			{
				m_TargetChampionList.clear();
				m_FriendChampionList.clear();

				return NodeResult::Node_True;
			}

			// ������ �����ߴ� Ÿ���� ��ž ��Ÿ� ������ ����
			else
			{
				if (m_TargetChampionList.empty())
				{
					m_Object->SetNormalAttackTarget(nullptr);

					m_TargetChampionList.clear();
					m_FriendChampionList.clear();

					return NodeResult::Node_False;
				}
				else
				{
					auto iter = m_TargetChampionList.begin();
					m_Object->SetNormalAttackTarget(*iter);

					m_TargetChampionList.clear();
					m_FriendChampionList.clear();

					return NodeResult::Node_True;
				}
			}
		}

		else
		{
			if (m_TargetChampionList.empty())
			{
				m_Object->SetNormalAttackTarget(nullptr);

				m_TargetChampionList.clear();
				m_FriendChampionList.clear();

				return NodeResult::Node_False;
			}
			else
			{
				auto iter = m_TargetChampionList.begin();
				m_Object->SetNormalAttackTarget(*iter);

				m_TargetChampionList.clear();
				m_FriendChampionList.clear();

				return NodeResult::Node_True;
			}
		}
	}

}

NodeResult CCheckTurretAttackTarget::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CCheckTurretAttackTarget::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CCheckTurretAttackTarget::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}

CGameObject* CCheckTurretAttackTarget::CheckPriorAttackCandidate()
{
	CColliderBox3D* TurretCollider = m_Object->FindComponentFromType<CColliderBox3D>();

	if (!TurretCollider)
		return nullptr;

	std::vector<int> vecSections;

	CSceneCollision* Collision = CSceneManager::GetInst()->GetScene()->GetCollision();
	int SectionCountX = Collision->GetSectionCountX();

	Vector3 TurretPos = TurretCollider->GetWorldPos();

	int MinIndexX = Collision->GetSectionIndexX(Vector3(TurretPos.x - 10.f, TurretPos.y, TurretPos.z));
	int MinIndexZ = Collision->GetSectionIndexZ(Vector3(TurretPos.x, TurretPos.y, TurretPos.z - 10.f));
	int MaxIndexX = Collision->GetSectionIndexX(Vector3(TurretPos.x + 10.f, TurretPos.y, TurretPos.z));
	int MaxIndexZ = Collision->GetSectionIndexZ(Vector3(TurretPos.x, TurretPos.y, TurretPos.z + 10.f));

	// ��ž�� �����ϴ� Section�� ��ó Section���� ������
	for (size_t i = MinIndexZ; i <= MaxIndexZ; ++i)
	{
		for (size_t j = MinIndexX; j <= MaxIndexX; ++j)
		{
			vecSections.push_back((int)(i * SectionCountX + j));
		}
	}

	size_t TestSectionSize = vecSections.size();

	for (size_t i = 0; i < TestSectionSize; ++i)
	{
		CCollisionSection* Section = Collision->GetCollisionSectionInfo()->vecSection[vecSections[i]];
		size_t PrevColliderSize = Section->GetPrevColliderSize();

		for (size_t j = 0; j < PrevColliderSize; ++j)
		{
			CColliderComponent* TestCollider = Section->GetPrevCollider((int)j);
			CGameObject* TestObject = TestCollider->GetGameObject();
			Vector3 TestObjectPos = TestObject->GetWorldPos();
			Vector3 MyPos = m_Object->GetWorldPos();

			// TODO : ���߿� ��ž ��Ÿ� csv���Ϸ� �о����
			if (TestObjectPos.Distance(MyPos) < 30.f)
			{
				// ��Ÿ����� �ִ� ��(è�Ǿ�, �̴Ͼ� ����)
				if ((TestObject->IsEnemy() && m_Object->IsEnemy()) || (!TestObject->IsEnemy() && !m_Object->IsEnemy()))
					m_FriendChampionList.push_back(TestObject);
				else
					m_TargetChampionList.push_back(TestObject);
			}
		}
	}

	return FindPriorAttackTarget();
}

CGameObject* CCheckTurretAttackTarget::FindPriorAttackTarget()
{
	auto iter = m_TargetChampionList.begin();
	auto iterEnd = m_TargetChampionList.end();

	for (; iter != iterEnd; ++iter)
	{
		// �� è�Ǿ��߿� ��ž ��Ÿ� ���� �����ϸ鼭 ��Ÿ�� �Ʊ� è�Ǿ��� ������ �ִ� è�Ǿ��� �ִٸ� 
		if ((*iter)->GetNormalAttackTarget())
		{
			m_Object->SetNormalAttackTarget(*iter);
			return *iter;
		}
	}

	iter = m_FriendChampionList.begin();
	iterEnd = m_FriendChampionList.end();

	// �Ʊ� è�Ǿ��� Ư�� ���¸� ������ �ִٸ� �� ���¸� ���� �� SrcObject�� ��ž ���� ������� ����
	for (; iter != iterEnd; ++iter)
	{
		CStateComponent* StateComp = (*iter)->FindComponentFromType<CStateComponent>();

		if (!StateComp)
			continue;

		if (StateComp->IsStateListEmpty())
			continue;

		size_t StateCount = StateComp->GetStateListSize();

		for (size_t i = 0; i < StateCount; ++i)
		{
			CState* State = StateComp->GetState((int)i);
			CGameObject* SrcObj = State->GetSourceObject();
			Vector3 SrcObjPos = SrcObj->GetWorldPos();

			// TODO : ���߿� ��ž ��Ÿ� csv���Ϸ� �о����
			if (SrcObjPos.Distance(m_Object->GetWorldPos()) < 30.f)
				return SrcObj;
		}
	}

	return nullptr;
}
