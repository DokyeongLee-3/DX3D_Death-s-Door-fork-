
#include "ReadyToShoot.h"
#include "RotateAttackDirectionNode.h"
#include "Component/Node/CompositeNode.h"
#include "Component/AnimationMeshComponent.h"
#include "../../Component/PlayerDataComponent.h"
#include "../../Component/PlayerBowComponent.h"
#include "../../Component/PlayerBombComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "../GameStateComponent.h"
#include "Component/BehaviorTree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Input.h"


CReadyToShoot::CReadyToShoot() :
	m_CameraMoveEnd(false)
{
	SetTypeID(typeid(CReadyToShoot).hash_code());
}

CReadyToShoot::CReadyToShoot(const CReadyToShoot& Node) :
	CActionNode(Node)
{
}

CReadyToShoot::~CReadyToShoot()
{
}

NodeResult CReadyToShoot::OnStart(float DeltaTime)
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	m_PlayerDataComp = dynamic_cast<CPlayerDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	Player_Ability Ability = m_PlayerDataComp->GetPlayerAbility();

	std::string ObjectName = m_Object->GetName();
	std::string SequenceName;

	if (Ability == Player_Ability::Arrow)
	{
		SequenceName = ObjectName + "Arrow";
	}

	else if (Ability == Player_Ability::Hook)
	{
		SequenceName = ObjectName + "Hook";
	}

	else if (Ability == Player_Ability::Bomb)
	{
		CPlayerBombComponent* BombComp = m_Object->FindComponentFromType<CPlayerBombComponent>();

		CGameObject* Bomb = BombComp->GetBomb();

		// �̹� Lift & Shoot�� �ѹ� �̻� �ߴµ� BombComp�� m_Bomb�� nullptr�� �ƴϸ�
		// �� m_Bomb�� �̹� ��ź ����Ʈ�� �������̶�� �ǹ��̹Ƿ� Lift & Shoot�� ���� �ʴ´�
		if (!BombComp->IsShootFirstTime() && Bomb)
		{
			m_IsEnd = true;
			BombComp->SetCancleAction(true);

			CProjectileComponent* Proj = Bomb->FindObjectComponentFromType<CProjectileComponent>();
			if (Proj)
			{
				bool Shoot = Proj->IsShoot();
				// Shoot�� �ȵ�ä�� �Ӹ����� �����ִٸ�
				if (!Shoot)
				{
					Bomb->Reset();
					BombComp->ResetInfo();
				}
			}

			return NodeResult::Node_True;
		}

		else
		{
			SequenceName = ObjectName + "Bomb";
			BombComp->SetCancleAction(false);
		}
	}

	if (m_AnimationMeshComp && Ability != Player_Ability::None)
	{
		CAnimationSequenceInstance* Instance = m_AnimationMeshComp->GetAnimationInstance();

		// AnimationSequenceInstance���� m_ChangeTime�� �ֱ� ������, �ٷ� �ִϸ��̼��� �ٲ�°� �ƴ϶� m_ChangeTime�� ���ļ� �ִϸ��̼��� �ٲ�µ�
		// CurrentAnimation�� ReadyToShoot�̶� ChangeAnimation�ص� �ƹ��͵� ���ϰ� �ٷ� return�ǰ� ChangeAnimation�� Idle���� �������� �����Ǿ� �ִ� ���¿���
		// �� ���¿��� OnUpdate�� ���� Idle�� ������ Sequence�� �ٲ� ���¿��� ReadyToShoot::OnUpdate���� CurrentAnimation�� ReadyToShoot�̶�� ����������
		// CurrentAnimation�� �ᱹ Idle�� �� ���̰�, ChangeAnimation�� nullptr�� �� ���̹Ƿ� OnUpdate�� else���� ��� ������ �Ǵ� ������ ����Ƿ�
		// �̷� ��Ȳ������ KeepAnimation�� ȣ���ؼ� ChangeAnimation�� ����� CurrentAnimation�� Attack���� ���������ش�
		if (Instance->GetChangeAnimation() && Instance->GetCurrentAnimation()->GetName() == SequenceName)
			Instance->KeepCurrentAnimation();

		else
			Instance->ChangeAnimation(SequenceName);
	}

	//m_Object->SetNoInterrupt(true);
	m_CallStart = true;


	CRotateAttackDirectionNode* Node = (CRotateAttackDirectionNode*)((CCompositeNode*)m_Parent->GetParent()->GetParent())->FindChildByType<CRotateAttackDirectionNode>();

	Vector3 PickingPoint = Node->GetPickingPoint();

	CCameraComponent* CurrentCamera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Vector3 ObjectPos = m_Object->GetWorldPos();
	Vector3 OriginCamPos = CurrentCamera->GetWorldPos();
	CSceneManager::GetInst()->GetScene()->SetOriginCamPos(OriginCamPos);

	m_CameraDestPos =  OriginCamPos + (PickingPoint - ObjectPos) / 2.3f;
	m_CameraDestPos.y = OriginCamPos.y;
	m_CameraMoveDir = PickingPoint - ObjectPos;
	m_CameraMoveDir.y = 0.f;
	m_CameraMoveDir.Normalize();


	return NodeResult::Node_True;
}

NodeResult CReadyToShoot::OnUpdate(float DeltaTime)
{
	Player_Ability Ability = m_PlayerDataComp->GetPlayerAbility();

	if (Ability == Player_Ability::None)
		return NodeResult::Node_True;

	if (Ability == Player_Ability::Arrow)
	{
		CPlayerBowComponent* BowComp = m_Object->FindComponentFromType<CPlayerBowComponent>();

		if (BowComp)
			BowComp->ShowBow(m_CameraMoveDir);
	}


	if (!m_CameraMoveEnd)
	{
		CPlayerBombComponent* BombComp = m_Object->FindComponentFromType<CPlayerBombComponent>();

		if (Ability == Player_Ability::Bomb && BombComp->IsCancleAction())
		{

			m_IsEnd = true;
			// OnStart���� CancleAction�� True�� ��������� ī�޶� �̵��� ���� �ʰ� �ٷ� ����
			// Node_False�� �����ؼ� Sibling Node�� ShootNode�� �Ѿ�� �ʴ´�

			// CancleAction�� �ٽ� ������� false�� ����
			BombComp->SetCancleAction(false);
			return NodeResult::Node_False;
		}

		m_CameraMoveEnd = CSceneManager::GetInst()->GetScene()->CameraMove(m_CameraMoveDir, m_CameraDestPos, 50.f, DeltaTime);

		bool RButtonUp = CInput::GetInst()->GetMouseRButtonUp();

		// ī�޶� ȭ���� ��ǥ �������� ������ �̵��ϱ� ���� ���콺RButton �� ����
		if (RButtonUp)
		{
			if (Ability == Player_Ability::Bomb)
			{
				// ī�޶� �̵��� ���� �ȳ����µ� RButton�� ����
				CGameObject* Bomb = BombComp->GetBomb();
				if (Bomb && !BombComp->IsEmptyLiftPathQueue())
				{
					Bomb->Reset();
					BombComp->ResetInfo();
				}
			}

			m_CallStart = false;
			m_Owner->SetCurrentNode(((CCompositeNode*)m_Parent->GetParent())->GetChild(1));
			m_IsEnd = false;
			m_CameraMoveEnd = false;

			return NodeResult::Node_True;
		}

		else
		{
			m_Owner->SetCurrentNode(this);

			return NodeResult::Node_Running;
		}
	}

	else
	{
		bool RButtonUp = CInput::GetInst()->GetMouseRButtonUp();
		if (RButtonUp)
		{
			if (Ability == Player_Ability::Bomb)
			{
				CPlayerBombComponent* BombComp = m_Object->FindComponentFromType<CPlayerBombComponent>();
				CGameObject* Bomb = BombComp->GetBomb();

				// Bomb Lift�� ���� �ȳ����µ� RButton�� ����
				if (Bomb && !BombComp->IsEmptyLiftPathQueue())
				{
					Bomb->Reset();
					BombComp->ResetInfo();
					BombComp->SetClearBomb(true);
				}
			}

			m_CallStart = false;
			m_Owner->SetCurrentNode(((CCompositeNode*)m_Parent)->GetChild(1));
			m_IsEnd = false;
			m_CameraMoveEnd = false;

			return NodeResult::Node_True;
		}

		else
		{
			m_Owner->SetCurrentNode(this);
			return NodeResult::Node_Running;
		}
	}


}

NodeResult CReadyToShoot::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);
	m_IsEnd = false;
	m_CallStart = false;

	return NodeResult::Node_True;
}

NodeResult CReadyToShoot::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}