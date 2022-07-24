#include "DDUtil.h"

// TODO : DeathDoor Component �߰� �ø��� ������Ʈ
#include "Component\EyeLaserComponent.h"
#include "Component\KnightDataComponent.h"
#include "Component\BossBettyDataComponent.h"
#include "Component\MonsterDataComponent.h"
#include "Component\MonsterPathFindCollider.h"
#include "Component\ObjectDataComponent.h"
#include "Component\GameStateComponent.h"
#include "Component\PlayerDataComponent.h"
#include "Component\PlayerHookComponent.h"
#include "Component\PlayerNormalAttackCheckCollider.h"
#include "Component\LurkerDataComponent.h"
#include "Component\MonsterNavAgent.h"
#include "Component\PlayerBowComponent.h"
#include "Component\ProjectileComponent.h"
#include "Component\ArrowComponent.h"
#include "Component\CrowBossDataComponent.h"

std::string CDDUtil::DDConditionNodeTypeToString(DDConditionNode NodeType)
{
	switch (NodeType)
	{
	case DDConditionNode::MoveInputCheckNode:
		return "MoveInputCheckNode";

	case DDConditionNode::MouseLButtonCheckNode:
		return "MouseLButtonCheckNode";

	case DDConditionNode::NoInterruptNode:
		return "NoInterruptNode";

	case DDConditionNode::AttackTargetCheck:
		return "AttackTargetCheck";

	case DDConditionNode::MouseRButtonCheckNode:
		return "MouseRButtonCheckNode";

	case DDConditionNode::MouseRButtonUpCheckNode:
		return "MouseRButtonUpCheckNode";

	case DDConditionNode::Lockstone3TriggerBoxHitCheck:
		return "Lockstone3TriggerBoxHitCheck";

	case DDConditionNode::CheckDetectRange:
		return "CheckDetectRange";

	case DDConditionNode::MeleeAttackRangeCheck:
		return "MeleeAttackRangeCheck";

	case DDConditionNode::PostAttackDelayCheck:
		return "PostAttackDelayCheck";

	case DDConditionNode::BossKnightFinalAttackCheck:
		return "BossKnightFinalAttackCheck";

	case DDConditionNode::BossKnightJumpAttackRangeCheck:
		return "BossKnightJumpAttackRangeCheck";

	case DDConditionNode::BossKnightCutScenePlayCheck:
		return "BossKnightCutScenePlayCheck";

	case DDConditionNode::BossKnightContinueAttackCheck:
		return "BossKnightContinueAttackCheck";

	case DDConditionNode::BossKnightPlayerEnterZoneCheck:
		return "BossKnightPlayerEnterZoneCheck";

	// Boss Betty
	case DDConditionNode::BossBettyCheckAttackDir:
		return "BossBettyCheckAttackDir";

	case DDConditionNode::BossBettyCheckFarAttackType:
		return "BossBettyCheckFarAttackType";

	case DDConditionNode::BossBettyCheckHPState:
		return "BossBettyCheckHPState";

	case DDConditionNode::BossBettyCheckThrowAttack:
		return "BossBettyCheckThrowAttack";

	case DDConditionNode::HPCheck:
		return "HPCheck";

	case DDConditionNode::DeathCheck:
		return "DeathCheck";

	case DDConditionNode::IsCombatCheck:
		return "IsCombatCheck";

	case DDConditionNode::PathFindEnableCheck:
		return "PathFindEnableCheck";

	case DDConditionNode::HitCheckNode:
		return "HitCheckNode";

	case DDConditionNode::RollInputCheck:
		return "RollInputCheck";

	case DDConditionNode::UpdateInputQueue:
		return "UpdateInputQueue";
	}

	return "";
}

DDConditionNode CDDUtil::StringToDDConditionNodeType(const std::string& Str)
{
	if (Str == "BossKnightFinalAttackCheck")
	{
		return DDConditionNode::BossKnightFinalAttackCheck;
	}
	else if (Str == "MoveInputCheckNode")
	{
		return DDConditionNode::MoveInputCheckNode;
	}
	else if (Str == "MouseLButtonCheckNode")
	{
		return DDConditionNode::MouseLButtonCheckNode;
	}
	else if (Str == "NoInterruptNode")
	{
		return DDConditionNode::NoInterruptNode;
	}
	else if (Str == "AttackTargetCheck")
	{
		return DDConditionNode::AttackTargetCheck;
	}
	else if (Str == "MouseRButtonCheckNode")
	{
		return DDConditionNode::MouseRButtonCheckNode;
	}
	else if (Str == "MouseRButtonUpCheckNode")
	{
		return DDConditionNode::MouseRButtonUpCheckNode;
	}
	else if (Str == "Lockstone3TriggerBoxHitCheck")
	{
		return DDConditionNode::Lockstone3TriggerBoxHitCheck;
	}
	else if (Str == "CheckDetectRange")
	{
		return DDConditionNode::CheckDetectRange;
	}
	else if (Str == "BossKnightJumpAttackRangeCheck")
	{
		return DDConditionNode::BossKnightJumpAttackRangeCheck;
	}
	else if (Str == "MeleeAttackRangeCheck")
	{
		return DDConditionNode::MeleeAttackRangeCheck;
	}
	else if (Str == "PostAttackDelayCheck")
	{
		return DDConditionNode::PostAttackDelayCheck;
	}
	else if (Str == "BossKnightCutScenePlayCheck")
	{
		return DDConditionNode::BossKnightCutScenePlayCheck;
	}
	else if (Str == "BossKnightPlayerEnterZoneCheck")
	{
		return DDConditionNode::BossKnightPlayerEnterZoneCheck;
	}
	else if (Str == "BossKnightContinueAttackCheck")
	{
		return DDConditionNode::BossKnightContinueAttackCheck;
	}
	// Boss Betty
	else if (Str == "BossBettyCheckAttackDir")
	{
		return DDConditionNode::BossBettyCheckAttackDir;
	}
	else if (Str == "BossBettyCheckFarAttackType")
	{
		return DDConditionNode::BossBettyCheckFarAttackType;
	}
	else if (Str == "BossBettyCheckHPState")
	{
		return DDConditionNode::BossBettyCheckHPState;
	}
	else if (Str == "BossBettyCheckThrowAttack")
	{
		return DDConditionNode::BossBettyCheckThrowAttack;
	}
	else if (Str == "HPCheck")
	{
		return DDConditionNode::HPCheck;
	}
	else if (Str == "DeathCheck")
	{
		return DDConditionNode::DeathCheck;
	}
	else if (Str == "IsCombatCheck")
	{
		return DDConditionNode::IsCombatCheck;
	}
	else if (Str == "PathFindEnableCheck")
	{
		return DDConditionNode::PathFindEnableCheck;
	}
	else if (Str == "HitCheckNode")
	{
		return DDConditionNode::HitCheckNode;
	}
	else if (Str == "RollInputCheck")
	{
		return DDConditionNode::RollInputCheck;
	}
	else if (Str == "UpdateInputQueue")
	{
	return DDConditionNode::UpdateInputQueue;
	}

	return DDConditionNode(-1);
}

std::string CDDUtil::DDActionNodeTypeToString(DDActionNode NodeType)
{
	switch (NodeType)
	{
	case DDActionNode::Move:
		return "Move";

	case DDActionNode::Idle:
		return "Idle";

	case DDActionNode::NormalAttack:
		return "NormalAttack";

	case DDActionNode::Death:
		return "Death";

	case DDActionNode::RotateAttackDirection:
		return "RotateAttackDirection";

	case DDActionNode::ReadyToShoot:
		return "ReadyToShoot";

	case DDActionNode::ShootNode:
		return "ShootNode";

	case DDActionNode::CancleShootNode:
		return "CancleShootNode";

	case DDActionNode::AddFallingFloorCallback:
		return "AddFallingFloorCallback";

	case DDActionNode::Lockstone3TriggerBoxAction:
		return "Lockstone3TriggerBoxAction";

	case DDActionNode::FindPath:
		return "FindPath";

	case DDActionNode::BossKnightContinueAttack:
		return "BossKnightContinueAttack";

	case DDActionNode::BossKnightFinalAttack:
		return "BossKnightFinalAttack";

	case DDActionNode::BossKnightJumpAttack:
		return "BossKnightJumpAttack";

	case DDActionNode::BossKnightMeleeAttack:
		return "BossKnightMeleeAttack";

	case DDActionNode::BossKnightSlamEnd:
		return "BossKnightSlamEnd";

	case DDActionNode::BossKnightIdle:
		return "BossKnightIdle";

	case DDActionNode::BossKnightWalk:
		return "BossKnightWalk";

	case DDActionNode::BossKnightCutScenePlay:
		return "BossKnightCutScenePlay";

	// Boss Betty
	case DDActionNode::BossBettyAngryAttack:
		return "BossBettyAngryAttack";
	case DDActionNode::BossBettyChangeAttackDir:
		return "BossBettyChangeAttackDir";
	case DDActionNode::BossBettyCloseAttack:
		return "BossBettyCloseAttack";
	case DDActionNode::BossBettyJumpAttack:
		return "BossBettyJumpAttack";
	case DDActionNode::BossBettySpinAttack:
		return "BossBettySpinAttack";
	case DDActionNode::BossBettyThrowAttack:
		return "BossBettyThrowAttack";

	case DDActionNode::ClearPathList:
		return "ClearPathList";

	case DDActionNode::HitBack:
		return "HitBack";

	case DDActionNode::PlayerRoll:
		return "PlayerRoll";
	}

	return "";
}

DDActionNode CDDUtil::StringToDDActionNodeType(const std::string& Str)
{
	if (Str == "BossKnightFinalAttack")
	{
		return DDActionNode::BossKnightFinalAttack;
	}
	else if (Str == "Move")
	{
		return DDActionNode::Move;
	}
	else if (Str == "Idle")
	{
		return DDActionNode::Idle;
	}
	else if (Str == "NormalAttack")
	{
		return DDActionNode::NormalAttack;
	}
	else if (Str == "Death")
	{
		return DDActionNode::Death;
	}
	else if (Str == "RotateAttackDirection")
	{
		return DDActionNode::RotateAttackDirection;
	}
	else if (Str == "ReadyToShoot")
	{
		return DDActionNode::ReadyToShoot;
	}
	else if (Str == "ShootNode")
	{
		return DDActionNode::ShootNode;
	}
	else if (Str == "CancleShootNode")
	{
		return DDActionNode::CancleShootNode;
	}
	else if (Str == "AddFallingFloorCallback")
	{
		return DDActionNode::AddFallingFloorCallback;
	}
	else if (Str == "Lockstone3TriggerBoxAction")
	{
		return DDActionNode::Lockstone3TriggerBoxAction;
	}
	else if (Str == "FindPath")
	{
		return DDActionNode::FindPath;
	}
	else if (Str == "BossKnightContinueAttack")
	{
		return DDActionNode::BossKnightContinueAttack;
	}
	else if (Str == "BossKnightJumpAttack")
	{
		return DDActionNode::BossKnightJumpAttack;
	}
	else if (Str == "BossKnightMeleeAttack")
	{
		return DDActionNode::BossKnightMeleeAttack;
	}
	else if (Str == "BossKnightSlamEnd")
	{
		return DDActionNode::BossKnightSlamEnd;
	}
	else if (Str == "BossKnightWalk")
	{
		return DDActionNode::BossKnightWalk;
	}
	else if (Str == "BossKnightCutScenePlay")
	{
		return DDActionNode::BossKnightCutScenePlay;
	}

	// Boss Betty
	else if (Str == "BossBettyAngryAttack")
	{
		return DDActionNode::BossBettyAngryAttack;
	}
	else if (Str == "BossBettyChangeAttackDir")
	{
		return DDActionNode::BossBettyChangeAttackDir;
	}
	else if (Str == "BossBettyCloseAttack")
	{
		return DDActionNode::BossBettyCloseAttack;
	}
	else if (Str == "BossBettyJumpAttack")
	{
		return DDActionNode::BossBettyJumpAttack;
	}
	else if (Str == "BossBettyThrowAttack")
	{
		return DDActionNode::BossBettyThrowAttack;
	}

	else if (Str == "ClearPathList")
	{
		return DDActionNode::ClearPathList;
	}
	else if (Str == "HitBack")
	{
		return DDActionNode::HitBack;
	}
	else if (Str == "PlayerRoll")
	{
		return DDActionNode::PlayerRoll;
	}

	return DDActionNode(-1);
}

std::string CDDUtil::DDSceneComponentTypeToString(DDSceneComponentType Type)
{
	switch (Type)
	{
	case DDSceneComponentType::PlayerNormalAttackCheckCollider:
		return "PlayerNormalAttackCheckCollider";
	case DDSceneComponentType::EyeLaserComponent:
		return "EyeLaserComponent";
	case DDSceneComponentType::PlayerHookComponent:
		return "PlayerHookComponent";
	case DDSceneComponentType::PlayerBowComponent:
		return "PlayerBowComponent";
	}

	return "";
}

DDSceneComponentType CDDUtil::StringToDDSceneComponentType(const std::string& Str)
{
	if (Str == "PlayerNormalAttackCheckCollider")
	{
		return DDSceneComponentType::PlayerNormalAttackCheckCollider;
	}
	else if (Str == "EyeLaserComponent")
	{
		return DDSceneComponentType::EyeLaserComponent;
	}
	else if (Str == "PlayerHookComponent")
	{
		return DDSceneComponentType::PlayerHookComponent;
	}
	//else if (Str == "MonsterPathFindCollider")
	//{
	//	return DDSceneComponentType::MonsterPathFindCollider;
	//}
	return DDSceneComponentType(-1);
}

size_t CDDUtil::DDSceneComponentTypeToTypeID(DDSceneComponentType Type)
{
	switch (Type)
	{
	case DDSceneComponentType::PlayerNormalAttackCheckCollider:
		return typeid(CPlayerNormalAttackCheckCollider).hash_code();
	case DDSceneComponentType::EyeLaserComponent:
		return typeid(CEyeLaserComponent).hash_code();
	case DDSceneComponentType::PlayerHookComponent:
		return typeid(CPlayerHookComponent).hash_code();
	case DDSceneComponentType::PlayerBowComponent:
		return typeid(CPlayerBowComponent).hash_code();
	}
	return -1;
}

std::string CDDUtil::DDObjectComponentTypeToString(DDObjectComponentType Type)
{
	switch (Type)
	{
	case DDObjectComponentType::PlayerDataComponent:
		return "PlayerDataComponent";

	case DDObjectComponentType::ObjectDataComponent:
		return "ObjectDataComponent";

	case DDObjectComponentType::GameStateComponent:
		return "GameStateComponent";

	case DDObjectComponentType::MonsterData:
		return "MonsterData";

	case DDObjectComponentType::KnightData:
		return "KnightData";
		
	case DDObjectComponentType::BossBettyData:
		return "BossBettyData";

	case DDObjectComponentType::CrowBossData:
		return "CrowBossData";

	case DDObjectComponentType::LurkerData:
		return "LurkerData";

	case DDObjectComponentType::MonsterNavAgent:
		return "MonsterNavAgent";

	case DDObjectComponentType::ProjectileComponent:
		return "ProjectileComponent";

	case DDObjectComponentType::ArrowComponent:
		return "ArrowComponent";
	}

	return "";
}

DDObjectComponentType CDDUtil::StringToDDObjectComponentType(const std::string& Str)
{
	if (Str == "MonsterData")
	{
		return DDObjectComponentType::MonsterData;
	}
	else if (Str == "ObjectDataComponent")
	{
		return DDObjectComponentType::ObjectDataComponent;
	}
	else if (Str == "PlayerDataComponent")
	{
		return DDObjectComponentType::PlayerDataComponent;
	}
	else if (Str == "GameStateComponent")
	{
		return DDObjectComponentType::GameStateComponent;
	}
	else if (Str == "KnightData")
	{
		return DDObjectComponentType::KnightData;
	}
	else if (Str == "BossBettyData")
	{
		return DDObjectComponentType::BossBettyData;
	}
	else if (Str == "CrowBossData")
	{
		return DDObjectComponentType::CrowBossData;
	}
	else if (Str == "LurkerData")
	{
		return DDObjectComponentType::LurkerData;
	}
	else if (Str == "MonsterNavAgent")
	{
		return DDObjectComponentType::MonsterNavAgent;
	}
	else if (Str == "ProjectileComponent")
	{
		return DDObjectComponentType::ProjectileComponent;
	}
	else if (Str == "ArrowComponent")
	{
		return DDObjectComponentType::ArrowComponent;
	}

	return DDObjectComponentType(-1);
}

size_t CDDUtil::DDObjectComponentTypeToTypeID(DDObjectComponentType Type)
{
	switch (Type)
	{
	case DDObjectComponentType::GameStateComponent:
		return typeid(CGameStateComponent).hash_code();
	case DDObjectComponentType::KnightData:
		return typeid(CKnightDataComponent).hash_code();
	case DDObjectComponentType::BossBettyData:
		return typeid(CBossBettyDataComponent).hash_code();
	case DDObjectComponentType::CrowBossData:
		return typeid(CCrowBossDataComponent).hash_code();
	case DDObjectComponentType::MonsterData:
		return typeid(CMonsterDataComponent).hash_code();
	case DDObjectComponentType::ObjectDataComponent:
		return typeid(CObjectDataComponent).hash_code();
	case DDObjectComponentType::PlayerDataComponent:
		return typeid(CPlayerDataComponent).hash_code();
	case DDObjectComponentType::LurkerData:
		return typeid(CLurkerDataComponent).hash_code();
	case DDObjectComponentType::MonsterNavAgent:
		return typeid(CMonsterNavAgent).hash_code();
	case DDObjectComponentType::ProjectileComponent:
		return typeid(CProjectileComponent).hash_code();
	case DDObjectComponentType::ArrowComponent:
		return typeid(CArrowComponent).hash_code();
	}
	return -1;
}
