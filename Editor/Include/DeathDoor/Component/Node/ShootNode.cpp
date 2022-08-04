
#include "ShootNode.h"
#include "RotateAttackDirectionNode.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/BehaviorTree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../PlayerDataComponent.h"
#include "../PlayerHookComponent.h"
#include "../PlayerBowComponent.h"
#include "../PlayerBombComponent.h"
#include "../../../Object/PlayerHook.h"
#include "Component/Node/CompositeNode.h"

CShootNode::CShootNode()	:
	m_InRestoreCam(false),
	m_CameraMoveSpeed(50.f),
	m_CameraMoveTime(0.f)
{
	SetTypeID(typeid(CShootNode).hash_code());
}

CShootNode::CShootNode(const CShootNode& Node) :
	CActionNode(Node)
{
}

CShootNode::~CShootNode()
{
}

NodeResult CShootNode::OnStart(float DeltaTime)
{
	m_CurrentCamPos = m_Object->GetScene()->GetCameraManager()->GetCurrentCamera()->GetWorldPos();

	// Shoot�������� ī�޶� �����̱� ���� ReadyToShoot ��忡�� �����س��� ���� ī�޶� ��ġ
	Vector3 CamOriginPos = m_Object->GetScene()->GetOriginCamPos();
	float Dist = m_CurrentCamPos.Distance(CamOriginPos);

	m_CameraMoveTime = Dist / m_CameraMoveSpeed;

	m_CallStart = true;

	return NodeResult::Node_True;
}

NodeResult CShootNode::OnUpdate(float DeltaTime)
{
	CPlayerDataComponent* Comp = m_Object->FindObjectComponentFromType<CPlayerDataComponent>();

	Player_Ability Ability = Comp->GetPlayerAbility();

	CRotateAttackDirectionNode* Node = (CRotateAttackDirectionNode*)((CCompositeNode*)(m_Parent->GetParent()->GetParent()))->FindChildByType<CRotateAttackDirectionNode>();
	Vector3 Point = Node->GetPickingPoint();

	Vector3 ObjectPos = m_Object->GetWorldPos();
	Vector3 Dir = Vector3(Point.x, 0.f, Point.z) - Vector3(ObjectPos.x, 0.f, ObjectPos.z);
	Dir.Normalize();

	if (Ability == Player_Ability::Hook)
	{
		if (m_InRestoreCam)
		{
			CScene* Scene = CSceneManager::GetInst()->GetScene();
			bool RestoreEnd = Scene->RestoreCamera(m_CameraMoveTime, m_CurrentCamPos, DeltaTime);

			if (RestoreEnd)
			{
				m_IsEnd = true;
				m_CallStart = false;
				m_Owner->SetCurrentNode(nullptr);
				m_InRestoreCam = false;

				return NodeResult::Node_True;
			}

			else
			{
				return NodeResult::Node_Running;
			}
		}

		CPlayerHookComponent* Comp = m_Object->FindComponentFromType<CPlayerHookComponent>();
		bool HookCollision = false;
		if (Comp)
		{
			HookResult Result = Comp->ShootHook(Dir, DeltaTime);

			if (Result == HookResult::NoCollision)
			{
				m_InRestoreCam = true;
				return NodeResult::Node_Running;
			}

			else if (Result == HookResult::Collision)
			{
				m_Owner->SetCurrentNode(this);
				return NodeResult::Node_Running;
				// �浹�� �������� �÷��̾ ���ư��� �ִ���
			}

			else if(Result == HookResult::OnShoot)
			{
				m_Owner->SetCurrentNode(this);
				return NodeResult::Node_Running;
			}

			else if (Result == HookResult::CollisionEnd)
			{
				m_Owner->SetCurrentNode(nullptr);
				return NodeResult::Node_True;
			}

		}

		else
		{
			// CancleShootNode�� CurrentNode�� ���� ī�޶� ���� ��ġ�� ����
			m_Owner->SetCurrentNode(((CCompositeNode*)m_Parent->GetParent())->GetChild(1));
		}

		m_CallStart = true;

		return NodeResult::Node_True;
	}

	else if (Ability == Player_Ability::Arrow)
	{
		if (m_InRestoreCam)
		{
			CScene* Scene = CSceneManager::GetInst()->GetScene();
			bool RestoreEnd = Scene->RestoreCamera(m_CameraMoveTime, m_CurrentCamPos, DeltaTime);

			if (RestoreEnd)
			{
				m_IsEnd = true;
				m_CallStart = false;
				m_Owner->SetCurrentNode(nullptr);
				m_InRestoreCam = false;

				return NodeResult::Node_True;
			}

			else
			{
				return NodeResult::Node_Running;
			}
		}

		else
		{
			CPlayerBowComponent* BowComp = m_Object->FindComponentFromType<CPlayerBowComponent>();

			if (BowComp)
			{
				BowComp->ShootArrow(Dir);

				m_Owner->SetCurrentNode(this);

				BowComp->HideBow();
			}

			m_InRestoreCam = true;
		}

		m_CallStart = true;

		return NodeResult::Node_True;
	}

	else if (Ability == Player_Ability::Bomb)
	{
		if (m_InRestoreCam)
		{
			CScene* Scene = CSceneManager::GetInst()->GetScene();
			bool RestoreEnd = Scene->RestoreCamera(m_CameraMoveTime, m_CurrentCamPos, DeltaTime);

			if (RestoreEnd)
			{
				m_IsEnd = true;
				m_CallStart = false;
				m_Owner->SetCurrentNode(nullptr);
				m_InRestoreCam = false;

				return NodeResult::Node_True;
			}

			else
			{
				return NodeResult::Node_Running;
			}

		}

		else
		{
			CPlayerBombComponent* BombComp = m_Object->FindObjectComponentFromType<CPlayerBombComponent>();

			// ReadyToShoot ��忡�� Bomb Lift�� ���� �ȳ����µ� RButton�� ���� BombComponent�� Bomb Object�� reset ��Ű�� 
			// BombComponent�� m_IsClearBomb�� true�� ������ش�
			if (BombComp->IsClearBomb())
			{
				BombComp->SetClearBomb(false);

			}

			else if(!BombComp->IsBeforeLift())
			{
				BombComp->ShootBomb(Dir);
			}

			m_InRestoreCam = true;

			if (BombComp->IsBeforeLift())
			{
				m_InRestoreCam = false;
			}

			m_Owner->SetCurrentNode(this);

			m_IsEnd = false;
		}

		m_CallStart = true;

		return NodeResult::Node_True;
	}
}

NodeResult CShootNode::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);
	m_IsEnd = false;

	return NodeResult::Node_True;
}

NodeResult CShootNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}
