#pragma once

#include "GameInfo.h"

// ��ž, �ؼ���, ��������� ��� ��ġ�� �˰� ������
// �̴Ͼ�, ���۸�, ��, �ٷ��� ���� �ֱ⿡ ����
class CGameManager
{

private:			   
	class CGameObject* m_FriendTopTurret1Order;
	class CGameObject* m_FriendTopTurret2Order;
	class CGameObject* m_FriendTopTurret3Order;
	class CGameObject* m_FriendMidTurret1Order;
	class CGameObject* m_FriendMidTurret2Order;
	class CGameObject* m_FriendMidTurret3Order;
	class CGameObject* m_FriendBottomTurret1Order;
	class CGameObject* m_FriendBottomTurret2Order;
	class CGameObject* m_FriendBottomTurret3Order;
	class CGameObject* m_EnemyTurretTop1Order;
	class CGameObject* m_EnemyTurretTop2Order;
	class CGameObject* m_EnemyTurretTop3Order;
	class CGameObject* m_EnemyTurretMid1Order;
	class CGameObject* m_EnemyTurretMid2Order;
	class CGameObject* m_EnemyTurretMid3Order;
	class CGameObject* m_EnemyTurretBottom1Order;
	class CGameObject* m_EnemyTurretBottom2Order;
	class CGameObject* m_EnemyTurretBottom3Order;

public:
	void SetTurret(class CGameObject* Turret, LoLLine line, int Order, bool Enemy);
	void GetTurret(LoLLine line, int Order, bool Enemy);

public:
	const Vector3 GetNearTurretPos(const Vector3& Pos);

	DECLARE_SINGLE(CGameManager)
};

