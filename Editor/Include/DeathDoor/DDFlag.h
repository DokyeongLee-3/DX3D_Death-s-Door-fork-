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
	// BossBetty
	BossBettyAngryAttack,
	BossBettyChangeAttackDir,
	BossBettyCloseAttack,
	BossBettyJumpAttack,
	BossBettySpinAttack,
	BossBettyThrowAttack,
	// CrowBoss
	CrowBossCutScenePlayNode,
	CrowBossRunNode,
	CrowBossSpinNode,
	CrowBossJump,
	// SporeBoomer
	SporeBoomerShoot,
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
	PlayerEnterZoneCheck,
	PlayerEnterTriggerIsEnable,
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
	UpdateInputQueue,
	// BossBetty
	BossBettyCheckAttackDir,
	BossBettyCheckFarAttackType,
	BossBettyCheckHPState,
	BossBettyCheckThrowAttack,
	// CrowBoss
	CrowBossCutScenePlayCheck,
	CrowBossDirectionCheck,
	CrowBossPhasePickNode,
	CrowBossBypassCheck,
	CrowBossJumpStartCheck,
	CrowBossLandingCheck,
	Max
};

enum class DDSceneComponentType
{
	PlayerNormalAttackCheckCollider,
	EyeLaserComponent,
	PlayerHookComponent,
	PlayerBowComponent,
	CrackedBlockCollider,
	Max
};

enum class DDObjectComponentType
{
	ObjectDataComponent,
	PlayerDataComponent,
	GameStateComponent,
	MonsterData,
	KnightData,
	BossBettyData,
	CrowBossData,
	LurkerData,
	MonsterNavAgent,
	ProjectileComponent,
	ArrowComponent,
	SporeBoomerData,
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

// TODO : Death Door SceneMode �߰��ø��� ������Ʈ
enum class DDSceneModeType
{
	DDSceneMode,
	DDBossSceneMode,
	Max
};
