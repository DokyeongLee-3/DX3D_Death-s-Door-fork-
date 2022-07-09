
#pragma once

enum class Engine_Space
{
	Space2D,
	Space3D
};

enum AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

enum class Buffer_Type
{
	Vertex,
	Index
};

enum class Shader_Type
{
	Graphic,
	Compute
};

enum class Buffer_Shader_Type
{
	Vertex = 0x1,
	Pixel = 0x2,
	Domain = 0x4,
	Hull = 0x8,
	Geometry = 0x10,
	Compute = 0x20,
	Graphic = Vertex | Pixel | Domain | Hull | Geometry,
	All = Vertex | Pixel | Domain | Hull | Geometry | Compute
};

enum class Component_Type
{
	SceneComponent,
	ObjectComponent
};

enum class Input_Type
{
	Direct,
	Window
};

enum Key_State
{
	KeyState_Down,
	KeyState_Push,
	KeyState_Up,
	KeyState_Max
};

enum class Image_Type
{
	Atlas,
	Frame,
	Array,
	RenderTarget
};

enum class Sampler_Type
{
	Point,
	Linear,
	Anisotropic
};

enum class RenderState_Type
{
	Blend,
	Rasterizer,
	DepthStencil,
	Max
};

enum class SceneComponent_Type
{
	Sprite,
	StaticMesh,
	Box2D,
	Circle,
	Pixel,
	Camera,
	Widget,
	Particle,
	TileMap,
	Max
};

enum class Collision_Channel
{
	Object,
	Player,
	Monster,
	PlayerAttack,
	MonsterAttack,
	Custom1,
	Custom2,
	Custom3,
	Custom4,
	Custom5,
	Custom6,
	Custom7,
	Custom8,
	Custom9,
	Custom10,
	Custom11,
	Custom12,
	Custom13,
	Custom14,
	Custom15,
	Custom16,
	Max
};

enum class Collision_Interaction
{
	Ignore,
	Collision
};

enum class Collision_State
{
	Begin,
	End,
	Max
};

enum class Collider_Type
{
	Box2D,
	Circle,
	Pixel,
	Sphere,
	Box3D,
	HalfLine,
	Ray
};

enum class Camera_Type
{
	Camera2D,
	Camera3D,
	CameraUI
};

enum class PixelCollision_Type
{
	Color_Ignore,
	Color_Confirm,
	Alpha_Ignore,
	Alpha_Confirm
};

enum class Button_State
{
	Normal,
	MouseOn,
	Click,
	Disable,
	Max
};

enum class Button_Sound_State
{
	MouseOn,
	Click,
	Max
};

enum class WidgetComponent_Space
{
	Screen,
	World
};

enum class ProgressBar_Dir
{
	RightToLeft,
	LeftToRight,
	TopToBottom,
	BottomToTop
};

enum class Mouse_State
{
	Normal,
	State1,
	State2,
	State3,
	State4,
	State5,
	State6,
	State7,
	State8,
	State9,
	State10,
	Max
};

enum class Tile_Shape
{
	Rect,
	Rhombus,
	End
};

enum class Tile_Type
{
	Normal,
	Wall,
	End
};

enum class Nav_Node_Type
{
	None,
	Open,
	Close
};

enum class Mesh_Type
{
	Sprite,
	Static,
	Animation,
	Nav
};

enum class Light_Type
{
	Dir,
	Point,
	Spot,
	End
};

enum class PopUpModalState
{
	Open,
	Closed,
	Max
};

enum class SceneComponent3DType
{
	AnimationMeshComponent,
	StaticMeshComponent,
	LandScapeComponent,
	ArmComponent,
	LightComponent,
	SceneComponent,
	ParticleComponent,
	ColliderBox3D,
	ColliderSphere,
	ColliderHalfLine,
	ColliderRay,
	NavMeshComponent,
	Max
};

enum class ObjectComponent3DType
{
	PaperBurnComponent,
	GameStateComponent,
	NavAgent,
	GameDataComponent,
	PlayerDataComponent,
	Max
};

enum class Transform_State
{
	None,
	Ground,
	Falling
};

enum class NodeResult
{
	Node_True,
	Node_False,
	Node_Running,
	Max
};

enum ConditionNode
{
	MoveInputCheckNode,
	MouseLButtonCheckNode,
	NoInterruptNode,
	AttackTargetCheck,
	MouseRButtonCheckNode,
	MouseRButtonUpCheckNode,
};

enum ActionNode
{
	Move,
	Idle,
	NormalAttack,
	Death,
	RotateAttackDirection,
	ReadyToShoot,
	ShootNode,
	CancleShootNode,
};

enum DecoratorNode
{
	Negate
};

enum Node_Type
{
	Sequence,
	Selector,
	Condition,
	Action,
	Decorator
};

enum class Object_Type
{
	Player,
	Monster,
	MapObject,
	None
};

enum class Fog_Type
{
	Depth,
	Y,
	Max
};

enum class Player_Ability
{
	Arrow,
	Fire,
	Hook,
	Photon
};

enum class ToonShaderType
{
	Default,
	Easy,
	Light,
	Warm,
	Max
};

enum class ParitcleShapeType
{
	YUpDirRing = 0, // 위를 바라보는 형태로 즉, Y 를 바라보는 형태로 Ring 생성
	Circle,
	ZMinusDirRing, // 사용자 정면을 바라보는 형태로 Ring 생성
	Max
};

enum class ParticleSpecialMoveDir
{
	XZSpread = 0, // xz 평명 방향으로 이동 y는 0
	XYSpread, // xy 평명 방향으로 이동 z 는 0
	Max
};