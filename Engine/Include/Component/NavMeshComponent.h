#pragma once

#include "SceneComponent.h"
#include "../Resource/ResourceManager.h"
#include "../Render/RenderManager.h"
#include "../Render/RenderState.h"

enum NAVIMESH_CELL_LIST_TYPE
{
	NCLT_NONE,
	NCLT_OPEN,
	NCLT_CLOSE
};

struct NavigationCell
{
	NAVIMESH_CELL_LIST_TYPE	Type;
	NavMeshPolygon Polygon;
	Vector3		Center;
	int			ParentIdx;
	float		G;
	float		H;
	float		Total;
	bool		Enable;

	void Clear()
	{
		Type = NCLT_NONE;
		ParentIdx = -1;
		G = -1.f;
		H = -1.f;
		Total = -1.f;
	}

	NavigationCell()
	{
		Type = NCLT_NONE;
		ParentIdx = -1;
		G = 0.f;
		H = 0.f;
		Total = 0.f;
		Enable = true;
	}

	NavigationCell operator + (const NavigationCell& cell)
	{
		NavigationCell	_cell;

		return _cell;
	}
};

class CNavMeshComponent :
    public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CNavMeshComponent();
	CNavMeshComponent(const CNavMeshComponent& com);
	virtual ~CNavMeshComponent();

protected:
	CSharedPtr<CNavMesh>	m_NavMesh;
	CSharedPtr<class CShader> m_Shader;

	bool m_DebugRender;
	CSharedPtr<CRenderState> m_WireFrameState;

	Vector3 m_PlayerSpawnPos;
	int m_PlayerSpawnPolyIndex;
	//int		m_CountX;
	//int		m_CountZ;
	//std::vector<NavMeshPolygon>		m_vecNavMeshPolygon;
	//Vector3 m_Min;
	//Vector3 m_Max;

	std::list<NavigationCell*> m_UseCellList;
	std::list<NavigationCell*> m_OpenList;
	std::list<NavigationCell*> m_CloseList;
	std::unordered_map<int, NavigationCell*> m_mapCell;

public:
	void SetPlayerSpawnPolyIndex(int Index)
	{
		m_PlayerSpawnPolyIndex = Index;
	}

	int GetPlayerSpawnPolyIndex()	const
	{
		return m_PlayerSpawnPolyIndex;
	}

	void SetPlayerSpawnPos(const Vector3& Pos)
	{
		m_PlayerSpawnPos = Pos;
	}

	const Vector3& GetPlayerSpawnPos()	const
	{
		return m_PlayerSpawnPos;
	}

	CNavMesh* GetNavMesh()	const
	{
		return m_NavMesh;
	}

	void SetDebugRender(bool Debug)
	{
		m_DebugRender = Debug;

		if (m_DebugRender)
		{
			m_Shader = CResourceManager::GetInst()->FindShader("Standard3DWireFrameShader");
			m_WireFrameState = CRenderManager::GetInst()->FindRenderState("Wireframe");
		}
	}

	bool GetDebugRender() const
	{
		return m_DebugRender;
	}

public:
	void SetNavMesh(const std::string& Name);
	void SetNavMesh(class CNavMesh* NavMesh);
	void GetNavPolgonVertexPos(int Index, std::vector<Vector3>& vecPos);
	void GetAdjPolyIndex(int Index, std::vector<int>& vecPolyIndex);
	const Vector3& GetVertexPos(int PolyIndex, int VertexIndex);
	void OnUpdatePos(const Vector3& WorldPos, const Vector3& RelativePos);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CNavMeshComponent* Clone();
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;


	void FindPath(const Vector3& Start, const Vector3& End,
		std::list<Vector3>& vecPath);
	NavigationCell* FindCell(const Vector3& Pos);

	void AddCellCloseList(NavigationCell* Cell, NavigationCell* End);
	// ù��° ���ڷ� ���� Cell�� ���� Cell���� ���� ��Ͽ� �־��ش�
	void AddAdjCellOpenList(NavigationCell* Cell, NavigationCell* End, int ParentIndex);
	void DelteCellOpenList(NavigationCell* Cell);
	
	NavigationCell* FindCell(int PolyIndex);

	static bool SortByTotal(NavigationCell* Src, NavigationCell* Dest)
	{
		return Src->Total < Dest->Total;
	}
};

