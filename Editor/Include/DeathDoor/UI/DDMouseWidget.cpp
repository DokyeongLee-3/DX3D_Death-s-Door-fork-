#include "DDMouseWidget.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"
#include "Component/CameraComponent.h"
#include "Device.h"

CDDMouseWidget::CDDMouseWidget()
{
	SetTypeID<CDDMouseWidget>();
}

CDDMouseWidget::~CDDMouseWidget()
{
}

void CDDMouseWidget::Render()
{
	if (m_Owner->GetViewport())
	{
		CScene* Scene = m_Owner->GetViewport()->GetScene();
		CGameObject* PlayerObj = Scene->GetPlayerObject();

		if (PlayerObj)
		{
			if (Scene)
			{
				CCameraComponent* CurCam = Scene->GetCameraManager()->GetCurrentCamera();

				Matrix matView = CurCam->GetViewMatrix();
				Matrix matProj = CurCam->GetProjMatrix();

				Matrix matVP = matView * matProj;
				Vector3 PlayerPos = PlayerObj->GetWorldPos();

				// �÷��̾��� ȭ����������� ��ġ�� ���Ѵ�.
				Vector3 PlayerSceenPos = PlayerPos.TransformCoord(matVP);
				PlayerSceenPos.x = PlayerSceenPos.x / PlayerSceenPos.z;
				PlayerSceenPos.y = PlayerSceenPos.y / PlayerSceenPos.z;

				Resolution RS = CDevice::GetInst()->GetResolution();
				PlayerSceenPos.x *= (float)RS.Width;
				PlayerSceenPos.y *= (float)RS.Height;
				PlayerSceenPos.z = 0.f;

				// �÷��̾��� ȭ������� ��ġ�κ��� ���콺�� ���ϴ� ���͸� ���Ѵ�.
				Vector3 ToMouse = Vector3(m_Pos.x, m_Pos.y, 0.f) - PlayerSceenPos;
				ToMouse.Normalize();

				// (0,1,0)�� �̷�� ������ ȸ�������� �Ѵ�.
				m_Angle = Vector3::Axis[AXIS::AXIS_Y].Angle(ToMouse);
			}
		}
	}

	CWidget::Render();
}
