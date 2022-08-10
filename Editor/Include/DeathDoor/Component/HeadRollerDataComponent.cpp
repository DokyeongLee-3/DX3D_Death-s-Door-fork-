#include "HeadRollerDataComponent.h"
#include "../DataManager.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"
#include "PlayerDataComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/AnimationMeshComponent.h"

CHeadRollerDataComponent::CHeadRollerDataComponent()
{
	SetTypeID<CHeadRollerDataComponent>();
}

CHeadRollerDataComponent::CHeadRollerDataComponent(const CHeadRollerDataComponent& com)	:
	CMonsterDataComponent(com)
{
}

CHeadRollerDataComponent::~CHeadRollerDataComponent()
{
}

void CHeadRollerDataComponent::Start()
{
	CMonsterDataComponent::Start();

	m_Data = CDataManager::GetInst()->GetObjectData("HeadRoller");

	m_HitBox->AddCollisionCallback(Collision_State::Begin, this, &CHeadRollerDataComponent::OnCollide);

	CAnimationSequenceInstance* AnimInst = m_AnimMesh->GetAnimationInstance();
	AnimInst->SetEndFunction("Stun", this, &CHeadRollerDataComponent::OnStunEnd);
	AnimInst->AddNotify("RollStart", "OnRollStart", 0, this,&CHeadRollerDataComponent::OnRollReadyStart);
	AnimInst->SetEndFunction("RollStart", this, &CHeadRollerDataComponent::OnRollStart);
}

void CHeadRollerDataComponent::SetIsHit(bool Enable)
{
	CMonsterDataComponent::SetIsHit(Enable);

	if (Enable)
	{
		Vector3 PlayerZAxis = m_Scene->GetPlayerObject()->GetWorldAxis(AXIS::AXIS_Z);
		Vector3 MyZAxis = m_Object->GetWorldAxis(AXIS::AXIS_Z);

		float Dot = MyZAxis.Dot(PlayerZAxis);

		// �÷��̾ ������ ���� ��� ��Ʈ���� �ʴ´�.
		if (Dot > 0)
		{
			m_IsHit = true;
		}
		else
		{
			m_IsHit = false;
		}
	}
	else
	{
		m_IsHit = Enable;
	}
}

void CHeadRollerDataComponent::DecreaseHP(int Amount)
{
 //	Vector3 PlayerZAxis = m_Scene->GetPlayerObject()->GetWorldAxis(AXIS::AXIS_Z);
 //	Vector3 MyZAxis = m_Object->GetWorldAxis(AXIS::AXIS_Z);
 //
 //	float Dot = MyZAxis.Dot(PlayerZAxis);
 //
 //	// �÷��̾ ������ ���� ��� ��Ʈ���� �ʴ´�.
 //	if (Dot > 0)
 //	{
		CMonsterDataComponent::DecreaseHP(Amount);
	// }
}

void CHeadRollerDataComponent::OnRollReadyStart()
{
	m_LookPlayer = true;
}

void CHeadRollerDataComponent::OnRollStart()
{
	CAnimationSequenceInstance* AnimInst = m_AnimMesh->GetAnimationInstance();
	AnimInst->ChangeAnimation("RollLoop");

	m_LookPlayer = false;
	m_MoveZ = true;
	m_CurMoveSpeed = m_Data.MoveSpeed;
	m_Rolling = true;
	m_Stun = false;
}

void CHeadRollerDataComponent::OnRollEnd()
{
	SetCurrentNodeNull();

	Vector3 WorldPos = m_Object->GetWorldPos();
	Vector3 PrevPos = m_Object->GetPrevFramePos();

	m_Object->SetWorldPos(PrevPos);

	if (PrevPos == Vector3::Zero)
	{
		m_Object->SetWorldPos(WorldPos);
	}

	m_MoveZ = false;
	m_CurMoveSpeed = 0.f;
	m_Rolling = false;
	m_Stun = true;
}

void CHeadRollerDataComponent::OnStunEnd()
{
	SetCurrentNodeNull();

	m_Stun = false;
}

void CHeadRollerDataComponent::OnCollide(const CollisionResult& Result)
{
	if (m_Rolling)
	{
		if (m_PlayerData && Result.Dest->GetGameObject() == m_PlayerData->GetGameObject())
		{
			m_PlayerData->DecreaseHP(1);
		}

		OnRollEnd();
	}
}
