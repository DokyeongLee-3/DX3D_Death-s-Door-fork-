
#include "ReadyToShoot.h"
#include "RotateAttackDirectionNode.h"
#include "Component/Node/CompositeNode.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/BehaviorTree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Input.h"


CReadyToShoot::CReadyToShoot()	:
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

	std::string ObjectName = m_Object->GetName();

	std::string SequenceName = ObjectName + "_" + "SkillQ";

	if (m_AnimationMeshComp)
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

	m_Object->SetNoInterrupt(true);
	m_CallStart = true;

	// Nav Agent�� PathList�� ����
	CNavAgent* Agent = m_Object->GetNavAgent();

	if (Agent)
		Agent->ClearPathList();




	CRotateAttackDirectionNode* Node = (CRotateAttackDirectionNode*)((CCompositeNode*)m_Parent->GetParent()->GetParent())->FindChildByType<CRotateAttackDirectionNode>();

	Vector3 PickingPoint = Node->GetPickingPoint();

	CCameraComponent* CurrentCamera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Vector3 OriginCamPos = CurrentCamera->GetWorldPos();
	CSceneManager::GetInst()->GetScene()->SetOriginCamPos(OriginCamPos);

	m_CameraDestPos = (PickingPoint + OriginCamPos) / 2.f;
	m_CameraMoveDir = (m_CameraDestPos - OriginCamPos);
	m_CameraMoveDir.y = 0.f;
	m_CameraMoveDir.Normalize();

	return NodeResult::Node_True;
}

NodeResult CReadyToShoot::OnUpdate(float DeltaTime)
{
	if (!m_CameraMoveEnd)
	{
		m_CameraMoveEnd = CSceneManager::GetInst()->GetScene()->CameraMove(m_CameraMoveDir, m_CameraDestPos, 50.f, DeltaTime);

		bool RButtonUp = CInput::GetInst()->GetMouseRButtonUp();

		// ī�޶� ȭ���� ��ǥ �������� ������ �̵��ϱ� ���� ���콺RButton �� ����
		if (RButtonUp)
		{
			m_CallStart = false;
			m_Owner->SetCurrentNode(((CCompositeNode*)m_Parent->GetParent())->GetChild(1));
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
			m_CallStart = false;
			m_Owner->SetCurrentNode(((CCompositeNode*)m_Parent)->GetChild(1));
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

	return NodeResult::Node_True;
}

NodeResult CReadyToShoot::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}