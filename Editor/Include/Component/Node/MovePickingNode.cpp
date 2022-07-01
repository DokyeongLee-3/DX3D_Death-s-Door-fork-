
#include "MovePickingNode.h"
#include "Input.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Navigation3DManager.h"
#include "Component/BehaviorTree.h"
#include "Component/AnimationMeshComponent.h"
#include "Collision/Collision.h"
#include "Component/ColliderComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"


CMovePickingNode::CMovePickingNode()
{
	SetTypeID(typeid(CMovePickingNode).hash_code());
}

CMovePickingNode::CMovePickingNode(const CMovePickingNode& Node)	:
	CActionNode(Node)
{
}

CMovePickingNode::~CMovePickingNode()
{
}

NodeResult CMovePickingNode::OnStart(float DeltaTime)
{

	CNavigation3DManager* Manager = m_Object->GetScene()->GetNavigation3DManager();

	Vector3 TargetPos;
	if (Manager->CheckPickingPoint(TargetPos))
	{
		std::vector<CColliderComponent*> vecTestCollider;
		std::vector<int>		vecTestIndex;
		CSceneCollision* Collision = CSceneManager::GetInst()->GetScene()->GetCollision();

		int CountX = Collision->GetSectionCountX();
		int CountZ = Collision->GetSectionCountZ();

		int Idx = Collision->GetSectionIndex(TargetPos);
		int IdxX = Collision->GetSectionIndexX(TargetPos);
		int IdxZ = Collision->GetSectionIndexZ(TargetPos);

		if (IdxX == 0 && IdxZ == 0)
		{
			vecTestIndex.push_back(Idx);
		}

		else if (IdxX == 0)
		{
			vecTestIndex.push_back(Idx - CountX);
			vecTestIndex.push_back(Idx);
		}

		else if (IdxZ == 0)
		{
			vecTestIndex.push_back(Idx - 1);
			vecTestIndex.push_back(Idx);
		}

		//else if (IdxX == Collision->GetSectionCountX() - 1)
		//{
		//	vecTestIndex.push_back(Idx - 1);
		//	vecTestIndex.push_back(Idx);
		//}

		//else if (Idx == Collision->GetSectionCountZ() - 1)
		//{
		//	vecTestIndex.push_back(Idx - CountX);
		//	vecTestIndex.push_back(Idx);
		//}

		else
		{
			vecTestIndex.push_back(Idx - CountX - 1);
			vecTestIndex.push_back(Idx);
		}

		size_t Count = vecTestIndex.size();
		bool Same = false;

		for (size_t i = 0; i < Count; ++i)
		{
			std::vector<CColliderComponent*> vecCollider = Collision->GetSectionCollider(vecTestIndex[i]);
			size_t Count2 = vecCollider.size();

			for (size_t j = 0; j < Count2; ++j)
			{
				// �̹� �ִ� Collider�� �� ������ �ȵȴ�
				size_t TestCount = vecTestCollider.size();
				for (size_t k = 0; k < TestCount; ++k)
				{
					if (vecTestCollider[k] == vecCollider[j])
					{
						Same = true;
						break;
					}
				}

				if (!Same)
					vecTestCollider.push_back(vecCollider[j]);

				Same = false;
			}
		}

		Count = vecTestCollider.size();

		for (size_t i = 0; i < Count; ++i)
		{
			// ��ŷ�� �浹ü�� ���� ���� �浹ü�� �˻�X
			if (m_Object->CheckSceneComponent(vecTestCollider[i]))
				continue;

			if (vecTestCollider[i]->CheckType<CColliderBox3D>())
			{
				Vector3 HitPoint;
				CCameraComponent* CurrentCamera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
				Ray ray = CInput::GetInst()->GetRay(CurrentCamera->GetViewMatrix());
				bool Collision = CCollision::CollisionRayToBox3D(HitPoint, ray, ((CColliderBox3D*)vecTestCollider[i])->GetInfo());

				// ��ŷ�ߴµ� ������ �ƴ� Ư�� ������Ʈ�� �浹�ϴ� ���
				if (Collision)
				{
					m_Object->ClearPath();
					m_Object->AddPath(vecTestCollider[i]->GetGameObject()->GetWorldPos());
					CAnimationSequenceInstance* Instance = m_Owner->GetAnimationMeshComp()->GetAnimationInstance();

					Instance->ChangeAnimation(m_Object->GetName() + "_Run");

					// Ư�� ������Ʈ�� ��ŷ�� ��� NormalAttackSequence�� �����ϱ� ������ false�� ����
					return NodeResult::Node_False;
				}
			}

			else if (vecTestCollider[i]->CheckType<CColliderSphere>())
			{
				Vector3 HitPoint;
				CCameraComponent* CurrentCamera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
				Ray ray = CInput::GetInst()->GetRay(CurrentCamera->GetViewMatrix());
				bool Collision = CCollision::CollisionRayToSphere(HitPoint, ray, ((CColliderSphere*)vecTestCollider[i])->GetInfo());

				// ��ŷ�ߴµ� ������ �ƴ� Ư�� ������Ʈ�� �浹�ϴ� ���
				if (Collision)
				{
					m_Object->ClearPath();
					m_Object->AddPath(vecTestCollider[i]->GetGameObject()->GetWorldPos());
					CAnimationSequenceInstance* Instance = m_Owner->GetAnimationMeshComp()->GetAnimationInstance();

					Instance->ChangeAnimation(m_Object->GetName() + "_Run");

					// Ư�� ������Ʈ�� ��ŷ�� ��� NormalAttackSequence�� �����ϱ� ������ false�� ����
					return NodeResult::Node_False;
				}
			}
		}

		// Ư�� ������Ʈ�� �ƴ� ������ ��ŷ�� ���
		m_Object->ClearPath();
		m_Object->AddPath(TargetPos);
		CAnimationSequenceInstance* Instance = m_Owner->GetAnimationMeshComp()->GetAnimationInstance();

		Instance->ChangeAnimation(m_Object->GetName() + "_Run");

	}

	return NodeResult::Node_True;
}

NodeResult CMovePickingNode::OnUpdate(float DeltaTime)
{
	// ���� NavAgent�� m_PathList�� ����ٸ� �׶� return false�ؼ� IdleNode�� ������
	if (!m_Object->IsNavAgentPathListEmpty())
	{
		//m_Object->SetInterruptEnable(false);
		return NodeResult::Node_True;
	}

	else
	{
		//m_Object->SetInterruptEnable(true);
		return NodeResult::Node_False;
	}
}

NodeResult CMovePickingNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CMovePickingNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

//bool CMovePickingNode::SavePos(FILE* File)
//{
//	CBehaviorTreeWindow* Window = (CBehaviorTreeWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(BEHAVIORTREE_WINDOW);
//
//	if (!Window)
//		return false;
//
//	GraphEditorDelegate::Node WindowNode = Window->m_Delegate.FindNode(this);
//
//	fwrite(&WindowNode.x, sizeof(float), 1, File);
//	fwrite(&WindowNode.y, sizeof(float), 1, File);
//
//}
//
//bool CMovePickingNode::LoadPos(FILE* File)
//{
//	fread(&m_InWindowPos.x, sizeof(float), 1, File);
//	fread(&m_InWindowPos.y, sizeof(float), 1, File);
//
//	return true;
//}
//
