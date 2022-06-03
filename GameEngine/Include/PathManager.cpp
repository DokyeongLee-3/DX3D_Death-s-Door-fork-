#include "PathManager.h"

CPathManager* CPathManager::m_Inst = nullptr;

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
    auto    iter = m_mapPath.begin();
    auto    iterEnd = m_mapPath.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE(iter->second);
    }
}

bool CPathManager::Init()
{
    // RootPath�� ��θ� ���´�.
    // RootPath�� Bin���� ������ ��ü ��θ� ���Ѵ�.
    TCHAR   Path[MAX_PATH] = {};

    // GetModuleFileName : ���� ���������� �ִ� ��� + ���������̸�.exe ��
    // ��ü��θ� ������ �ȴ�.
    // ���⿡�� �������� �̸������� ���� Bin���� ������ ���ͼ�
    // Root������ ����Ѵ�.
    GetModuleFileName(0, Path, MAX_PATH);

    // ���� ���ڿ��� �ڿ������� ���ʷ� �����Ͽ� \�� / �� �ִ� ���ڸ� ã�´�.
    int Length = lstrlen(Path);

    // D:\Lecture\37th\API\GameFramework\GameFramework\Bin\GameFramework_Debug.exe
    for (int i = Length - 1; i > 0; --i)
    {
        if (Path[i] == '\\')
        {
            memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
            break;
        }
    }

    PathInfo* Info = new PathInfo;

    lstrcpy(Info->Path, Path);

#ifdef UNICODE
    // �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
    int ConvertLength = WideCharToMultiByte(CP_ACP, 0, Path, -1, nullptr, 0, 0, 0);

    WideCharToMultiByte(CP_ACP, 0, Path, -1, 
        Info->PathMultibyte, ConvertLength, 0, 0);
#else
    strcpy_s(Info->PathMultibyte, Path);
#endif // UNICODE

    m_mapPath.insert(std::make_pair(ROOT_PATH, Info));

    AddPath(SHADER_PATH, TEXT("Shader\\"));
    AddPath(TEXTURE_PATH, TEXT("Texture\\"));
    AddPath(FONT_PATH, TEXT("Font\\"));
    AddPath(ANIMATION_PATH, TEXT("Animation\\"));
    AddPath(SCENE_PATH, TEXT("Scene\\"));
    AddPath(SOUND_PATH, TEXT("Sound\\"));
    AddPath(MESH_PATH, TEXT("Mesh\\"));
    AddPath(EXCEL_PATH, TEXT("Excel\\"));
    AddPath(PARTICLE_PATH, TEXT("Texture\\Particle\\"));

    // Resource Path
    AddPath(RESOURCE_SHADER_PATH, TEXT("Resource\\Shader\\"));
    AddPath(RESOURCE_TEXTURE_PATH, TEXT("Resource\\Texture\\"));
    AddPath(RESOURCE_FONT_PATH, TEXT("Resource\\Font\\"));
    AddPath(RESOURCE_ANIMATION_PATH, TEXT("Resource\\Animation\\"));
    AddPath(RESOURCE_SCENE_PATH, TEXT("Resource\\Scene\\"));
    AddPath(RESOURCE_SOUND_PATH, TEXT("Resource\\Sound\\"));
    AddPath(RESOURCE_MESH_PATH, TEXT("Resource\\Mesh\\"));
    AddPath(RESOURCE_EXCEL_PATH, TEXT("Resource\\Excel\\"));
    AddPath(ENGINE_RESOURCE_OBJECT_PATH, TEXT("Resource\\Object\\"));
    AddPath(RESOURCE_PARTICLE_PATH, TEXT("Resource\\Texture\\Particle\\"));

    // Engine Path �����
    TCHAR EnginePath[MAX_PATH] = {};
    GetModuleFileName(nullptr, EnginePath, MAX_PATH);

    int EnginePathLength = lstrlen(EnginePath);

    int Count = 0;

    for (int i = EnginePathLength - 1; i > 0; --i)
    {
        if (EnginePath[i] == '\\')
        {
            Count += 1;

            if (Count == 3)
            {
                memset(&EnginePath[i + 1], 0, sizeof(TCHAR) * (EnginePathLength - i - 1));
                break;
            }
        }
    }

    PathInfo* GameEngineInfo = new PathInfo;

    lstrcpy(GameEngineInfo->Path, EnginePath);

#ifdef UNICODE
    // �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
    int ConvertEngineLength = WideCharToMultiByte(CP_ACP, 0, EnginePath, -1, nullptr, 0, nullptr, nullptr);

    WideCharToMultiByte(CP_ACP, 0, EnginePath, -1,
        GameEngineInfo->PathMultibyte, ConvertEngineLength, nullptr, nullptr);
#else
    strcpy_s(Info->PathMultibyte, Path);
#endif // UNICODE

    m_mapPath.insert(std::make_pair(ENGINE_ROOT_PATH, GameEngineInfo));
    
    AddPath(ENGINE_RESOURCE_SHADER_PATH, TEXT("GameEngine\\Include\\Resource\\Shader\\"), ENGINE_ROOT_PATH);
    AddPath(ENGINE_RESOURCE_TEXTURE_PATH, TEXT("GameEngine\\Include\\Resource\\Texture\\"), ENGINE_ROOT_PATH);
    AddPath(ENGINE_RESOURCE_FONT_PATH, TEXT("GameEngine\\Include\\Resource\\Font\\"), ENGINE_ROOT_PATH);
    AddPath(ENGINE_RESOURCE_ANIMATION_PATH, TEXT("GameEngine\\Include\\Resource\\Animation\\"), ENGINE_ROOT_PATH);
    AddPath(ENGINE_RESOURCE_SCENE_PATH, TEXT("GameEngine\\Include\\Resource\\Scene\\"), ENGINE_ROOT_PATH);
    AddPath(ENGINE_RESOURCE_SOUND_PATH, TEXT("GameEngine\\Include\\Resource\\Sound\\"), ENGINE_ROOT_PATH);
    AddPath(ENGINE_RESOURCE_SEQUENCE_PATH, TEXT("GameEngine\\Include\\Resource\\Sequence\\"), ENGINE_ROOT_PATH);
    AddPath(ENGINE_RESOURCE_OBJECT_PATH, TEXT("GameEngine\\Include\\Resource\\Object\\"), ENGINE_ROOT_PATH);

    return true;
}

bool CPathManager::AddPath(const std::string& Name, const TCHAR* Path, 
    const std::string& BaseName)
{
    if (FindPath(Name))
        return false;

    const PathInfo* BaseInfo = FindPath(BaseName);

    if (!BaseInfo)
        return false;

    PathInfo* Info = new PathInfo;

    lstrcpy(Info->Path, BaseInfo->Path);
    lstrcat(Info->Path, Path);

#ifdef UNICODE
    // �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
    int ConvertLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, nullptr, 0, 0, 0);

    WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
        Info->PathMultibyte, ConvertLength, 0, 0);
#else
    strcpy_s(Info->PathMultibyte, Info->Path);
#endif // UNICODE

    m_mapPath.insert(std::make_pair(Name, Info));

    return true;
}

const PathInfo* CPathManager::FindPath(const std::string& Name)
{
    std::unordered_map<std::string, PathInfo*>::iterator iter = m_mapPath.find(Name);

    if (iter == m_mapPath.end())
        return nullptr;

    return iter->second;
}
