#include "BossBettyDataComponent.h"
#include "GameBehaviorTree.h"
#include "GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "BossBettyDataComponent.h"

CBossBettyDataComponent::CBossBettyDataComponent() :
	m_ThrowFarAttackEnable(false),
	m_FarAttackType(BossBettyFarAttackType::Spin)
{
}

CBossBettyDataComponent::CBossBettyDataComponent(const CBossBettyDataComponent& com)
{
}

CBossBettyDataComponent::~CBossBettyDataComponent()
{
}

void CBossBettyDataComponent::Start()
{
    // m_BossBettySpinCollider �� Object �� Component List �� �߰��Ѵ�.
    // - �׸��� Spin �߰���, Collide �� Spin Collider Animation ���� �ٲٴ� Callback�� �����Ѵ�.
    // - ó������ ��Ȱ��ȭ ��Ų��.
    m_BossBettySpinCollider = m_Object->CreateComponent<CColliderBox3D>("BossBettySpinCollide");
    m_BossBettySpinCollider->Enable(false);
    m_BossBettySpinCollider->AddCollisionCallback(Collision_State::Begin, 
        this, &CBossBettyDataComponent::OnChangeFromSpinToSpinCollideWhenCollide);

    // �ٰŸ� ���� �Ÿ� �Ǻ� Square Pos ��ġ ����� 
    //  0: ���� �ϴ�, 1 : ���� ���, 2 : ������ ���, 3 : ������ �ϴ�
    const Vector3& ObjectWorldScale = m_Object->GetRootComponent()->GetWorldScale();

    m_PunchLeftSquarePos[0] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z * 0.5f);
    m_PunchLeftSquarePos[1] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z);
    m_PunchLeftSquarePos[2] = Vector3(0.f, 0.f, ObjectWorldScale.z);
    m_PunchLeftSquarePos[3] = Vector3(0.f, 0.f, ObjectWorldScale.z * 0.5f);

    m_PunchRightSquarePos[0] = Vector3(0.f, 0.f, ObjectWorldScale.z * 0.5f);
    m_PunchRightSquarePos[1] = Vector3(0.f, 0.f, ObjectWorldScale.z);
    m_PunchRightSquarePos[2] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z);
    m_PunchRightSquarePos[3] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z * 0.5f);

    m_SlashLeftSquarePos[0] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z * -1.f);
    m_SlashLeftSquarePos[1] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z);
    m_SlashLeftSquarePos[2] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z);
    m_SlashLeftSquarePos[3] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z * -1.f);

    m_SlashRightSquarePos[0] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z * -1.f);
    m_SlashRightSquarePos[1] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z);
    m_SlashRightSquarePos[2] = Vector3(ObjectWorldScale.x * 1 * 1.5f, 0.f, ObjectWorldScale.z);
    m_SlashRightSquarePos[3] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z * -1.f);

    m_TwoSideFrontSquarePos[0] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z);
    m_TwoSideFrontSquarePos[1] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z * 1.5f);
    m_TwoSideFrontSquarePos[2] = Vector3(ObjectWorldScale.x * 1 * 1.5f, 0.f, ObjectWorldScale.z * 1.5f);
    m_TwoSideFrontSquarePos[3] = Vector3(ObjectWorldScale.x * 1 * 1.5f, 0.f, ObjectWorldScale.z);

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
}

void CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect()
{
    // m_RelativePunchRightPos
}

void CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect()
{
    // m_RelativePunchLeftPos
}

void CBossBettyDataComponent::OnBossBettyRoarEffect()
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
    CAnimationSequenceInstance* AnimInst =  dynamic_cast<CAnimationMeshComponent*>(m_Object->GetRootComponent())->GetAnimationInstance();

    if (AnimInst->GetCurrentAnimation()->GetName() == "Spin")
    {
        AnimInst->ChangeAnimation("SpinCollide");
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
