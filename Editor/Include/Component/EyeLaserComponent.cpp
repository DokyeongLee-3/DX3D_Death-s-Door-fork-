
#include "EyeLaserComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CEyeLaserComponent::CEyeLaserComponent()	:
	m_TriggerHitCount(0),
	m_Player(nullptr),
	m_AnimComp(nullptr),
	m_WakeEnd(false)
{
	SetTypeID<CEyeLaserComponent>();
	m_ComponentType = Component_Type::SceneComponent;

	m_CurrentLaserDir = Vector3(0.f, 0.f, 1.f);

	m_LayerName = "Transparency";
	m_LaserPlaneMesh1 = CResourceManager::GetInst()->FindMesh("PlaneMesh");

	m_Shader = CResourceManager::GetInst()->FindShader("LaserShader");
	
}

CEyeLaserComponent::CEyeLaserComponent(const CEyeLaserComponent& com)	:
	CSceneComponent(com)
{
}

CEyeLaserComponent::~CEyeLaserComponent()
{
}

void CEyeLaserComponent::SetWakeEnd()
{
	m_WakeEnd = true;
	m_AnimComp->GetAnimationInstance()->ChangeAnimation("EyeLaser_Idle");
}

void CEyeLaserComponent::Start()
{
	CSceneComponent::Start();

	m_Player = m_Object->GetScene()->GetPlayerObject();

	// Notify(ex. Wake �ִϸ��̼� ������ TrackPlayer �Լ� ȣ��) ����

	m_AnimComp = m_Object->FindComponentFromType<CAnimationMeshComponent>();

	if (m_AnimComp)
	{
		CAnimationSequenceInstance* Instance = m_AnimComp->GetAnimationInstance();
		
		Instance->ChangeAnimation("EyeLaser_Sleep");
		Instance->SetEndFunction<CEyeLaserComponent>("EyeLaser_Wake", this, &CEyeLaserComponent::SetWakeEnd);
	}
	
}

bool CEyeLaserComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CEyeLaserComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	CAnimationSequenceInstance* Instance = m_AnimComp->GetAnimationInstance();
	if (m_TriggerHitCount == 1 && !m_WakeEnd)
	{
		// EyeLaser�� ����� �ִϸ޴ϼ����� ��ȯ
		Instance->ChangeAnimation("EyeLaser_Wake");
	}

	if (m_TriggerHitCount == 1 && m_WakeEnd)
	{
		TrackPlayer(DeltaTime);
	}

	else if (m_TriggerHitCount == 4)
	{
		// EyeLaser�� �ı�
	}
}

void CEyeLaserComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CEyeLaserComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CEyeLaserComponent::Render()
{
	if (m_TriggerHitCount == 1 && m_WakeEnd)
	{
		CSceneComponent::Render();

		m_Shader->SetShader();

		m_LaserPlaneMesh1->Render();
	}
}

void CEyeLaserComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CEyeLaserComponent* CEyeLaserComponent::Clone()
{
	return new CEyeLaserComponent(*this);
}

bool CEyeLaserComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	return true;
}

bool CEyeLaserComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);

	return true;
}

bool CEyeLaserComponent::SaveOnly(FILE* File)
{
	return false;
}

bool CEyeLaserComponent::LoadOnly(FILE* File)
{
	return false;
}

void CEyeLaserComponent::TrackPlayer(float DeltaTime)
{
	m_WakeEnd = true; 

	// ���� �������� �ٶ󺸴� ������ �÷��̾ �����ϴ� ���� �ٶ󺸴� ����
	Vector3 LaserRot = Vector3(0.f, 0.f, 1.f);

	Matrix RotMatrix = m_Object->GetRootComponent()->GetTransform()->GetRotationMatrix();

	LaserRot = LaserRot.TransformCoord(RotMatrix);

	Vector3 PlayerPos = m_Player->GetWorldPos();
	Vector3 LaserPos = m_Object->GetWorldPos();

	Vector3 Dir = PlayerPos - LaserPos;
	Dir.Normalize();

	float Rad = Dir.Dot(LaserRot);
	float Degree = RadianToDegree(acosf(Rad));

	Vector3 CrossVector = Dir.Cross(LaserRot);

	// Player�� �������� �ٶ������, Player�� ��ġ�� ������ ��� ��ġ���� ���ʿ� ���� 
	if (CrossVector.y < 0.f)
	{
		m_Object->AddWorldRotationY(DeltaTime * 6.f);
	}

	// Player�� �������� �ٶ������, Player�� ��ġ�� ������ ��� ��ġ���� �����ʿ� ����
	else
	{
		m_Object->AddWorldRotationY(-DeltaTime * 6.f);
	}

	Vector3 LaserTopPos = GetWorldPos();;
	Dir = PlayerPos - Vector3(LaserTopPos.x, LaserPos.y + 1.5f, LaserPos.z);

	Vector3 UpDownVec = Vector3(0.f, Dir.y, Dir.z);
	UpDownVec.Normalize();

	Rad = m_CurrentLaserDir.Dot(UpDownVec);
	Degree = RadianToDegree(acosf(Rad));

	CrossVector = m_CurrentLaserDir.Cross(UpDownVec);

	// LaserEye�� -y �������� ���ؾ� �Ҷ�
	if (CrossVector.x > 0)
	{
		m_Object->AddWorldRotationX(DeltaTime * 3.f);
		m_LaserRotMatrix.Rotation(DeltaTime * 3.f, 0.f, 0.f);
	}

	// LaserEye�� +y �������� ���ؾ� �Ҷ�
	else
	{
		m_Object->AddWorldRotationX(-DeltaTime * 3.f);
		m_LaserRotMatrix.Rotation(-DeltaTime * 3.f, 0.f, 0.f);
	}

	m_CurrentLaserDir = m_CurrentLaserDir.TransformCoord(m_LaserRotMatrix);
	m_CurrentLaserDir.Normalize();
}

void CEyeLaserComponent::ChangeToWakeAnimation()
{
}

void CEyeLaserComponent::ChangeToDieAnimation()
{
}

void CEyeLaserComponent::ChangeToIdleAnimation()
{
}
