#include "GruntCommonDataComponent.h"
#include "GameBehaviorTree.h"
#include "GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../DataManager.h"
#include "Component/CameraComponent.h"
#include "GameObject/GameObject.h"
#include "ObjectPool.h"

CGruntCommonDataComponent::CGruntCommonDataComponent() :
	m_StartAct(false),
	m_SpawnEnable(true) // SpawnNode ���� Animation �� ��� ����� ����, false �� ó������ ���̴�.
{
	SetTypeID<CGruntCommonDataComponent>();
}

CGruntCommonDataComponent::CGruntCommonDataComponent(const CGruntCommonDataComponent& com) :
	CMonsterDataComponent(com)
{
}

CGruntCommonDataComponent::~CGruntCommonDataComponent()
{
}

void CGruntCommonDataComponent::Start()
{
	CMonsterDataComponent::Start();

	m_HitBox->SetExtent(1.5f, 2.5f, 1.5f);

	m_Data = CDataManager::GetInst()->GetObjectData("BossBetty");

	// MoveSpeed
	m_OriginMoveSpeed = m_Data.MoveSpeed;
	m_CurMoveSpeed = m_Data.MoveSpeed;
}

void CGruntCommonDataComponent::OnResetOriginalMoveSpeed()
{
	m_CurMoveSpeed = m_OriginMoveSpeed;
}
