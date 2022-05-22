
#include "RunNode.h"
#include "../AnimationMeshComponent.h"
#include "../../Input.h"
#include "../BehaviorTree.h"

CRunNode::CRunNode()
{
	SetTypeID(typeid(CRunNode).hash_code());
}

CRunNode::CRunNode(const CRunNode& Node)	:
	CActionNode(Node)
{
}

CRunNode::~CRunNode()
{
}


NodeResult CRunNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string ChampionName = m_Object->GetName();

	std::string SequenceName = ChampionName + "_" + "Run";

	if (m_AnimationMeshComp)
	{
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
		m_CallStart = true;
	}

	return NodeResult::Node_True;
}

NodeResult CRunNode::OnUpdate(float DeltaTime)
{
	// ���� �̵�(Test�� ���� ���ǵ带 �����, ���⵵ �������� �ϴ� ����)
	m_Object->AddWorldPos(0.f, 0.f, 10.f * DeltaTime);

	return NodeResult::Node_True;
}

NodeResult CRunNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

bool CRunNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}
