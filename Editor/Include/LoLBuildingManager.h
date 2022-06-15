#pragma once

#include "GameInfo.h"

class CLoLBuildingManager
{

public:
	void Init();

private:
	class CGameObject* m_EnemyNexus;
	class CGameObject* m_FriendNexus;
	Vector3 m_FriendHomePos;
	Vector3 m_EnemyHomePos;

public:
	void SetFriendHomePos(const Vector3& Pos);
	void SetEnemyHomePos(const Vector3& Pos);
	const Vector3& GetFriendHomePos()	const;
	const Vector3& GetEnemyHomePos()	const;
	void SetEnemyNexus(class CGameObject* Nexus);
	void SetFriendNexus(class CGameObject* Nexus);
	class CGameObject* GetEnemyNexus()	const;
	class CGameObject* GetFriendNexus()	const;

	// �ش� ���ο� ���� ���濡 �ִ� �ǹ��� ����
	CGameObject* GetFrontmostBuilding(LoLLine Line, bool Enemy = false);

private:
	class CGameObject* GetBuildingFromLine(LoLLine Line, class CGameObject* NexusObj) const;

	DECLARE_SINGLE(CLoLBuildingManager)
};

