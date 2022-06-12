#pragma once

#include "GameInfo.h"

// ��ž, �ؼ���, ��������� ��� ��ġ�� �˰� ������
// �̴Ͼ�, ���۸�, ��, �ٷ��� ���� �ֱ⿡ ����
class CGameManager
{

private:
	std::vector<Vector3> m_vecTurretPos;
	std::vector<Vector3> m_vecNexusPos;
	//std::vector<Vector3> 

public:
	const Vector3 GetNearTurretPos(const Vector3& Pos);

	DECLARE_SINGLE(CGameManager)
};

