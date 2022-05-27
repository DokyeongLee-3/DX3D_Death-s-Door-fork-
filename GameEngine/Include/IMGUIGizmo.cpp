#include "IMGUIGizmo.h"
#include "ImGuizmo.h"
#include "Scene/SceneManager.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Component/CameraComponent.h"

CIMGUIGizmo::CIMGUIGizmo() :
	m_Object(nullptr),
	m_Using(false)
{
	m_Operation = ImGuizmo::OPERATION::TRANSLATE;
	m_Mode = ImGuizmo::MODE::WORLD;
	m_matWorld.Identity();
}

CIMGUIGizmo::~CIMGUIGizmo()
{
}

bool CIMGUIGizmo::Init()
{
	CIMGUIWidget::Init();

	return true;
}

void CIMGUIGizmo::Render()
{
	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Matrix matScale, matRot, matTrans;

	matScale.Identity();
	matRot.Identity();
	matTrans.Identity();

	m_matWorld = matScale * matRot * matTrans;

	if (m_Object && m_Object->GetRootComponent())
	{
		m_matWorld = m_Object->GetWorldMatrix();
	}

	Matrix matView, matProj;

	matView = Camera->GetViewMatrix();
	matProj = Camera->GetProjMatrix();

 	ImGuizmo::Manipulate((float*)&matView, (float*)&matProj, m_Operation,
 		m_Mode, (float*)&m_matWorld);

	if (ImGuizmo::IsUsing() && m_Object)
	{
		// Gizmo�� ��� ���϶��� Transform�� World Matrix�� ���� �ִ� ������� �����Ѵ�.
		m_Object->SetTransformByWorldMatrix(m_matWorld);
		m_Using = true;
	}
	else
	{
		if (m_Using)
		{
			// Gizmo����� ������, �ٽ� Scale, Pos, Rot ������ World Matirx�� �����ϰ� �������´�.
			m_Object->SetUpdateByMat(false);
			m_Object->DecomposeWorld();
			m_Using = false;
		}
	}
}

