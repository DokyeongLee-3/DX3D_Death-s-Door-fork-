#pragma once

#include "../GameInfo.h"

class CCollisionSection
{
	friend class CSceneCollision;

private:
	CCollisionSection();
	~CCollisionSection();

private:
	std::vector<class CColliderComponent*>	m_vecCollider;
	std::vector<class CColliderComponent*>	m_vecPrevCollider;
	Vector3		m_SectionSize;
	Vector3		m_SectionTotalSize;
	Vector3		m_Min;
	Vector3		m_Max;
	int			m_IndexX;
	int			m_IndexY;
	int			m_IndexZ;
	int			m_Index;

public:
	void Init(int IndexX, int IndexY, int IndexZ, int Index, 
		const Vector3& Min, const Vector3& Max,
		const Vector3& SectionSize,
		const Vector3& SectionTotalSize);
	void Clear();
	void ClearPrevCollider();
	void AddCollider(class CColliderComponent* Collider);
	void AddPrevCollider(class CColliderComponent* Collider);
	void Collision(float DeltaTime);
	class CColliderComponent* CollisionMouse(bool Is2D, float DeltaTime);

public:
	size_t GetColliderSize()	const
	{
		return m_vecCollider.size();
	}

	size_t GetPrevColliderSize()	const
	{
		return m_vecPrevCollider.size();
	}

	class CColliderComponent* GetCollider(int Idx)	const;
	class CColliderComponent* GetPrevCollider(int Idx)	const;

private:
	static int SortY(const void* Src, const void* Dest);
};

