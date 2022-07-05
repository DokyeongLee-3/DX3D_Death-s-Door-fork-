#pragma once

#include "SceneMode.h"
#include "SceneResource.h"
#include "SceneCollision.h"
#include "CameraManager.h"
#include "Viewport.h"
#include "NavigationManager.h"
#include "Navigation3DManager.h"
#include "LightManager.h"
#include "../ObjectPoolManager.h"
#include "../GameObject/GameObject.h"

class CScene
{
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	CSharedPtr<CSceneMode> m_Mode;
	CSceneResource* m_Resource;
	CSceneCollision* m_Collision;
	CCameraManager* m_CameraManager;
	CViewport* m_Viewport;
	CNavigationManager* m_NavManager;
	CNavigation3DManager* m_Nav3DManager;
	CLightManager* m_LightManager;
	CSharedPtr<CGameObject> m_SkyObject;

	SceneSaveGlobalData m_SceneGlobalData;

	std::list<CSharedPtr<CGameObject>>	m_ObjList;
	bool		m_Start;
	bool		m_Change;

	bool m_Play;

	std::list<class CSceneComponent*> m_RenderComponentList;
	Vector3 m_OriginCamPos;

public:
	CSceneMode* GetSceneMode()	const
	{
		return m_Mode;
	}

	void SetOriginCamPos(const Vector3& Pos)
	{
		m_OriginCamPos = Pos;
	}

	const Vector3& GetOriginCamPos()	const
	{
		return m_OriginCamPos;
	}
	//void UpdateObjUpdateOrder();

public:
	void SetAutoChange(bool Change)
	{
		m_Change = Change;
	}

	void Play()
	{
		m_Play = true;
	}

	void Pause()
	{
		m_Play = false;
	}

public:
	CGameObject* GetSkyObject() const
	{
		return m_SkyObject;
	}

	CSceneResource* GetResource()	const
	{
		return m_Resource;
	}

	CSceneCollision* GetCollision()	const
	{
		return m_Collision;
	}

	CCameraManager* GetCameraManager()	const
	{
		return m_CameraManager;
	}

	CViewport* GetViewport()	const
	{
		return m_Viewport;
	}

	CNavigationManager* GetNavigationManager()	const
	{
		return m_NavManager;
	}

	CNavigation3DManager* GetNavigation3DManager()	const
	{
		return m_Nav3DManager;
	}

	CLightManager* GetLightManager()	const
	{
		return m_LightManager;
	}

	CGameObject* GetPlayerObject()	const
	{
		return m_Mode->GetPlayerObject();
	}

	const SceneSaveGlobalData& GetSceneSaveGlobalData() const
	{
		return m_SceneGlobalData;
	}

	bool IsPlay() const
	{
		return m_Play;
	}

	CGameObject* FindObject(const std::string& Name)
	{
		auto	iter = m_ObjList.begin();
		auto	iterEnd = m_ObjList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}

		return nullptr;
	}

	bool EraseObjFromList(CGameObject* Target)
	{
		auto iter = m_ObjList.begin();
		auto iterEnd = m_ObjList.end();

		for (; iter != iterEnd; ++iter)
		{
			if (*iter == Target)
			{
				m_ObjList.erase(iter);
				return true;
			}
		}

		return false;
	}


public:
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	bool Save(const char* FileName, const std::string& PathName = SCENE_PATH);
	bool SaveFullPath(const char* FullPath);
	bool Load(const char* FileName, const std::string& PathName = SCENE_PATH);
	bool LoadFullPath(const char* FullPath);

private:
	bool SaveSceneGlobalDataCSV(const char* FileName);
	bool LoadSceneGlobalDataCSV(const char* FileName);

public:
	bool Picking(CGameObject*& Result);
	bool CheckSameName(const std::string& Name);
	void UpdateSceneGlobalData();

public:
	void GetAllObjectsPointer(std::vector<CGameObject*>& vecOutObj);
	void GetAllIncludeSaveObjectsPointer(std::vector<CGameObject*>& vecOutObj);
	void CloneAllNoDestroyObjects(std::list<CSharedPtr<CGameObject>>& OutList);
	void AddObject(CGameObject* Object);

	CGameObject* CreateEmtpyObject()
	{
		//CGameObject* Obj = new CGameObject;
		CGameObject* Obj = CObjectPoolManager::GetInst()->Allocate();
		Obj->SetScene(this);
		m_ObjList.push_back(Obj);
		return Obj;
	}


public:
	// ī�޶� �̵��� ������ true ����
	bool CameraMove(const Vector3& Direction, const Vector3& DestPos, float Speed, float DeltaTime);
	bool RestoreCamera(float Speed, float DeltaTime);

public:
	template <typename T>
	bool CreateSceneMode()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		if (!m_Mode->Init())
		{
			m_Mode = nullptr;
			return false;
		}

		return true;
	}

	template <typename T>
	T* CreateSceneModeEmpty()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		return (T*)*m_Mode;
	}

	template <typename T>
	bool LoadSceneMode()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		return true;
	}

	template <typename T>
	T* CreateGameObject(const std::string& Name)
	{
		T* Obj = nullptr;

		if (typeid(T).hash_code() == typeid(CGameObject).hash_code())
		{
			CGameObject* Obj = CObjectPoolManager::GetInst()->Allocate();

			Obj->SetName(Name);
			Obj->SetScene(this);

			if (!Obj->Init())
			{
				SAFE_RELEASE(Obj);
				return nullptr;
			}

			m_ObjList.push_back(Obj);

			if (m_Start)
				Obj->Start();

			return (T*)Obj;
		}

		else
			Obj = new T;

		Obj->SetName(Name);
		Obj->SetScene(this);

		if (!Obj->Init())
		{
			SAFE_RELEASE(Obj);
			return nullptr;
		}

		m_ObjList.push_back(Obj);

		if (m_Start)
			Obj->Start();

		return Obj;
	}

	template <typename T>
	T* LoadGameObject()
	{
		T* Obj = nullptr;

		if (typeid(T).hash_code() == typeid(CGameObject).hash_code())
		{
			CGameObject* Obj = CObjectPoolManager::GetInst()->Allocate();

			Obj->SetScene(this);

			m_ObjList.push_back(Obj);

			if (m_Start)
				Obj->Start();

			return (T*)Obj;
		}

		else
			Obj = new T;

		Obj->SetScene(this);

		m_ObjList.push_back(Obj);

		if (m_Start)
			Obj->Start();

		return Obj;
	}


private:
	int GetSaveExcludeObjectCount();
	static bool SortRenderList(class CSceneComponent* Src, class CSceneComponent* Dest);
};

