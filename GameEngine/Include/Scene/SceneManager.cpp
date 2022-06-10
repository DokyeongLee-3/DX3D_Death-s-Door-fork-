
#include "SceneManager.h"
#include "../Render/RenderManager.h"
#include "../Sync.h"
#include "../PathManager.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()	:
	m_Scene(nullptr),
	m_NextScene(nullptr),
	m_Play(true),
	m_Pause(false),
	m_StateManager(nullptr)
{
	InitializeCriticalSection(&m_Crt);
}

CSceneManager::~CSceneManager()
{
	DeleteCriticalSection(&m_Crt);
	SAFE_DELETE(m_Scene);
	SAFE_DELETE(m_NextScene);
	SAFE_DELETE(m_StateManager);
}

void CSceneManager::Start()
{
	m_Scene->Start();
}

bool CSceneManager::Init()
{
	m_Scene = new CScene;

	CRenderManager::GetInst()->SetObjectList(&m_Scene->m_ObjList);

	m_StateManager = new CStateManager;

	m_StateManager->Init();

	return true;
}

bool CSceneManager::Update(float DeltaTime)
{
	if (!m_Play || m_Pause)
	{
		return false;
	}

	m_Scene->Update(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float DeltaTime)
{
	if (!m_Play || m_Pause)
	{
		return false;
	}

	m_Scene->PostUpdate(DeltaTime);

	return ChangeScene();
}

void CSceneManager::Play()
{
	if (m_Play)
	{
		return;
	}

	m_Play = true;
	m_Pause = false;
}

void CSceneManager::Pause()
{
	if (m_Pause || !m_Play)
	{
		return;
	}

	m_Pause = true;
}

void CSceneManager::Resume()
{
	if (!m_Pause || !m_Play)
	{
		return;
	}

	m_Pause = false;
}

void CSceneManager::Stop()
{
	if (!m_Play)
	{
		return;
	}

	m_Play = false;
	m_Pause = false;
}

bool CSceneManager::LoadNewScene(const char* FileName, const std::string& PathName, bool ChangeNow)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	char FullPath[MAX_PATH] = {};

	strcpy_s(FullPath, FileName);

	if (Path)
	{
		strcat_s(FullPath, Path->PathMultibyte);
	}

	return LoadNewSceneFullPath(FullPath, ChangeNow);
}

bool CSceneManager::LoadNewSceneFullPath(const char* FullPath, bool ChangeNow)
{
	CreateNextScene(ChangeNow);
	
	bool LoadResult = m_NextScene->LoadFullPath(FullPath);

	return LoadResult;
}

bool CSceneManager::ChangeScene()
{
	CSync	sync(&m_Crt);

	if (m_NextScene)
	{
		if (m_NextScene->m_Change)
		{
			std::list<CSharedPtr<CGameObject>> NoDestroyObjectCloneList;
			m_Scene->CloneAllNoDestroyObjects(NoDestroyObjectCloneList);

			SAFE_DELETE(m_Scene);
			m_Scene = m_NextScene;
			m_NextScene = nullptr;

			auto iter = NoDestroyObjectCloneList.begin();
			auto iterEnd = NoDestroyObjectCloneList.end();

			for (; iter != iterEnd; ++iter)
			{
				m_Scene->AddObject((*iter).Get());
			}

			CRenderManager::GetInst()->SetObjectList(&m_Scene->m_ObjList);

			m_Scene->Start();

			return true;
		}
	}

	return false;
}

void CSceneManager::CreateNextScene(bool AutoChange)
{
	CSync	sync(&m_Crt);

	SAFE_DELETE(m_NextScene);

	m_NextScene = new CScene;

	m_NextScene->SetAutoChange(AutoChange);
}

void CSceneManager::ChangeNextScene()
{
	CSync	sync(&m_Crt);

	m_NextScene->SetAutoChange(true);
}
