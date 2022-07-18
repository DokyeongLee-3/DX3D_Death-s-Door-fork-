#include "KnightDataComponent.h"
#include "../DataManager.h"
#include "Component/ColliderBox3D.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CKnightDataComponent::CKnightDataComponent()	:
	m_JumpAttackRange(0.f),
	m_MeleeAttackCount(0)
{
	SetTypeID<CKnightDataComponent>();
}

CKnightDataComponent::CKnightDataComponent(const CKnightDataComponent& com)	:
	CMonsterDataComponent(com)
{
	m_JumpAttackRange = com.m_JumpAttackRange;
	m_MeleeAttackCount = 0;
}

CKnightDataComponent::~CKnightDataComponent()
{
}

void CKnightDataComponent::Start()
{
	m_Data = CDataManager::GetInst()->GetObjectData("BossKnight");
	m_MeleeAttackCollider = (CColliderBox3D*)m_Object->FindComponent("MeleeAttackCollider");
}

void CKnightDataComponent::OnInActiveMeleeAttackCollider()
{
	// TODO : Boss Knight - Particle, Cam Shake

	m_MeleeAttackCollider->Enable(false);
}

void CKnightDataComponent::OnHitMeleeAttack(const CollisionResult& Result)
{
	// TODO : Boss Knight - Player Hit ó��
}

void CKnightDataComponent::OnLookPlayerMove(float DeltaTime)
{
	// �÷��̾� �������� ȸ�� ������ŭ ȸ��
	float AnglePlayer = GetAnglePlayer();

	CGameObject* MyObj = m_Object;

	if (abs(AnglePlayer) < m_Data.RotateSpeedPerSec)
	{
		MyObj->AddWorldRotationY(-AnglePlayer * DeltaTime);
	}
	else
	{
		MyObj->AddWorldRotationY(m_Data.RotateSpeedPerSec * DeltaTime);
	}

	// Walk Speed �ι�� �̵�
	MyObj->AddWorldPosByLocalAxis(AXIS::AXIS_Z, -1.f * m_Data.MoveSpeed * 2.f);

	// ���� ���� �� ���� ����
	Vector3 FinalZAxis = ToPlayer();
	m_FinalTraceZAxis = FinalZAxis;
	m_FinalTraceZAxis.Normalize();
}

// ���� ����, �÷��̾���� ������ ������ �̾��, ������ �Ǵ��ϴ� �Լ�
void CKnightDataComponent::OnEndAnimJudgeContinueAttack()
{
	// �ִϸ��̼� ���� ���� �÷��̾ ���� ���� ���� ���� ���� ���
	if (IsPlayerInMeleeAttackRange())
	{
		// ���� �̾�� ���·�
		SetContinueAttack(true);

		// ���� ���� ���� �Ǵ�
		float AnglePlayer = GetAnglePlayer();

		// �÷��̾���� ������ ���� ������ ���
		if (abs(AnglePlayer) <= 90.f)
		{
			SetAttackRot(Knight_Attack_Rot_Type::Front);
			return;
		}

		CGameObject* Player = m_Object->GetScene()->GetPlayerObject();

		if (!Player)
		{
			return;
		}

		bool IsPlayerLeft = IsPlayerLeftBasedInLookDir();

		if (IsPlayerLeft)
		{
			SetAttackRot(Knight_Attack_Rot_Type::CCW);
		}
		else
		{
			SetAttackRot(Knight_Attack_Rot_Type::CW);
		}
	}
	else
	{
		// �ĵ����� ���·� ��ȯ
		SetPostAttckDelaying(true);
		SetContinueAttack(false);
	}

}

void CKnightDataComponent::OnActiveMeleeAttackCollider()
{
	m_MeleeAttackCollider->Enable(true);
}
