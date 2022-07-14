#pragma once

#include "../GameInfo.h"
#include "../Component/LandScape.h"
//#include "../ThreadQueue.h"
#include "NavigationThread3D.h"

class CNavigation3DManager
{
	friend class CScene;

private:
	CNavigation3DManager();
	~CNavigation3DManager();

private:
	class CScene* m_Scene;
	std::vector<CNavigationThread3D*>	m_vecNavigationThread;
	CSharedPtr<CLandScape>				m_NavData;
	CThreadQueue<NavResultData>			m_ResultQueue;
	class CLandScape* m_LandScape;
	class CNavMeshComponent* m_NavMeshComponent;
	int m_PlayerPolyIndex;

public:
	void SetPlayerPolyIndex(int Index)
	{
		m_PlayerPolyIndex = Index;
	}

	int GetPlayerPolyIndex()	const
	{
		return m_PlayerPolyIndex;
	}

	class CNavMeshComponent* GetNavMeshData() const;
	void SetNavMeshData(CNavMeshComponent* NavComp);



	void SetNavData(CLandScape* NavData);
	void AddNavResult(const NavResultData& NavData);
	void SetLandScape(class CLandScape* LandScape);
	class CLandScape* GetLandScape()	const;

public:
	float GetY(const Vector3& Pos);
	bool CheckPickingPoint(Vector3& OutPos);
	// 높이를 결과로 리턴
	bool CheckPlayerNavMeshPoly(float& Height);
	bool CheckNavMeshPickingPoint(Vector3& OutPos);

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);


	template <typename T, typename ComponentType>
	bool FindPath(T* Obj, void(T::* Func)(const std::list<Vector3>&),
		ComponentType* OwnerComponent, const Vector3& End)
	{
		if (m_vecNavigationThread.empty())
			return false;

		int	Count = m_vecNavigationThread[0]->GetWorkCount();
		int	WorkIndex = 0;

		size_t	Size = m_vecNavigationThread.size();

		for (size_t i = 1; i < Size; ++i)
		{
			if (Count > m_vecNavigationThread[i]->GetWorkCount())
			{
				Count = m_vecNavigationThread[i]->GetWorkCount();
				WorkIndex = (int)i;
			}
		}

		m_vecNavigationThread[WorkIndex]->AddWork<T>(Obj, Func, OwnerComponent, End);

		return true;
	}
};


