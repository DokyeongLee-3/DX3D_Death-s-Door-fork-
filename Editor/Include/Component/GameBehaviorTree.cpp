
#include "GameBehaviorTree.h"
#include "Component/Node/SelectorNode.h"
#include "Component/Node/SequenceNode.h"
#include "Node/MoveNode.h"
#include "Node/IdleNode.h"
#include "Node/MoveInputCheckNode.h"
#include "Node/NoInterruptNode.h"
#include "Node/NormalAttack.h"
#include "Node/MouseLButtonCheckNode.h"
#include "Node/MouseRButtonCheckNode.h"
#include "Node/MouseRButtonUpCheckNode.h"
#include "Node/NegateNode.h"
#include "Node/RotateAttackDirectionNode.h"
#include "Node/ReadyToShoot.h"
#include "Node/ShootNode.h"
#include "Node/CancleShootNode.h"
#include "Node/AddFallingFloorCallbackNode.h"
#include "Node/Lockstone3TriggerBoxHitCheck.h"
#include "Node/CheckDetectRangeNode.h"
#include "Node/FindPathNode.h"
#include "Node/Lockstone3TriggerBoxAction.h"
#include "Node/MeleeRangeCheckNode.h"
#include "Node/ClearPathListNode.h"
#include "Node/PathFindEnableCheck.h"
#include "GameStateComponent.h"


CGameBehaviorTree::CGameBehaviorTree()
{
}

CGameBehaviorTree::CGameBehaviorTree(const CGameBehaviorTree& Tree) :
	CBehaviorTree(Tree)
{
}

CGameBehaviorTree::~CGameBehaviorTree()
{
}

bool CGameBehaviorTree::Init()
{
	return false;
}

void CGameBehaviorTree::Start()
{
}

CGameStateComponent* CGameBehaviorTree::GetOwner() const
{
	return dynamic_cast<CGameStateComponent*>(m_Owner);
}


CNode* CGameBehaviorTree::LoadNode(CNode* Parent, size_t TypeID)
{
	// TODO : Node ���� �߰��� �� ���� �߰����ֱ�
	if (TypeID == typeid(CSelectorNode).hash_code())
	{
		CSelectorNode* NewNode = new CSelectorNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CSequenceNode).hash_code())
	{
		CSequenceNode* NewNode = new CSequenceNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CNoInterruptNode).hash_code())
	{
		CNoInterruptNode* NewNode = new CNoInterruptNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CIdleNode).hash_code())
	{
		CIdleNode* NewNode = new CIdleNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CMoveNode).hash_code())
	{
		CMoveNode* NewNode = new CMoveNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		CNavAgent* Agent = NewNode->GetGameObject()->FindObjectComponentFromType<CNavAgent>();

		if (Agent)
			NewNode->SetNavAgent(Agent);

		return NewNode;
	}

	else if (TypeID == typeid(CMoveInputCheckNode).hash_code())
	{
		CMoveInputCheckNode* NewNode = new CMoveInputCheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CMouseLButtonCheckNode).hash_code())
	{
		CMouseLButtonCheckNode* NewNode = new CMouseLButtonCheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CMouseRButtonCheckNode).hash_code())
	{
		CMouseRButtonCheckNode* NewNode = new CMouseRButtonCheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CMouseRButtonUpCheckNode).hash_code())
	{
		CMouseRButtonUpCheckNode* NewNode = new CMouseRButtonUpCheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CReadyToShoot).hash_code())
	{
		CReadyToShoot* NewNode = new CReadyToShoot;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CShootNode).hash_code())
	{
		CShootNode* NewNode = new CShootNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CCancleShootNode).hash_code())
	{
		CCancleShootNode* NewNode = new CCancleShootNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CNormalAttack).hash_code())
	{
		CNormalAttack* NewNode = new CNormalAttack;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CRotateAttackDirectionNode).hash_code())
	{
		CRotateAttackDirectionNode* NewNode = new CRotateAttackDirectionNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CNegateNode).hash_code())
	{
		CNegateNode* NewNode = new CNegateNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CAddFallingFloorCallbackNode).hash_code())
	{
		CAddFallingFloorCallbackNode* NewNode = new CAddFallingFloorCallbackNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CLockstone3TriggerBoxHitCheck).hash_code())
	{
		CLockstone3TriggerBoxHitCheck* NewNode = new CLockstone3TriggerBoxHitCheck;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CLockstone3TriggerBoxAction).hash_code())
	{
		CLockstone3TriggerBoxAction* NewNode = new CLockstone3TriggerBoxAction;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CCheckDetectRangeNode).hash_code())
	{
		CCheckDetectRangeNode* NewNode = new CCheckDetectRangeNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CFindPathNode).hash_code())
	{
		CFindPathNode* NewNode = new CFindPathNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CMeleeRangeCheckNode).hash_code())
	{
		CMeleeRangeCheckNode* NewNode = new CMeleeRangeCheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CClearPathListNode).hash_code())
	{
		CClearPathListNode* NewNode = new CClearPathListNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());

		return NewNode;
	}

	else if (TypeID == typeid(CPathFindEnableCheck).hash_code())
	{
		CPathFindEnableCheck* NewNode = new CPathFindEnableCheck;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());

		return NewNode;
	}

	return nullptr;
}
