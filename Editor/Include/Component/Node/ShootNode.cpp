
#include "ShootNode.h"
#include "RotateAttackDirectionNode.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/BehaviorTree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../PlayerDataComponent.h"
#include "../PlayerHookComponent.h"
#include "../../Object/PlayerHook.h"
#include "Component/Node/CompositeNode.h"

CShootNode::CShootNode()	:
	m_InRestoreCam(false)
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
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string ChampionName = m_Object->GetName();

	std::string SequenceName = ChampionName + "_" + "Hook";

	if (m_AnimationMeshComp)
	{
		CAnimationSequenceInstance* Instance = m_AnimationMeshComp->GetAnimationInstance();

		// AnimationSequenceInstance���� m_ChangeTime�� �ֱ� ������, �ٷ� �ִϸ��̼��� �ٲ�°� �ƴ϶� m_ChangeTime�� ���ļ� �ִϸ��̼��� �ٲ�µ�
		// CurrentAnimation�� ShootNode�̶� ChangeAnimation�ص� �ƹ��͵� ���ϰ� �ٷ� return�ǰ� ChangeAnimation�� Idle���� �������� �����Ǿ� �ִ� ���¿���
		// �� ���¿��� OnUpdate�� ���� Idle�� ������ Sequence�� �ٲ� ���¿��� ShootNode::OnUpdate���� CurrentAnimation�� ShootNode�̶�� ����������
		// CurrentAnimation�� �ᱹ Idle�� �� ���̰�, ChangeAnimation�� nullptr�� �� ���̹Ƿ� OnUpdate�� else���� ��� ������ �Ǵ� ������ ����Ƿ�
		// �̷� ��Ȳ������ KeepAnimation�� ȣ���ؼ� ChangeAnimation�� ����� CurrentAnimation�� Attack���� ���������ش�
		if (Instance->GetChangeAnimation() && Instance->GetCurrentAnimation()->GetName() == SequenceName)
			Instance->KeepCurrentAnimation();

		else
			Instance->ChangeAnimation(SequenceName);
	}

	m_Object->SetNoInterrupt(false);
	m_CallStart = true;

	return NodeResult::Node_True;
}

NodeResult CShootNode::OnUpdate(float DeltaTime)
{
	CPlayerDataComponent* Comp = m_Object->FindObjectComponentFromType<CPlayerDataComponent>();

	Player_Ability Ability = Comp->GetPlayerAbility();

	if (Ability == Player_Ability::Hook)
	{
		if (m_InRestoreCam)
		{
			CScene* Scene = CSceneManager::GetInst()->GetScene();

			bool RestoreEnd = Scene->RestoreCamera(50.f, DeltaTime);

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

		CRotateAttackDirectionNode* Node = (CRotateAttackDirectionNode*)((CCompositeNode*)(m_Parent->GetParent()->GetParent()))->FindChildByType<CRotateAttackDirectionNode>();
		Vector3 Point = Node->GetPickingPoint();

		Vector3 ObjectPos = m_Object->GetWorldPos();
		Vector3 Dir = Vector3(Point.x, 0.f, Point.z) - Vector3(ObjectPos.x, 0.f, ObjectPos.z);
		Dir.Normalize();

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
