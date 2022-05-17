
#include "ObjectHierarchyWindow.h"
#include "IMGUITree.h"
#include "../EditorUtil.h"
#include "IMGUIButton.h"
#include "../EditorInfo.h"
#include "Flag.h"
#include "IMGUISameLine.h"
#include "IMGUIDummy.h"
#include "ObjectCreateModal.h"
#include "IMGUIManager.h"
#include "Scene/SceneManager.h"
#include "GameObject/GameObject.h"

CObjectHierarchyWindow::CObjectHierarchyWindow() :
	m_Root(nullptr),
	m_ObjectDeleteButton(nullptr)
{
}

CObjectHierarchyWindow::~CObjectHierarchyWindow()
{
	//size_t Count = m_vecObjectTree.size();

	//for (size_t i = 0; i < Count; ++i)
	//{
	//	SAFE_DELETE(m_vecObjectTree[i]);
	//}
}

CIMGUITree* CObjectHierarchyWindow::GetRoot() const
{
	return m_Root;
}

bool CObjectHierarchyWindow::Init()
{
	CIMGUIWindow::Init();

	m_Root = AddWidget<CIMGUITree>("Objects");
	// Object Tree�� RootNode�� Component Tree�� RootNode�� �ڽ� TreeNode�� �����ϱ� �������� ����Ʈ�� ���õ� ���·� �س��´�
	m_Root->SetSelected(true);
	m_SelectNode = m_Root;

	m_ObjectCreatePopUpButton = AddWidget<CIMGUIButton>("Create", 50.f, 20.f);
	m_ObjectCreatePopUpButton->SetClickCallback<CObjectHierarchyWindow>(this, &CObjectHierarchyWindow::OnCreateObjectPopUp);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectDeleteButton = AddWidget<CIMGUIButton>("Delete", 50.f, 20.f);
	m_ObjectDeleteButton->SetClickCallback<CObjectHierarchyWindow>(this, &CObjectHierarchyWindow::OnDeleteObject);

	m_vecObjectTree.push_back(m_Root);

	return true;
}

void CObjectHierarchyWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	//CEditorUtil::ShowDemo();

	//CIMGUITree Tree;

	// �� �����Ӹ��� Tree�� ��ȸ�ϸ鼭 m_SelectNode�� ���� ���õ� ��带 ��������
	//FindSelectNode(m_Root);
}

void CObjectHierarchyWindow::OnCreateObjectPopUp()
{
	if (!m_ObjectCreateModal)
		m_ObjectCreateModal = AddWidget<CObjectCreateModal>(OBJECTCREATE_POPUPMODAL);

	else
	{
		PopUpModalState State = m_ObjectCreateModal->GetPopUpModalState();

		if (State == PopUpModalState::Closed)
			m_ObjectCreateModal->SetPopUpModalState(PopUpModalState::Open);
	}
}

void CObjectHierarchyWindow::OnDeleteObject()
{
	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	std::string SelectObjName = Window->GetSelectNode()->GetName();

	CGameObject* Obj = CSceneManager::GetInst()->GetScene()->FindObject(SelectObjName);

	// ���� �󿡼� Object ���� ���� ������ ����
	Obj->DeleteObj();

	// GUI�󿡼� ��� ����
	m_SelectNode->Delete();
}

//void CObjectHierarchyWindow::FindSelectNode(CIMGUITree* RootNode)
//{
//	if (!RootNode)
//		return;
//
//	if (RootNode->IsSelected())
//	{
//		m_SelectNode = RootNode;
//		return;
//	}
//
//	else
//	{
//		size_t Count = RootNode->GetChildCount();
//
//		if(Count > 0)
//		{
//			for (size_t i = 0; i < Count; ++i)
//				FindSelectNode(RootNode->GetNode((int)i));
//		}
//	}
//}

CIMGUITree* CObjectHierarchyWindow::GetSelectNode() const
{
	return m_SelectNode;
}

void CObjectHierarchyWindow::OnSetSelectNode(CIMGUITree* SelectNode)
{
	m_SelectNode = SelectNode;
}

void CObjectHierarchyWindow::OnDragDropSrc(CIMGUITree* SrcTree)
{
	if (m_SelectNode == m_Root || m_DragSrc == m_Root)



		return;

	m_DragSrc = SrcTree;
}

void CObjectHierarchyWindow::OnDragDropDest(CIMGUITree* DestTree, const std::string& ParentName, const std::string& NewChildName)
{
	if (m_SelectNode == m_Root)
		return;

	m_DragDest = DestTree;

	if (m_DragDest && m_DragSrc)
	{
		CScene* CurrentScene = CSceneManager::GetInst()->GetScene();

		CGameObject* SrcObj = CurrentScene->FindObject(m_DragSrc->GetName());
		CGameObject* DestObj = CurrentScene->FindObject(m_DragDest->GetName());

		SrcObj->ClearParent();

		DestObj->AddChildObject(SrcObj);
	}
}

