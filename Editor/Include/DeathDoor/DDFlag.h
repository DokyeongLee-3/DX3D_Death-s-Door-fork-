#pragma once

enum class DDActionNode
{
	Move,
	Idle,
	NormalAttack,
	Death,
	RotateAttackDirection,
	ReadyToShoot,
	ShootNode,
	CancleShootNode,
	AddFallingFloorCallback,
	Lockstone3TriggerBoxAction,
	FindPath,
	BossKnightContinueAttack,
	BossKnightFinalAttack,
	BossKnightJumpAttack,
	BossKnightIdle,
	BossKnightMeleeAttack,
	BossKnightSlamEnd,
	BossKnightWalk,
	BossKnightCutScenePlay,
	ClearPathList,
	HitBack,
	PlayerRoll,
	Max
};

enum class DDConditionNode
{
	MoveInputCheckNode,
	MouseLButtonCheckNode,
	NoInterruptNode,
	AttackTargetCheck,
	MouseRButtonCheckNode,
	MouseRButtonUpCheckNode,
	Lockstone3TriggerBoxHitCheck,
	CheckDetectRange,
	MeleeAttackRangeCheck,
	PostAttackDelayCheck,
	BossKnightFinalAttackCheck,
	BossKnightJumpAttackRangeCheck,
	BossKnightCutScenePlayCheck,
	BossKnightPlayerEnterZoneCheck,
	BossKnightContinueAttackCheck,
	HPCheck,
	DeathCheck,
	IsCombatCheck,
	PathFindEnableCheck,
	HitCheckNode,
	RollInputCheck,
	Max
};

enum class DDSceneComponentType
{
	PlayerNormalAttackCheckCollider,
	EyeLaserComponent,
	PlayerHookComponent,
	PlayerBowComponent,
	Max
};

enum class DDObjectComponentType
{
	ObjectDataComponent,
	PlayerDataComponent,
	GameStateComponent,
	MonsterData,
	KnightData,
	LurkerData,
	MonsterNavAgent,
	ProjectileComponent,
	Max
};


enum class HookResult
{
	// ���ư��� �ִ���
	OnShoot,
	NoCollision,
	Collision,
	CollisionEnd
};
