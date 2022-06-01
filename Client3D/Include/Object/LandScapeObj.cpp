
#include "LandScapeObj.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Device.h"
#include "Player.h"


CLandScapeObj::CLandScapeObj()	:
	m_FrameCount(0)
{
	SetTypeID<CLandScapeObj>();
}

CLandScapeObj::CLandScapeObj(const CLandScapeObj& obj) :
	CGameObject(obj)
{
	m_LandScape = (CLandScape*)FindComponent("LandScape");
}

CLandScapeObj::~CLandScapeObj()
{
}

bool CLandScapeObj::Init()
{
	m_LandScape = CreateComponent<CLandScape>("LandScape");

	m_LandScape->CreateLandScape("LandScape", 129, 129,
		TEXT("LandScape/height1.bmp"));

	m_LandScape->AddMaterial("LandScape");

	m_LandScape->SetDetailLevel(30.f);
	m_LandScape->SetSplatCount(4);


	return true;
}

void CLandScapeObj::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CLandScapeObj::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	///// ���� Picking Test /////
	TerrainPicking();
}

CLandScapeObj* CLandScapeObj::Clone()
{
	return new CLandScapeObj(*this);
}

void CLandScapeObj::TerrainPicking()
{
	++m_FrameCount;

	if (CInput::GetInst()->GetMouseLButtonClick() && m_FrameCount > 20)
	{
		CCameraComponent* Current = m_Scene->GetCameraManager()->GetCurrentCamera();

		Matrix ProjMat = Current->GetProjMatrix();
		Matrix ViewMat = Current->GetViewMatrix();

		Ray ray = CInput::GetInst()->GetRay(ViewMat);
		Vector3 RayDir = ray.Dir;
		Vector3 RayStartPos = ray.Pos;

		float Step = 0.05f;

		while (true)
		{
			Vector3 Point_I1 = RayStartPos;
			Vector3 Point_I2 = RayStartPos + RayDir * Step;

			float Height1 = m_LandScape->GetHeight(Point_I1);
			float Height2 = m_LandScape->GetHeight(Point_I2);

			Vector3 ResultPos;

			if (m_LandScape->CheckInArea(Point_I1, Point_I2, ResultPos))
			{
				m_FrameCount = 0;

				CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();
				//Player->SetWorldPos((Point_I1.x + Point_I2.x) / 2.f, 0.f, (Point_I1.z + Point_I2.z) / 2.f);
				Player->SetWorldPos(ResultPos.x, ResultPos.y, ResultPos.z);
				break;
			}

			// LandScape �ۿ� Ŭ���ϸ� ��ŷ ���� ã�� �ʵ��� ���� ó��
			if (Point_I1.x < m_LandScape->GetMin().x && ray.Dir.x <= 0.f)
				return;

			if (Point_I1.z < m_LandScape->GetMin().z && ray.Dir.z <= 0.f)
				return;

			if (Point_I1.x > m_LandScape->GetMax().x && ray.Dir.x >= 0.f)
				return;

			if (Point_I1.z > m_LandScape->GetMax().z && ray.Dir.z >= 0.f)
				return;

			else
			{
				RayStartPos = Point_I2;
			}
		}
	}
}

