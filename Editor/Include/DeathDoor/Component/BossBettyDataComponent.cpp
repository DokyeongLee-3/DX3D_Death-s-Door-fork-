#include "BossBettyDataComponent.h"
#include "GameBehaviorTree.h"
#include "GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "BossBettyDataComponent.h"
#include "../DataManager.h"
#include "Component/CameraComponent.h"
#include "GameObject/GameObject.h"
#include "ObjectPool.h"

CBossBettyDataComponent::CBossBettyDataComponent() :
	m_ThrowFarAttackEnable(false),
	m_FarAttackType(BossBettyFarAttackType::JumpSmash),
    m_ChangeDirLimitAngle(10.f),
    m_CloseAttackAnimChangeEnable(true)
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
    m_HitBox->SetExtent(3.5f, 3.5f, 3.5f);

    m_Data = CDataManager::GetInst()->GetObjectData("BossBetty");

    // Move Speed �� CSV OBJ Data �� �����Ѵ�.
    m_OriginMoveSpeed = m_Data.MoveSpeed;
    m_CurMoveSpeed = m_Data.MoveSpeed;

    // Move Speed �� CSV OBJ Data �� �����Ѵ�.
    m_OriginRotSpeed = m_Data.RotateSpeedPerSec;
    m_CurRotSpeed = m_OriginRotSpeed;

    m_BettyHPMax = (float)m_Data.HP;

    // HitBox �� �ݹ��� �ɾ��ش�.

    // Current Animation �� Idle �� �����Ѵ�.
    CAnimationSequenceInstance* AnimInst = dynamic_cast<CAnimationMeshComponent*>(m_Object->GetRootComponent())->GetAnimationInstance();
    // AnimInst->ChangeAnimation("Idle");

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
    // m_MeleeAttackCollider

    if (m_MeleeAttackCollider)
    {
        m_MeleeAttackCollider->SetExtent(3.f, 3.f, 3.f);

        m_MeleeAttackCollider->Enable(false);

        m_MeleeAttackCollider->AddCollisionCallback(Collision_State::Begin,
            (CMonsterDataComponent*)this, &CMonsterDataComponent::OnHitMeleeAttack);
    }

    // CParticleComponent* FoundParticle = (CParticleComponent*)(m_Object->FindComponent("AttackGrass"));
    // 
    // if (FoundParticle)
    // {
    //     FoundParticle->GetCBuffer()->SetFollowRealTimeParticleComponentPos(true);
    //     m_vecAttackAfterEffectParticle.push_back(FoundParticle);
    // }
    // 
    // FoundParticle = (CParticleComponent*)(m_Object->FindComponent("AttackCircle"));
    // 
    // if (FoundParticle)
    // {
    //     FoundParticle->GetCBuffer()->SetFollowRealTimeParticleComponentPos(true);
    //     m_vecAttackAfterEffectParticle.push_back(FoundParticle);
    // }


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

void CBossBettyDataComponent::OnActivateBloodParticle()
{
    CMonsterDataComponent::OnActivateBloodParticle();

   // const Vector3& XWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_X) * -1.f;
   // const Vector3& ZWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_Z) * -1.f;
   // 
   // Vector3 RelativePos = ZWorldAxis * (rand() % 1) * 3.f + Vector3(0.f, 2.f, 0.f);
   // 
   //  m_BloodParticle->SetRelativePos(RelativePos);
}

void CBossBettyDataComponent::DecreaseHP(int Amount)
{
    int PrevHP = m_Data.HP;

    CObjectDataComponent::DecreaseHP(Amount);

    int CurHP = m_Data.HP;

    if (PrevHP >= m_BettyHPMax * 0.6f && CurHP < m_BettyHPMax * 0.6f)
    {
        m_BettyHPState = BossBettyHPState::Below60;
    }

    if (PrevHP >= m_BettyHPMax * 0.3f && CurHP < m_BettyHPMax * 0.3f)
    {
        m_BettyHPState = BossBettyHPState::Below30;
    }
}

void CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect()
{
	// ���ʿ� 
	// 1) �浹ü Ȱ��ȭ
	// 2) Particle ����

    const Vector3& XWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_X) * -1.f;
    const Vector3& ZWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_Z) * -1.f;

    const Vector3& ColliderRelativePos = ZWorldAxis * 6.0f;

    m_MeleeAttackCollider->SetRelativePos(ColliderRelativePos);

    m_MeleeAttackCollider->SetExtent(4.f, 1.f, 4.f);

    OnBossBettyActivateAfterEffect(m_Object->GetWorldPos() + ColliderRelativePos);
}

void CBossBettyDataComponent::OnSetBossBettyAttackColliderPosToBettyBody()
{
    m_MeleeAttackCollider->SetRelativePos(Vector3(0.f, 0.f, 0.f));
    m_MeleeAttackCollider->SetExtent(2.5f, 2.5f, 2.5f);
}

void CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect()
{
    const Vector3& XWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_X) * -1.f;
    const Vector3& ZWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_Z) * -1.f;

    const Vector3& ColliderRelativePos = XWorldAxis * 3.0f + ZWorldAxis * 3.0f;

    m_MeleeAttackCollider->SetRelativePos(ColliderRelativePos);
    m_MeleeAttackCollider->SetExtent(2.5f, 2.5f, 5.f);

    OnBossBettyActivateAfterEffect(m_Object->GetWorldPos() + ColliderRelativePos);
}

void CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect()
{
    const Vector3& XWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_X) * -1.f;
    const Vector3& ZWorldAxis = m_MeleeAttackCollider->GetRelativeAxis(AXIS::AXIS_Z) * -1.f;

    const Vector3& ColliderRelativePos = XWorldAxis * 3.5f * -1.f + ZWorldAxis * 3.0f;

    m_MeleeAttackCollider->SetRelativePos(ColliderRelativePos);
    m_MeleeAttackCollider->SetExtent(2.5f, 2.5f, 5.f);

    OnBossBettyActivateAfterEffect(m_Object->GetWorldPos() + ColliderRelativePos);
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
        if (Result.Dest->GetGameObject()->GetName() != "MapSurrounding")
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
    
    if (Result.Dest->GetGameObject()->GetName() != "MapSurrounding")
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

void CBossBettyDataComponent::OnBossBettySetAttackColliderToBettyBodyPos()
{
    m_MeleeAttackCollider->SetExtent(3.f, 3.f, 3.f);
    m_MeleeAttackCollider->SetWorldPos(m_Object->GetWorldPos());
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

void CBossBettyDataComponent::OnBossBettyActivateAfterEffect(const Vector3& WorldPos)
{
    CGameObject* AfterEffectParticle = CObjectPool::GetInst()->GetParticle("BettyAttackAfterEffect", CSceneManager::GetInst()->GetScene());
    AfterEffectParticle->Enable(true);
    AfterEffectParticle->SetWorldPos(WorldPos);

    // ��� ParticleComponent ��� �Ͽ��� SetFollowRealTimeParticleComponentPos �� true �� �����Ѵ�.
// �׷��߸�, ���� ��ġ���� Particle �� �����ȴ�.
    std::vector<CParticleComponent*> vecParticleComponents;
    AfterEffectParticle->FindAllSceneComponentFromType(vecParticleComponents);

    size_t vecSize = vecParticleComponents.size();

    for (size_t i = 0; i < vecSize; ++i)
    {
        vecParticleComponents[i]->RecreateOnlyOnceCreatedParticle();
    }

    // AfterEffectParticle->SetLifeSpan(1.f);
}

void CBossBettyDataComponent::OnBossBettyEnableCloseAttackChangeAnim()
{
    m_CloseAttackAnimChangeEnable = true;
}

void CBossBettyDataComponent::OnBossBettyDisableCloseAttackChangeAnim()
{
    m_CloseAttackAnimChangeEnable = false;
}

void CBossBettyDataComponent::IncFarAttackCount()
{
    ++m_FarAttackAttackNum;

    // 3�� ����, Far Attack Type �� �ٸ��� ���� ���̴�.
    //  if (m_FarAttackAttackNum % 3 == 0)
    if (m_FarAttackAttackNum % 2 == 0)
    {
        if (m_FarAttackType == BossBettyFarAttackType::Spin)
            m_FarAttackType = BossBettyFarAttackType::JumpSmash;
        else
            m_FarAttackType = BossBettyFarAttackType::Spin;
    }

    // if (m_FarAttackAttackNum == 4)
    if (m_FarAttackAttackNum == 2)
    {
        m_ThrowFarAttackEnable = true;
        m_FarAttackAttackNum = 0;
    }
}
