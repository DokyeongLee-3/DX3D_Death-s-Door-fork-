

#include "BehaviorTree.h"
#include "StateComponent.h"
#include "Node/Node.h"
#include "Node/ActionNode.h"
#include "Node/ConditionNode.h"
#include "Node/SequenceNode.h"
#include "Node/SelectorNode.h"
#include "Node/RunNode.h"
#include "Node/IdleNode.h"
#include "Node/MoveInputCheckNode.h"
#include "Node/MovePickingNode.h"
#include "Node/SkillQNode.h"
#include "Node/SkillWNode.h"
#include "Node/SkillENode.h"
#include "Node/SkillRNode.h"
#include "Node/InputQCheckNode.h"
#include "Node/InputWCheckNode.h"
#include "Node/InputECheckNode.h"
#include "Node/InputRCheckNode.h"
#include "Node/NoInterruptNode.h"

CBehaviorTree::CBehaviorTree() :
	m_Root(nullptr),
	m_CurrentNode(nullptr)
{
	
}

CBehaviorTree::CBehaviorTree(const CBehaviorTree& Tree)
{
}

CBehaviorTree::~CBehaviorTree()
{
	auto iter = m_NodeList.begin();
	auto iterEnd = m_NodeList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}
}

CNode* CBehaviorTree::GetCurrentNode() const
{
	return m_CurrentNode;
}

void CBehaviorTree::SetCurrentNode(CNode* CurrentNode)
{
	m_CurrentNode = CurrentNode;
}

void CBehaviorTree::SetAnimationMeshComponent(CAnimationMeshComponent* Mesh)
{
	m_AnimationMeshComp = Mesh;
}

CAnimationMeshComponent* CBehaviorTree::GetAnimationMeshComp() const
{
	return m_AnimationMeshComp;
}

CNode* CBehaviorTree::GetRootNode() const
{
	return m_Root;
}

void CBehaviorTree::SetRoot(class CNode* Node)
{
	m_Root = Node;
}

CNode* CBehaviorTree::FindNode(const std::string& Name)
{
	auto iter = m_NodeList.begin();
	auto iterEnd = m_NodeList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return (*iter);
	}

	return nullptr;
}

void CBehaviorTree::AddNode(CNode* Node)
{
	m_NodeList.push_back(Node);
}

void CBehaviorTree::DeleteNode(CNode* Node)
{
	auto iter = m_NodeList.begin();
	auto iterEnd = m_NodeList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == Node)
		{
			CCompositeNode* Parent = (CCompositeNode*)((*iter)->GetParent());

			if(Parent)
				Parent->DeleteChild((*iter));

			SAFE_DELETE(*iter);
			m_NodeList.erase(iter);

			break;
		}
	}
}

void CBehaviorTree::Start()
{

}

bool CBehaviorTree::Init()
{
	return true;
}

void CBehaviorTree::Update(float DeltaTime)
{

}

void CBehaviorTree::PostUpdate(float DeltaTime)
{
	if (m_CurrentNode)
		m_CurrentNode->Invoke(DeltaTime);

	else if (m_Root)
	{
		m_Root->Invoke(DeltaTime);
	}
}

void CBehaviorTree::PrevRender()
{

}

void CBehaviorTree::Render()
{
}

void CBehaviorTree::PostRender()
{
}

bool CBehaviorTree::Save(FILE* File)
{
	bool Root = false;

	if (m_Root)
		Root = true;

	fwrite(&Root, sizeof(bool), 1, File);

	if (m_Root)
	{
		size_t	TypeID = m_Root->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, File);

		m_Root->Save(File);
	}

	return true;
}

bool CBehaviorTree::Load(FILE* File)
{
	bool Root = false;
	fread(&Root, sizeof(bool), 1, File);

	if (Root)
	{
		size_t	TypeID = 0;
		fread(&TypeID, sizeof(size_t), 1, File);

		if (!m_Root)
		{
			if (TypeID == typeid(CSequenceNode).hash_code())
			{
				m_Root = new CSequenceNode;
			}

			else if (TypeID == typeid(CSelectorNode).hash_code())
			{
				m_Root = new CSelectorNode;
			}

			m_Root->m_Owner = this;
			m_Root->m_Parent = nullptr;
			m_Root->m_Object = m_Owner->GetGameObject();
		}
		
		m_NodeList.push_back(m_Root);
		m_Root->Load(File);
		
	}

	return true;
}

CBehaviorTree* CBehaviorTree::Clone()
{
	return new CBehaviorTree(*this);
}

CNode* CBehaviorTree::LoadNode(CNode* Parent, size_t TypeID)
{
	//if (m_NodeCreateCallback)
	//	return m_NodeCreateCallback(Parent, TypeID);

	if (TypeID == typeid(CSelectorNode).hash_code())
	{
		CSelectorNode* NewNode = new CSelectorNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());
		
		return NewNode;
	}

	else if (TypeID == typeid(CSequenceNode).hash_code())
	{
		CSequenceNode* NewNode = new CSequenceNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CNoInterruptNode).hash_code())
	{
		CNoInterruptNode* NewNode = new CNoInterruptNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CIdleNode).hash_code())
	{
		CIdleNode* NewNode = new CIdleNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CMovePickingNode).hash_code())
	{
		CMovePickingNode* NewNode = new CMovePickingNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CMoveInputCheckNode).hash_code())
	{
		CMoveInputCheckNode* NewNode = new CMoveInputCheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CSkillQNode).hash_code())
	{
		CSkillQNode* NewNode = new CSkillQNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CSkillWNode).hash_code())
	{
		CSkillWNode* NewNode = new CSkillWNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CSkillENode).hash_code())
	{
		CSkillENode* NewNode = new CSkillENode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CSkillRNode).hash_code())
	{
		CSkillRNode* NewNode = new CSkillRNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CInputQCheckNode).hash_code())
	{
		CInputQCheckNode* NewNode = new CInputQCheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CInputWCheckNode).hash_code())
	{
		CInputWCheckNode* NewNode = new CInputWCheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CInputECheckNode).hash_code())
	{
		CInputECheckNode* NewNode = new CInputECheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CInputRCheckNode).hash_code())
	{
		CInputRCheckNode* NewNode = new CInputRCheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(Parent->GetOwner());
		NewNode->SetObject(Parent->GetGameObject());

		return NewNode;
	}


	return nullptr;
}
