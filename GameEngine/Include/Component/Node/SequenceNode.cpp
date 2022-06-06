
#include "SequenceNode.h"
#include "../BehaviorTree.h"

CSequenceNode::CSequenceNode()
{
	SetTypeID(typeid(CSequenceNode).hash_code());
	m_NodeType = Node_Type::Sequence;
}

CSequenceNode::CSequenceNode(const CSequenceNode& Node)	:
	CCompositeNode(Node)
{
}

CSequenceNode::~CSequenceNode()
{
}

bool CSequenceNode::Invoke(float DeltaTime)
{
	auto iter = GetChildrenList().begin();
	auto iterEnd = GetChildrenList().end();

	for (; iter != iterEnd; ++iter)
	{
		// Sequence�� �ϳ��� �ڽ��̶� false�� �����ϸ� �ڽŵ� false�� ����
		if (!(*iter)->Invoke(DeltaTime))
		{
			// �� �ڽ��� �ϳ��� false��� ���� �̹� �����ӿ� �� �ڽĵ��� Action�� �������� �ʰڴٴ� �ǹ��̹Ƿ�
			// ���� �����ӿ� �ٽ� ���� �ڽ��� ��ȸ�ؼ� ��� true�� ��ȯ�ؼ� �ڽĵ��� Action�� �����Ҷ���
			// OnStart���� ȣ���ϰ� �ؾ��ϹǷ� ��� m_CallStart�� false�� �����
			SetChildrenCallStart(false);
			return false;
		}
	}

	return true;
}

NodeResult CSequenceNode::OnStart(float DeltaTime)
{
	return NodeResult();
}

NodeResult CSequenceNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CSequenceNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

