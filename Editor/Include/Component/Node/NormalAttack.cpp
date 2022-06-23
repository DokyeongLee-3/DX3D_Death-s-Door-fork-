
#include "NormalAttack.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/BehaviorTree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CNormalAttack::CNormalAttack()
{
	SetTypeID(typeid(CNormalAttack).hash_code());
}

CNormalAttack::CNormalAttack(const CNormalAttack& Node)	:
	CActionNode(Node)
{
}

CNormalAttack::~CNormalAttack()
{
}

NodeResult CNormalAttack::OnStart(float DeltaTime)
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string ChampionName = m_Object->GetName();

	std::string SequenceName = ChampionName + "_" + "Attack1";

	if (m_AnimationMeshComp)
	{
		CAnimationSequenceInstance* Instance = m_AnimationMeshComp->GetAnimationInstance();

		// AnimationSequenceInstance���� m_ChangeTime�� �ֱ� ������, �ٷ� �ִϸ��̼��� �ٲ�°� �ƴ϶� m_ChangeTime�� ���ļ� �ִϸ��̼��� �ٲ�µ�
		// CurrentAnimation�� NormalAttack�̶� ChangeAnimation�ص� �ƹ��͵� ���ϰ� �ٷ� return�ǰ� ChangeAnimation�� Idle���� �������� �����Ǿ� �ִ� ���¿���
		// �� ���¿��� OnUpdate�� ���� Idle�� ������ Sequence�� �ٲ� ���¿��� NormalAttack::OnUpdate���� CurrentAnimation�� NormalAttack�̶�� ����������
		// CurrentAnimation�� �ᱹ Idle�� �� ���̰�, ChangeAnimation�� nullptr�� �� ���̹Ƿ� OnUpdate�� else���� ��� ������ �Ǵ� ������ ����Ƿ�
		// �̷� ��Ȳ������ KeepAnimation�� ȣ���ؼ� ChangeAnimation�� ����� CurrentAnimation�� Attack���� ���������ش�
		if (Instance->GetChangeAnimation() && Instance->GetCurrentAnimation()->GetName() == SequenceName)
			Instance->KeepCurrentAnimation();

		else
			Instance->ChangeAnimation(SequenceName);
	}

	m_Object->SetNoInterrupt(false);
	m_CallStart = true;
	
	// Nav Agent�� PathList�� ����
	CNavAgent* Agent = m_Object->GetNavAgent();

	if (Agent)
		Agent->ClearPathList();

	return NodeResult::Node_True;
}

NodeResult CNormalAttack::OnUpdate(float DeltaTime)
{
	if (!m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimLoop() && m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimEnd())
	{
		m_Object->SetNoInterrupt(false);

		m_IsEnd = true;
		m_CallStart = false;
		m_Owner->SetCurrentNode(nullptr);
		return NodeResult::Node_False;
	}

	else
	{
		m_Owner->SetCurrentNode(this);

		return NodeResult::Node_Running;
	}
}

NodeResult CNormalAttack::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);
	m_IsEnd = false;

	return NodeResult::Node_True;
}

NodeResult CNormalAttack::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}
