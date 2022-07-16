#pragma once

#include "ObjectComponent.h"
//#include "../Scene/Scene.h"
//#include "../Scene/SceneManager.h"

class CNavAgent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CNavAgent();
	CNavAgent(const CNavAgent& com);
	virtual ~CNavAgent();

private:
	CSharedPtr<class CSceneComponent> m_UpdateComponent;
	std::list<Vector3>		m_PathList;
	float					m_MoveSpeed;
	bool					m_ApplyNavMesh;
	bool					m_PathFindStart;
	int						m_OccupyPolygonIndex;

public:
	void SetUpdateComponent(class CSceneComponent* UpdateComponent);
	bool Move(const Vector3& EndPos);
	// ��ã�⸦ ���� �ʰ�, NavMesh������ �����̰�, NavMesh�ȿ����� �����̴� ��� ������Ʈ���� �� �Լ����� �̿��ؼ� �������� �Ѵ�
	bool MoveOnNavMesh(const Vector3 EndPos);

	void SetMoveSpeed(float Speed)
	{
		m_MoveSpeed = Speed;
	}

	void SetPathFindStart(bool Start)
	{
		m_PathFindStart = Start;
	}

	bool GetPathFindStart()	const
	{
		return m_PathFindStart;
	}

	void SetApplyNavMesh(bool Apply)
	{
		m_ApplyNavMesh = Apply;
	}

	bool GetApplyNavMesh()	const
	{
		return m_ApplyNavMesh;
	}

public:
	void AddTargetPos(const Vector3& TargetPos)
	{
		m_PathList.push_back(TargetPos);
	}

	void ClearPathList()
	{
		m_PathList.clear();
	}

	bool IsEmptyPathList()	const
	{
		return m_PathList.empty();
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CNavAgent* Clone();
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File);
	virtual bool LoadOnly(FILE* File);


private:
	void PathResult(const std::list<Vector3>& PathList);


public:
	void FillPathList(const std::list<Vector3>& PathList);
	void AddPath(const Vector3& EndPos);
	bool FindPath(class CSceneComponent* OwnerComponent, const Vector3& End);
	bool CheckStraightPath(const Vector3& StartPos, const Vector3& EndPos, std::vector<Vector3>& vecPath);
};

