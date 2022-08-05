
#include "ClimbDownStart.h"
#include "../PlayerDataComponent.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Scene/Scene.h"


CClimbDownStart::CClimbDownStart()
{
	SetTypeID(typeid(CClimbDownStart).hash_code());
}

CClimbDownStart::CClimbDownStart(const CClimbDownStart& Node) :
	CActionNode(Node)
{
}

CClimbDownStart::~CClimbDownStart()
{
}

NodeResult CClimbDownStart::OnStart(float DeltaTime)
{

	CPlayerDataComponent* DataComp = dynamic_cast<CPlayerDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	DataComp->SetClimbingLadder(true);

	Vector3 PlayerPos = m_Object->GetWorldPos();

	// �÷��̾ ��ٸ��� �ٶ󺸵��� Rotation, ��ٸ� �߾ӿ� ������ x�� ����,  
	CGameObject* Ladder = m_Object->GetScene()->FindObject("Ladder");

	if (Ladder)
	{
		// �÷��̾ ��ٸ��� �ٶ󺸵��� Rotation, ��ٸ� �߾ӿ� ������ x�� ����,  
		Vector3 LadderZAxis = Ladder->GetWorldAxis(AXIS_Z);
		LadderZAxis.z *= 1.f;

		Vector3 PlayerZAxis = m_Object->GetWorldAxis(AXIS_Z);

		float DotProduct = PlayerZAxis.Dot(LadderZAxis);
		float Degree = 0.f;
		if (DotProduct >= -0.99999999f && DotProduct <= 0.99999999f)
		{
			Degree = RadianToDegree(acosf(DotProduct));

			Vector3 CrossVec = PlayerZAxis.Cross(LadderZAxis);

			if (CrossVec.y < 0.f)
				Degree *= -1.f;

			m_Object->AddWorldRotationY(Degree);
		}

		Vector3 LadderPos = Ladder->GetWorldPos();
		m_Object->SetWorldPos(LadderPos.x - 0.3f, PlayerPos.y * 0.58f + LadderPos.y * 0.42f, LadderPos.z + LadderZAxis.z);
	}


	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("PlayerLadderDown");

	return NodeResult::Node_True;
}

NodeResult CClimbDownStart::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CClimbDownStart::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}