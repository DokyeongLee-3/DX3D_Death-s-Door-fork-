
#include "FileBrowserTree.h"
#include "IMGUIText.h"
#include "IMGUITree.h"
#include "IMGUIDummy.h"
#include "../EditorUtil.h"
#include "PathManager.h"
#include "FileBrowser.h"
#include "IMGUIManager.h"

CFileBrowserTree::CFileBrowserTree()
{
	m_CurrentPath = ROOT_PATH;
}

CFileBrowserTree::~CFileBrowserTree()
{
}

bool CFileBrowserTree::Init()
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(m_CurrentPath);
	std::string FullPath = Info->PathMultibyte;

	m_CurrentPathText = AddWidget<CIMGUIText>(m_CurrentPath);
	m_CurrentPathText->SetText(FullPath.c_str());

	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 20.f, 10.f);

	m_Root = AddWidget<CIMGUITree>("Root");
	std::vector<std::string>	Tmp; // ������� ���� ����

	CEditorUtil::GetAllFilenames(m_CurrentPath, Tmp, m_vecCurrentPathDir);

	size_t Count = m_vecCurrentPathDir.size();
	for (size_t i = 0; i < Count; ++i)
	{
		CIMGUITree* ChildTreeNode = m_Root->AddChild(m_vecCurrentPathDir[i]);
		ChildTreeNode->AddOpenCallback<CFileBrowserTree>(this, &CFileBrowserTree::OnOpenBrowserTree);
		ChildTreeNode->AddSelectCallback<CFileBrowserTree>(this, &CFileBrowserTree::OnShowFileBrowser);
	}

	return true;
}

void CFileBrowserTree::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CFileBrowserTree::OnOpenBrowserTree(CIMGUITree* Tree)
{
	if (Tree->GetChildCount() > 0)
		return;

	// open�� ���丮�� ��ξȿ� �ִ� ���丮 ��ϵ��� �ٽ� �޾ƿͼ� Tree�� Child�� �־��ش�
	const PathInfo* Info = CPathManager::GetInst()->FindPath(m_CurrentPath);
	std::string FullPath = Info->PathMultibyte;

	// Ŭ���� TreeNode ���α����� ���
	FullPath += Tree->GetName();
	std::vector<std::string> vecFileName;
	std::vector<std::string> vecDirName;

	CEditorUtil::GetAllFilenameFullPath(FullPath, vecFileName, vecDirName);

	size_t Count = vecDirName.size();

	for (size_t i = 0; i < Count; ++i)
	{
		CIMGUITree* ChildTreeNode = Tree->AddChild(vecDirName[i]);
		ChildTreeNode->AddOpenCallback<CFileBrowserTree>(this, &CFileBrowserTree::OnOpenBrowserTree);
		ChildTreeNode->AddSelectCallback<CFileBrowserTree>(this, &CFileBrowserTree::OnShowFileBrowser);
	}
}

void CFileBrowserTree::OnShowFileBrowser(CIMGUITree* Tree)
{
	CFileBrowser* FileBrowser = (CFileBrowser*)CIMGUIManager::GetInst()->FindIMGUIWindow(FILE_BROWSER);

	// FileBrowserâ Path Update
	//FileBrowser->SetInitialPath()
}

