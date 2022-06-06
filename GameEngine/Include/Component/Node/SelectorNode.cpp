
#include "SelectorNode.h"
#include "../BehaviorTree.h"

CSelectorNode::CSelectorNode()
{
	SetTypeID(typeid(CSelectorNode).hash_code());
	m_NodeType = Node_Type::Selector;
}

CSelectorNode::CSelectorNode(const CSelectorNode& Node)	:
	CCompositeNode(Node)
{
}

CSelectorNode::~CSelectorNode()
{
}

bool CSelectorNode::Invoke(float DeltaTime)
{
	auto iter = GetChildrenList().begin();
	auto iterEnd = GetChildrenList().end();

	for (; iter != iterEnd; ++iter)
	{
		// Selector�� �ϳ��� �ڽ��̶� true�� �����ϸ� �ٷ� �ڽŵ� true�� ����
		if ((*iter)->Invoke(DeltaTime))
			return true;
	}

	SetChildrenCallStart(false);
	return false;
}

NodeResult CSelectorNode::OnStart(float DeltaTime)
{
	return NodeResult();
}

NodeResult CSelectorNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CSelectorNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
