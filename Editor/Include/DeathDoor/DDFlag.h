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
	ClimbDown,
	ClimbDownEnd,
	ClimbDownStart,
	ClimbUp,
	ClimbUpEnd,
	ClimbUpStart,
	ClimbPause,
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
	// Monster
	ChasePlayer,
	MeleeAttack,
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
	CrowBossFly,
	CrowBossMoveShootSpot,
	CrowBossResetShootState,
	CrowBossShoot,
	CrowBossSlide,
	CrowBossSpitting,
	// SporeBoomer
	SporeBoomerShoot,
	// HeadRoller
	HeadRollerStun,
	HeadRollerRoll,
	// Dodger
	DodgerFirstAttack,
	DodgerSecondAttack,
	DodgerPostAttack,
	DodgerDash,
	// PlagueKnight
	PlagueKnightShoot,
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
	AttackCoolTimeCheck,
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
	SKeyInputCheck,
	WKeyInputCheck,
	IsClimbingCheck,
	ClimbDownContinueCheck,
	ClimbDownEndCheck,
	ClimbDownStartCheck,
	ClimbUpContinueCheck,
	ClimbUpEndCheck,
	ClimbUpStartCheck,
	StraightPathCheck,
	// BossBetty
	BossBettyCheckAttackDir,
	BossBettyCheckFarAttackType,
	BossBettyCheckHPState,
	BossBettyCheckThrowAttack,
	// CrowBoss
	CrowBossCutScenePlayCheck,
	CrowBossPhasePickNode,
	CrowBossBypassCheck,
	CrowBossJumpStartCheck,
	CrowBossLandingCheck,
	CrowBossJumpAttackRangeCheck,
	CrowBossDoneCheck,
	CrowBossFirstShootCheck,
	CrowBossShootCheck,
	CrowBossShootEndCheck,
	CrowBossShootReadyCheck,
	// HeadRoller
	HeadRollerStunCheck,
	// Dodger
	DodgerDashCheck,
	DodgerSecondAttackCheck,
	Max
};

enum class DDSceneComponentType
{
	PlayerNormalAttackCheckCollider,
	EyeLaserComponent,
	PlayerHookComponent,
	PlayerBowComponent,
	LadderCollider,
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
	TinyCrowDataComponent,
	PlayerBombComponent,
	SporeBoomerData,
	FirePlantData,
	MonsterBulletData,
	HeadRollerData,
	DodgerData,
	PlagueKnightData,
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

enum class CrowBossShootState
{
	Ready,
	ReadyEnd,
	Teleport,
	Shoot,
	ShootEnd,
	Fly,
	Done,
	Max
};

// TODO : Death Door SceneMode �߰��ø��� ������Ʈ
enum class DDSceneModeType
{
	DDSceneMode,
	DDBossSceneMode,
	DDInstanceSceneMode,
	DDPuzzleSceneMode,
	Max
};
