#include "BossBettyDataComponent.h"
#include "GameBehaviorTree.h"
#include "GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "BossBettyDataComponent.h"
#include "../DataManager.h"
#include "Component/CameraComponent.h"
#include "GameObject/GameObject.h"

CBossBettyDataComponent::CBossBettyDataComponent() :
	m_ThrowFarAttackEnable(false),
	m_FarAttackType(BossBettyFarAttackType::JumpSmash),
    m_ChangeDirLimitAngle(5.f)
{
    m_ComponentType = Component_Type::ObjectComponent;
    SetTypeID<CBossBettyDataComponent>();
}

CBossBettyDataComponent::CBossBettyDataComponent(const CBossBettyDataComponent& com) :
    CMonsterDataComponent(com)
{
}

CBossBettyDataComponent::~CBossBettyDataComponent()
{
}

void CBossBettyDataComponent::Start()
{
    CMonsterDataComponent::Start();

    //  CMonsterDataComponent::Start(); ���� HitBox Collider �� ã���� ���̴�.
    // �ش� Collider �� Extent �� �������ش�.
    m_HitBox->SetExtent(3.0f, 3.0f, 3.0f);

    m_Data = CDataManager::GetInst()->GetObjectData("BossBetty");

    // Move Speed �� CSV OBJ Data �� �����Ѵ�.
    m_OriginMoveSpeed = m_Data.MoveSpeed;
    m_CurMoveSpeed = m_Data.MoveSpeed;

    // Move Speed �� CSV OBJ Data �� �����Ѵ�.
    m_OriginRotSpeed = m_Data.RotateSpeedPerSec;
    m_CurRotSpeed = m_OriginRotSpeed;

    // HitBox �� �ݹ��� �ɾ��ش�.

    // Current Animation �� Idle �� �����Ѵ�.
    CAnimationSequenceInstance* AnimInst = dynamic_cast<CAnimationMeshComponent*>(m_Object->GetRootComponent())->GetAnimationInstance();
    AnimInst->ChangeAnimation("Idle");

    // m_BossBettySpinCollider �� Object �� Component List �� �߰��Ѵ�.
    // - �׸��� Spin �߰���, Collide �� Spin Collider Animation ���� �ٲٴ� Callback�� �����Ѵ�.
    // - ó������ ��Ȱ��ȭ ��Ų��.
    m_BossBettySpinCollider = (CColliderBox3D*)(m_Object->FindComponent("BossBettySpinCollide"));

    if (m_BossBettySpinCollider)
    {
        m_BossBettySpinCollider->Enable(false);

        m_BossBettySpinCollider->AddCollisionCallback(Collision_State::Stay,
            this, &CBossBettyDataComponent::OnChangeFromSpinToSpinCollideWhenCollide);

        m_BossBettySpinCollider->AddCollisionCallback(Collision_State::Stay,
            this, &CBossBettyDataComponent::OnPreventGoingOutOfMapSurroundingCollider);
    }

    m_MeleeAttackCollider = dynamic_cast<CColliderBox3D*>((m_Object->FindComponent("BossBettyAttackCollider")));

    if (m_MeleeAttackCollider)
    {
        m_MeleeAttackCollider->SetExtent(3.f, 3.f, 3.f);

        m_MeleeAttackCollider->Enable(false);

        m_MeleeAttackCollider->AddCollisionCallback(Collision_State::Begin,
            (CMonsterDataComponent*)this, &CMonsterDataComponent::OnHitMeleeAttack);
        
    }

    // �ٰŸ� ���� �Ÿ� �Ǻ� Square Pos ��ġ ����� 
    //  0: ���� �ϴ�, 1 : ���� ���, 2 : ������ ���, 3 : ������ �ϴ�
    const Vector3& ObjectWorldScale = m_Object->GetRootComponent()->GetWorldScale();
    const Vector3& MeshOriginScale = dynamic_cast<CAnimationMeshComponent*>(m_Object->GetRootComponent())->GetMeshSize();
    // const Vector3& FinalMeshScale = ObjectWorldScale * MeshOriginScale;
    const Vector3& FinalMeshScale = Vector3(10.f, 0.f, 10.f);

    m_PunchLeftSquarePos[0] = Vector3(FinalMeshScale.x * -1, 0.f, 0.f);
    m_PunchLeftSquarePos[1] = Vector3(FinalMeshScale.x * -1, 0.f, FinalMeshScale.z);
    m_PunchLeftSquarePos[2] = Vector3(0.f, 0.f, FinalMeshScale.z);
    m_PunchLeftSquarePos[3] = Vector3(0.f, 0.f, 0.f);

    m_PunchRightSquarePos[0] = Vector3(0.f, 0.f, 0.f);
    m_PunchRightSquarePos[1] = Vector3(0.f, 0.f, FinalMeshScale.z);
    m_PunchRightSquarePos[2] = Vector3(FinalMeshScale.x, 0.f, FinalMeshScale.z);
    m_PunchRightSquarePos[3] = Vector3(FinalMeshScale.x, 0.f, 0.f);

    m_SlashLeftSquarePos[0] = Vector3(FinalMeshScale.x * -1 * 1.5f, 0.f, FinalMeshScale.z * -1.5f);
    m_SlashLeftSquarePos[1] = Vector3(FinalMeshScale.x * -1 * 1.5f, 0.f, FinalMeshScale.z);
    m_SlashLeftSquarePos[2] = Vector3(FinalMeshScale.x * -1 * 0.5f, 0.f, FinalMeshScale.z);
    m_SlashLeftSquarePos[3] = Vector3(FinalMeshScale.x * -1 * 0.5f, 0.f, FinalMeshScale.z * -1.5f);

    m_SlashRightSquarePos[0] = Vector3(FinalMeshScale.x * 0.5f, 0.f, FinalMeshScale.z * -1.5f);
    m_SlashRightSquarePos[1] = Vector3(FinalMeshScale.x * 0.5f, 0.f, FinalMeshScale.z);
    m_SlashRightSquarePos[2] = Vector3(FinalMeshScale.x * 1 * 1.5f, 0.f, FinalMeshScale.z);
    m_SlashRightSquarePos[3] = Vector3(FinalMeshScale.x * 1 * 1.5f, 0.f, FinalMeshScale.z * -1.5f);

    m_TwoSideFrontSquarePos[0] = Vector3(FinalMeshScale.x * -1 * 1.5f, 0.f, FinalMeshScale.z * 0.5f);
    m_TwoSideFrontSquarePos[1] = Vector3(FinalMeshScale.x * -1 * 1.5f, 0.f, FinalMeshScale.z * 1.5f);
    m_TwoSideFrontSquarePos[2] = Vector3(FinalMeshScale.x * 1 * 1.5f, 0.f, FinalMeshScale.z * 1.5f);
    m_TwoSideFrontSquarePos[3] = Vector3(FinalMeshScale.x * 1 * 1.5f, 0.f, FinalMeshScale.z * 0.5f);

    m_RelativePunchRightPos = (m_PunchRightSquarePos[0] + m_PunchRightSquarePos[2]) / 2.f;
    m_RelativePunchLeftPos = (m_PunchLeftSquarePos[0] + m_PunchLeftSquarePos[2]) / 2.f;

    m_RelativeSlashRightPos = (m_SlashLeftSquarePos[0] + m_SlashLeftSquarePos[2]) / 2.f;
    m_RelativeSlashLeftPos = (m_SlashRightSquarePos[0] + m_SlashRightSquarePos[2]) / 2.f;
}

void CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect()
{
	// ���ʿ� 
	// 1) �浹ü Ȱ��ȭ
	// 2) Particle ����

    const Vector3& XWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_X) * -1.f;
    const Vector3& ZWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_Z) * -1.f;

    const Vector3& ColliderRelativePos = ZWorldAxis * 8.0f;

    m_MeleeAttackCollider->SetRelativePos(ColliderRelativePos);

    m_MeleeAttackCollider->SetExtent(5.f, 1.f, 3.f);
}

void CBossBettyDataComponent::OnSetBossBettyAttackColliderPosToBettyBody()
{
    m_MeleeAttackCollider->SetRelativePos(Vector3(0.f, 0.f, 0.f));
    m_MeleeAttackCollider->SetExtent(3.f, 3.f, 3.f);
}

void CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect()
{
    const Vector3& XWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_X) * -1.f;
    const Vector3& ZWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_Z) * -1.f;

    const Vector3& ColliderRelativePos = XWorldAxis * 3.0f + ZWorldAxis * 5.0f;

    m_MeleeAttackCollider->SetRelativePos(ColliderRelativePos);
    m_MeleeAttackCollider->SetExtent(3.f, 3.f, 4.f);
}

void CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect()
{
    const Vector3& XWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_X) * -1.f;
    const Vector3& ZWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_Z) * -1.f;

    const Vector3& ColliderRelativePos = XWorldAxis * 3.5f * -1.f + ZWorldAxis * 5.0f;

    m_MeleeAttackCollider->SetRelativePos(ColliderRelativePos);
    m_MeleeAttackCollider->SetExtent(3.f, 3.f, 4.f);
}

void CBossBettyDataComponent::OnBossBettyRoarEffect(float DeltaTime)
{
	// 1. ������, ���� ���·� ������� ������ Particle ������ 
}

// ����ü�� ���� ����, ����ü�� �ٴ�, �� � �浹 ����, ���� �� ȿ�� 
void CBossBettyDataComponent::OnExplodeBettyThrowBallCallback()
{
    // �ڱ� ��ġ�� �������� 20���� ��ġ�� �����Ѵ�.
    // ������ Delay �� �ָ鼭 �������� �� ���̴�.
}

void CBossBettyDataComponent::OnChangeFromSpinToSpinCollideWhenCollide(const CollisionResult& Result)
{
    CAnimationSequenceInstance* AnimInst = dynamic_cast<CAnimationMeshComponent*>(m_Object->GetRootComponent())->GetAnimationInstance();

    if (AnimInst->GetCurrentAnimation()->GetName() == "Spin")
    {
        if (Result.Dest->GetGameObject()->GetName() != "MapSurroundCollider")
            return;

        // �� ~ �� �浹
        // BossBettySpinCollider �浹ü��, ��ü Map Surround Collider �ȿ� �����Ѵ�.
        // �� �ȿ�, Ư�� ���� ������ ��쿡��, �׻� �浹�� �����Ѵ�.
        // ���, ������ �浹�ϴ� �� �������� HitPoint ������ 0�� �ƴ� ���·� �����ϰ� �� ���̴�.
        if (Result.HitPoint.x == 0.f)
            return;

        // 1. Spin Collider �� Change
        AnimInst->ChangeAnimation("SpinCollide");

        // �� �浹 -> Camera Shake ȿ���� �ش�.
        OnBossBettyNormalShakeCamera();

        // 2. ���̻��� �������� �����.
        m_MoveZ = false;

        // BossBetty Spin Collider �� Enable False ó�� �Ѵ�.
        m_BossBettySpinCollider->Enable(false);
    }
}

void CBossBettyDataComponent::OnPreventGoingOutOfMapSurroundingCollider(const CollisionResult& Result)
{
    CAnimationSequenceInstance* AnimInst = dynamic_cast<CAnimationMeshComponent*>(m_Object->GetRootComponent())->GetAnimationInstance();
    
    if (Result.Dest->GetGameObject()->GetName() != "MapSurroundCollider")
        return;

    // ������ ������ ����, Z Move �� ��Ȱ��ȭ �Ѵ�.
    if (Result.HitPoint.x > 0.f)
    {
        OnDisableMoveZ();
    }
}

void CBossBettyDataComponent::OnBossBettyDisableSpinCollider()
{
    m_BossBettySpinCollider->Enable(false);
}

void CBossBettyDataComponent::OnBossBettyEnableSpinCollider()
{
    m_BossBettySpinCollider->Enable(true);
}

void CBossBettyDataComponent::OnBossBettySetCurrentNodeNullPtr()
{	
    // Current Node �� BettySpinAttackNode �� �Ǿ� ���� ���̴�. 
    // �̸� nullptr �� ����
    m_State->GetBehaviorTree()->SetCurrentNode(nullptr);
}

void CBossBettyDataComponent::OnBossBettyResetOriginalMoveSpeed()
{
    m_CurMoveSpeed = m_OriginMoveSpeed;
}

void CBossBettyDataComponent::OnBossBettyNormalShakeCamera()
{
    m_Scene->GetCameraManager()->GetCurrentCamera()->Shake(0.2f, 1.2f);
}

void CBossBettyDataComponent::OnBossBettyApplyOutOfMapSurroundingColliderMoveSpeed()
{
   SetCurMoveSpeed(m_OriginMoveSpeed * 0.2f);
}

void CBossBettyDataComponent::OnBossBettyEnableAttackCollider()
{
    CMonsterDataComponent::OnActiveMeleeAttackCollider();

    m_MeleeAttackCollider->Enable(true);
}

void CBossBettyDataComponent::OnBossBettyDisableAttackCollider()
{
    CMonsterDataComponent::OnInActiveMeleeAttackCollider();

    m_MeleeAttackCollider->Enable(false);
}
