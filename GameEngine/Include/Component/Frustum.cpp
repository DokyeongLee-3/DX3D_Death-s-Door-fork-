#include "Frustum.h"

CFrustum::CFrustum()
{
	// ����ü�� �����ϴ� 8���� ���� �̸� �����صд�.
	m_Pos[0] = Vector3(-1.f, 1.f, 0.f); // ���� ���
	m_Pos[1] = Vector3(-1.f, 1.f, 0.f); // ������ ���
	m_Pos[2] = Vector3(-1.f, -1.f, 0.f); // ���� �ϴ�
	m_Pos[3] = Vector3(1.f, -1.f, 0.f); // ������ �ϴ�

	// �Ĺ� 4���� ��
	m_Pos[4] = Vector3(-1.f, 1.f, 1.f); // ���� ���
	m_Pos[5] = Vector3(-1.f, 1.f, 1.f); // ������ ���
	m_Pos[6] = Vector3(-1.f, -1.f, 1.f); // ���� �ϴ�
	m_Pos[7] = Vector3(1.f, -1.f, 1.f); // ������ �ϴ�
}

CFrustum::~CFrustum()
{
}

void CFrustum::Update(Matrix matVP)
{
	// �츮�� ��������� World ���������� ����� �ʿ��ϴ�

	// ���� ���� ���������� 8�� �� ��ġ�� ���� ���̴�.
	// matVP �� View  ��� * ���� ���
	// �̰��� ������� ���ؼ�, ���� ������ ����, ���� �������� �̵������ִ� ȿ���� �� �� �ִ�

	Vector3 Pos[8];

	matVP.Inverse();

	for (int i = 0; i < 8; ++i)
	{
		// �� ���� ���� ���� ������ �̵� �����ش�.
		Pos[i] = m_Pos[i].TransformCoord(matVP);
	}

	// ������ ���� �������ش�.
	// �ۿ��� ��� �������� �ٶ󺸴� ����, �츮�� �ü��̴�.
	// �׷���, �ݴ��, �ۿ��� ����, �� ����� ������ �ð� �������� �־�����
	// �ش� ���� �������Ͱ� �츮 ������ �ٶ� �� �ֵ��� ��������� �Ѵ�.
	// �Ʒ��� 6�� ������, ���� ���������� 6�� ��� ������ �� ���̴�.
	
	
	// Left
	m_Plane[(int)Frustum_Plane_Dir::Left] = XMPlaneFromPoints(Pos[4].Convert(), Pos[0].Convert(),
		Pos[2].Convert());

	// Right
	m_Plane[(int)Frustum_Plane_Dir::Right] = XMPlaneFromPoints(Pos[1].Convert(), Pos[5].Convert(),
		Pos[7].Convert());

	// Top --> �� ������ �ٶ󺸰� ��
	m_Plane[(int)Frustum_Plane_Dir::Top] = XMPlaneFromPoints(Pos[4].Convert(), Pos[5].Convert(),
		Pos[1].Convert());

	// Bottom --> �Ʒ� �������� �ٶ󺸰Բ�
	m_Plane[(int)Frustum_Plane_Dir::Bottom] = XMPlaneFromPoints(Pos[2].Convert(), Pos[3].Convert(),
		Pos[7].Convert());

	// Near --> ī�޶� ������ ���� �ٶ󺸰Բ�
	m_Plane[(int)Frustum_Plane_Dir::Near] = XMPlaneFromPoints(Pos[0].Convert(), Pos[1].Convert(),
		Pos[3].Convert());

	// Far --> Near �� ���� ���� ����� �ݴ�
	m_Plane[(int)Frustum_Plane_Dir::Far] = XMPlaneFromPoints(Pos[5].Convert(), Pos[4].Convert(),
		Pos[6].Convert());

}

bool CFrustum::FrustumInPoint(const Vector3& Point)
{
	for (int i = 0; i < (int)Frustum_Plane_Dir::Max; ++i)
	{
		// GetX �� ���ִ� ������, ���� ������ Vector �� x,y,z ���ҿ� ���Ƿ�, �� �߿��� �ϳ��� ���� �޾ƿ��� ���̴�.
		// �Ʒ��� ����, �ش� Plane�� Normal �� �ش� Point ���Ϳ� ������ ���� ���� ��.
		// ax + by + cz + d (d �� �������� �ش� �������� �Ÿ� / a,b,c �� �ش� Plane �� Normal Vector �� x,y,z �� / x,y,z �� ���� Point �� ��)
		// ���� ax + by + cz + d ��  0 ���� ũ��, �ش� Point �� ��� "��"�� �����Ѵٴ� �� --> ����ü �ۿ� �����Ѵٴ� �� -->  �Ⱥ��̴� �� --> Culling�� �ȴٴ� ��
		// == 0 �̶��, �ش� ��� �� ���� --> ���̴� ��
		// < 0 �̶��, ��� �Ʒ��� ���� --> ����ü ���ʿ� ���� --> ���̴� ��
		float Dot = XMVectorGetX(XMPlaneDotCoord(m_Plane[i].Convert(), Point.Convert()));

		if (Dot > 0.f)
			return true;
	}

	return false;
}

bool CFrustum::FrustumInSphere(const SphereInfo& Sphere)
{
	for (int i = 0; i < (int)Frustum_Plane_Dir::Max; ++i)
	{
		// ���� ���� ����, ���� �߽��� ���Ϳ� ����� �����Ѵ�.
		float Dot = XMVectorGetX(XMPlaneDotCoord(m_Plane[i].Convert(), Sphere.Center.Convert()));

		// ���� ���, �ش� �߽������� �������� �����༭, ���� ��� �Ʒ��� �����ϴ� ������ Dot �̶�� ����� ��������
		// �ش� �߽�������, ��� �������� �������� ���ؼ� �İ���
		// ��� ���� �����ϴ� ������ �Ǻ��� ���� �ִ�.
		// ��, (ax + by + cz + d) + (������) = c �� ���� �����ؾ� �Ѵ�.
		if (Dot > Sphere.Radius)
			return true;
	}
	return false;
}
