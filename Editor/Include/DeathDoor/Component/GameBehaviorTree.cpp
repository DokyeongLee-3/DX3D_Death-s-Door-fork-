
#include "GameBehaviorTree.h"
#include "Component/Node/SelectorNode.h"
#include "Component/Node/SequenceNode.h"

// Public Nodes
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
#include "Node/ClearPathListNode.h"
#include "Node/PathFindEnableCheck.h"
#include "Node/HitBackNode.h"
#include "Node/HitCheckNode.h"
#include "Node/RollInputCheck.h"
#include "Node/PlayerRoll.h"

// Public Nodes
#include "Node/MeleeRangeCheckNode.h"
#include "Node/PostAttackDelayCheck.h"
#include "Node/AttackCoolTimeCheck.h"
#include "Node/DeathCheck.h"
#include "Node/IsCombatCheck.h"
#include "Node/DeathNode.h"

// Boss - Knight
#include "Node/BossKnightContinueAttackNode.h"
#include "Node/BossKnightCutScenePlayNode.h"
#include "Node/BossKnightFinalAttackCheck.h"
#include "Node/BossKnightFinalAttackNode.h"
#include "Node/BossKnightJumpAttackRangeCheck.h"
#include "Node/BossKnightIdleNode.h"
#include "Node/BossKnightJumpAttackNode.h"
#include "Node/BossKnightMeleeAttackNode.h"
#include "Node/BossKnightSlamEnd.h"
#include "Node/BossKnightWalkNode.h"
#include "Node/BossKnightContinueAttackCheck.h"
#include "Node/BossKnightCutScenePlayCheck.h"
#include "Node/BossKnightFinalAttackCheck.h"
#include "Node/BossKnightJumpAttackRangeCheck.h"
#include "Node/BossKnightPlayerEnterZoneCheck.h"
#include "Node/UpdateInputQueue.h"

// Boss - Betty
#include "Node/BossBettyAngryAttackNode.h"
#include "Node/BossBettyChangeAttackDirNode.h"
#include "Node/BossBettyCloseAttackNode.h"
#include "Node/BossBettyJumpAttackNode.h"
#include "Node/BossBettySpinAttackNode.h"
#include "Node/BossBettyThrowNode.h"

#include "Node/BossBettyChangeDirCheck.h"
#include "Node/BossBettyFarAttackTypeCheck.h"
#include "Node/BossBettyHPStateCheck.h"
#include "Node/BossBettyThrowAttackCheck.h"

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
	CBehaviorTree::Init();
	return true;
}

void CGameBehaviorTree::Start()
{
	CBehaviorTree::Start();
}

CGameStateComponent* CGameBehaviorTree::GetOwner() const
{
	return dynamic_cast<CGameStateComponent*>(m_Owner);
}


CNode* CGameBehaviorTree::LoadNode(CNode* Parent, size_t TypeID)
{
	CNode* NewNode = nullptr;
	CGameObject* OwnerObject = m_Owner->GetGameObject();

	// TODO : Node ���� �߰��� �� ���� �߰����ֱ�
	// Public Node �̰��� �ۼ�
	if (TypeID == typeid(CSelectorNode).hash_code())
	{
		NewNode = MakeNode<CSelectorNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CSequenceNode).hash_code())
	{
		NewNode = MakeNode<CSequenceNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CNoInterruptNode).hash_code())
	{
		NewNode = MakeNode<CNoInterruptNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CIdleNode).hash_code())
	{
		NewNode = MakeNode<CIdleNode>(Parent, OwnerObject);
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
		NewNode = MakeNode<CMoveInputCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CMouseLButtonCheckNode).hash_code())
	{
		NewNode = MakeNode<CMouseLButtonCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CMouseRButtonCheckNode).hash_code())
	{
		NewNode = MakeNode<CMouseRButtonCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CMouseRButtonUpCheckNode).hash_code())
	{
		NewNode = MakeNode<CMouseRButtonUpCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CReadyToShoot).hash_code())
	{
		NewNode = MakeNode<CReadyToShoot>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CShootNode).hash_code())
	{
		NewNode = MakeNode<CShootNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CCancleShootNode).hash_code())
	{
		NewNode = MakeNode<CCancleShootNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CNormalAttack).hash_code())
	{
		NewNode = MakeNode<CNormalAttack>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CRotateAttackDirectionNode).hash_code())
	{
		NewNode = MakeNode<CRotateAttackDirectionNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CNegateNode).hash_code())
	{
		NewNode = MakeNode<CNegateNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CAddFallingFloorCallbackNode).hash_code())
	{
		NewNode = MakeNode<CAddFallingFloorCallbackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CLockstone3TriggerBoxHitCheck).hash_code())
	{
		NewNode = MakeNode<CLockstone3TriggerBoxHitCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CLockstone3TriggerBoxAction).hash_code())
	{
		NewNode = MakeNode<CLockstone3TriggerBoxAction>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CCheckDetectRangeNode).hash_code())
	{
		NewNode = MakeNode<CCheckDetectRangeNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CFindPathNode).hash_code())
	{
		NewNode = MakeNode<CFindPathNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CMeleeRangeCheckNode).hash_code())
	{
		NewNode = MakeNode<CMeleeRangeCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CPostAttackDelayCheck).hash_code())
	{
		NewNode = MakeNode<CPostAttackDelayCheck>(Parent, OwnerObject);
	}
	else if (TypeID == typeid(CAttackCoolTimeCheck).hash_code())
	{
		NewNode = MakeNode<CAttackCoolTimeCheck>(Parent, OwnerObject);
	}
	else if (TypeID == typeid(CIsCombatCheck).hash_code())
	{
		NewNode = MakeNode<CIsCombatCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CDeathCheck).hash_code())
	{
		NewNode = MakeNode<CDeathCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CDeathNode).hash_code())
	{
		NewNode = MakeNode<CDeathNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CClearPathListNode).hash_code())
	{
		NewNode = MakeNode<CClearPathListNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CPathFindEnableCheck).hash_code())
	{
		NewNode = MakeNode<CPathFindEnableCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CHitCheckNode).hash_code())
	{
		NewNode = MakeNode<CHitCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CHitBackNode).hash_code())
	{
		NewNode = MakeNode<CHitBackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CRollInputCheck).hash_code())
	{
		NewNode = MakeNode<CRollInputCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CUpdateInputQueue).hash_code())
	{
		NewNode = MakeNode<CUpdateInputQueue>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CPlayerRoll).hash_code())
	{
		NewNode = MakeNode<CPlayerRoll>(Parent, OwnerObject);
	}

	// Boss Knight - Action
	else if (TypeID == typeid(CBossKnightContinueAttackNode).hash_code())
	{
		NewNode = MakeNode<CBossKnightContinueAttackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightCutScenePlayNode).hash_code())
	{
		NewNode = MakeNode<CBossKnightCutScenePlayNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightFinalAttackNode).hash_code())
	{
		NewNode = MakeNode<CBossKnightFinalAttackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightIdleNode).hash_code())
	{
		NewNode = MakeNode<CBossKnightIdleNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightJumpAttackNode).hash_code())
	{
		NewNode = MakeNode<CBossKnightJumpAttackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightMeleeAttackNode).hash_code())
	{
		NewNode = MakeNode<CBossKnightMeleeAttackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightSlamEnd).hash_code())
	{
		NewNode = MakeNode<CBossKnightSlamEnd>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightWalkNode).hash_code())
	{
		NewNode = MakeNode<CBossKnightWalkNode>(Parent, OwnerObject);
	}

	// Boss Knight - Condition
	else if (TypeID == typeid(CBossKnightContinueAttackCheck).hash_code())
	{
		NewNode = MakeNode<CBossKnightContinueAttackCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightCutScenePlayCheck).hash_code())
	{
		NewNode = MakeNode<CBossKnightCutScenePlayCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightFinalAttackCheck).hash_code())
	{
		NewNode = MakeNode<CBossKnightFinalAttackCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightJumpAttackRangeCheck).hash_code())
	{
		NewNode = MakeNode<CBossKnightJumpAttackRangeCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightPlayerEnterZoneCheck).hash_code())
	{
		NewNode = MakeNode<CBossKnightPlayerEnterZoneCheck>(Parent, OwnerObject);
	}
	// Boss Betty Action
	else if (TypeID == typeid(CBossBettyAngryAttackNode).hash_code())
	{
	NewNode = MakeNode<CBossBettyAngryAttackNode>(Parent, OwnerObject);
	}
	else if (TypeID == typeid(CBossBettyChangeAttackDirNode).hash_code())
	{
	NewNode = MakeNode<CBossBettyChangeAttackDirNode>(Parent, OwnerObject);
	}
	else if (TypeID == typeid(CBossBettyCloseAttackNode).hash_code())
	{
	NewNode = MakeNode<CBossBettyCloseAttackNode>(Parent, OwnerObject);
	}
	else if (TypeID == typeid(CBossBettyJumpAttackNode).hash_code())
	{
	NewNode = MakeNode<CBossBettyJumpAttackNode>(Parent, OwnerObject);
	}
	else if (TypeID == typeid(CBossBettySpinAttackNode).hash_code())
	{
	NewNode = MakeNode<CBossBettySpinAttackNode>(Parent, OwnerObject);
	}
	else if (TypeID == typeid(CBossBettyThrowNode).hash_code())
	{
	NewNode = MakeNode<CBossBettyThrowNode>(Parent, OwnerObject);
	}
	// Boss Betty Condition
	else if (TypeID == typeid(CBossBettyChangeDirCheck).hash_code())
	{
	NewNode = MakeNode<CBossBettyChangeDirCheck>(Parent, OwnerObject);
	}
	else if (TypeID == typeid(CBossBettyFarAttackTypeCheck).hash_code())
	{
	NewNode = MakeNode<CBossBettyFarAttackTypeCheck>(Parent, OwnerObject);
	}
	else if (TypeID == typeid(CBossBettyHPStateCheck).hash_code())
	{
	NewNode = MakeNode<CBossBettyHPStateCheck>(Parent, OwnerObject);
	}
	else if (TypeID == typeid(CBossBettyThrowAttackCheck).hash_code())
	{
	NewNode = MakeNode<CBossBettyThrowAttackCheck>(Parent, OwnerObject);
	}

	return NewNode;
}
