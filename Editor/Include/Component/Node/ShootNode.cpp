
#include "ShootNode.h"
#include "RotateAttackDirectionNode.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/BehaviorTree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../PlayerDataComponent.h"
#include "../../Object/PlayerHook.h"

CShootNode::CShootNode()
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

	std::string SequenceName = ChampionName + "_" + "SkillR";

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
		CPlayerHook* HookHead = Comp->GetPlayerHookHead();

		CRotateAttackDirectionNode* Node = (CRotateAttackDirectionNode*)m_Owner->FindNodeByType<CRotateAttackDirectionNode>();
		Vector3 Point = Node->GetPickingPoint();
		Vector3 ObjectPos = m_Object->GetWorldPos();
		Vector3 Dir = Vector3(Point.x, 0.f, Point.z) - Vector3(ObjectPos.x, 0.f, ObjectPos.z);

		bool Collision = false;
		bool ThrowHookEnd = HookHead->Move(Dir, 1.f, Collision);

		// Hook�� �ǹ��� �浹�ؼ� �� �������� ���������Ѵٸ� return NodeResult::Node_False�� �����ؼ� �÷��̾ �������� ������ ���ִ� ���� ���� ������
	}

	if (!m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimLoop() && m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimEnd())
	{
		CScene* Scene = CSceneManager::GetInst()->GetScene();

		bool RestoreEnd = Scene->RestoreCamera(50.f, DeltaTime);

		if (RestoreEnd)
		{
			m_Object->SetNoInterrupt(false);

			m_IsEnd = true;
			m_CallStart = false;
			m_Owner->SetCurrentNode(nullptr);

			return NodeResult::Node_True;
		}

		else
		{
			return NodeResult::Node_Running;
		}
	}

	else
	{
		m_Owner->SetCurrentNode(this);

		return NodeResult::Node_Running;
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
