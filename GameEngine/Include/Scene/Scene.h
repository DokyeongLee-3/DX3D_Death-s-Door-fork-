#pragma once

#include "SceneMode.h"
#include "SceneResource.h"
#include "SceneCollision.h"
#include "CameraManager.h"
#include "Viewport.h"
#include "NavigationManager.h"
#include "Navigation3DManager.h"
#include "LightManager.h"
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

	std::list<CSharedPtr<CGameObject>>	m_ObjList;
	bool		m_Start;
	bool		m_Change;

	std::list<class CSceneComponent*> m_RenderComponentList;

public:
	void SetAutoChange(bool Change)
	{
		m_Change = Change;
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

public:
	bool Picking(CGameObject*& Result);
	bool CheckSameName(const std::string& Name);
	CGameObject* FindNearChampion(const Vector3& MyPos, float Distance);

public:
	void GetAllObjectsPointer(std::vector<CGameObject*>& vecOutObj);
	void CloneAllNoDestroyObjects(std::list<CSharedPtr<CGameObject>>& OutList);
	void AddObject(CGameObject* Object);

	CGameObject* CreateEmtpyObject()
	{
		CGameObject* Obj = new CGameObject;
		Obj->SetScene(this);
		m_ObjList.push_back(Obj);
		return Obj;
	}

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
		T* Obj = new T;

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
		T* Obj = new T;

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

