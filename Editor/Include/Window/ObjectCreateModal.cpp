
#include "ObjectCreateModal.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "../EditorInfo.h"
#include "../EditorUtil.h"
#include "IMGUIDummy.h"
#include "Scene/SceneManager.h"
#include "IMGUIManager.h"
#include "ObjectHierarchyWindow.h"
#include "SceneComponentHierarchyWindow.h"
#include "ObjectComponentWindow.h"
#include "SceneComponentCreateModal.h"
#include "IMGUITree.h"
#include "GameObject/Champion.h"
#include "GameObject/Minion.h"
#include "GameObject/MapObject.h"
#include "IMGUIRadioButton.h"
#include "IMGUICheckBox.h"

#include <sstream>

CObjectCreateModal::CObjectCreateModal()	:
	m_NameTextInput(nullptr),
	m_ObjectCreateButton(nullptr)
{
}

CObjectCreateModal::~CObjectCreateModal()
{
}

bool CObjectCreateModal::Init()
{
	CIMGUIPopUpModal::Init();

	//m_ObjectCreatePopUp = AddWidget<CIMGUIPopUpModal>("CreateObjectPopUp");

	m_NameTextInput = AddWidget<CIMGUITextInput>("Object Name");
	m_NameTextInput->SetHideName(true);
	m_NameTextInput->SetHintText("Object Name");

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectCreateButton = AddWidget<CIMGUIButton>("Create Button", 100.f, 20.f);
	m_ObjectCreateButton->SetClickCallback<CObjectCreateModal>(this, &CObjectCreateModal::OnCreateObject);

	m_ObjectTypeCheckBox = AddWidget<CIMGUIRadioButton>("Object Type");
	m_ObjectTypeCheckBox->AddCheckInfo("Player");
	m_ObjectTypeCheckBox->AddCheckInfo("Monster");
	m_ObjectTypeCheckBox->AddCheckInfo("MapObject");

	m_ObjectTypeCheckBox->SetCheck(0, true);
	m_ObjectTypeCheckBox->AlwaysCheck(true);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_EnemyCheckBox = AddWidget<CIMGUICheckBox>("Enemy Check");
	m_EnemyCheckBox->AddCheckInfo("Enemy");

	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 100.f, 40.f);

	return true;
}

void CObjectCreateModal::Update(float DeltaTime)
{
	CIMGUIPopUpModal::Render();
}

std::string CObjectCreateModal::GetObjectNameInput() const
{
	std::string Name = m_NameTextInput->GetTextMultibyte();

	return Name;
}

void CObjectCreateModal::OnCreateObject()
{
	CScene* CurrentScene = CSceneManager::GetInst()->GetScene();

	if (!CurrentScene)
		return;

	CIMGUITree* NewObjectTree = nullptr;

	CGameObject* NewObject = nullptr;

	char Name[256] = {};
	strcpy_s(Name, m_NameTextInput->GetTextMultibyte());

	std::string strName = Name;

	CScene* Scene = CSceneManager::GetInst()->GetScene();

	while (Scene)
	{
		bool Same = Scene->CheckSameName(strName);

		if (!Same)
			break;

		if (strName[strName.length() - 1] == '0' || strName[strName.length() - 1] == '1' || strName[strName.length() - 1] == '2'
			|| strName[strName.length() - 1] == '3' || strName[strName.length() - 1] == '4' || strName[strName.length() - 1] == '5'
			|| strName[strName.length() - 1] == '6' || strName[strName.length() - 1] == '7' || strName[strName.length() - 1] == '8'
			|| strName[strName.length() - 1] == '9')
		{
			size_t Length = strName.length();
			std::string NumPart;

			int UnderIdx = -1;

			for (size_t i = 0; i < Length; ++i)
			{
				if (strName[i] == '_')
					UnderIdx = (int)i;

				if (strName[i] == '0' || strName[i] == '1' || strName[i] == '2'
					|| strName[i] == '3' || strName[i] == '4' || strName[i] == '5'
					|| strName[i] == '6' || strName[i] == '7' || strName[i] == '8'
					|| strName[i] == '9')
				{
					NumPart += strName[i];
				}
			}

			std::stringstream ss(NumPart);

			int Num = 0;
			ss >> Num;

			++Num;
			ss.clear();
			ss << Num;

			strName.erase(UnderIdx + 1);

			strName += ss.str();
			strcpy_s(Name, strName.c_str());
		}

		else
		{
			strName += "_1";
			strcpy_s(Name, strName.c_str());
		}
	}


	NewObject = CurrentScene->CreateGameObject<CGameObject>(Name);


	// ����, Loading �� ���ؼ� ObjectCombo Select Index ������ �������ش�.
	// NewObject->SetEditorObjectModalIndex(Index);
	int Idx = -1;
	int CheckIdx = m_ObjectTypeCheckBox->GetCheckItemIdx();

	// TODO : Object_Type �߰��� �� ���� �߰�
	switch (CheckIdx)
	{
	case 0:
		NewObject->SetObjectType(Object_Type::Player);
		break;
	case 1:
		NewObject->SetObjectType(Object_Type::Monster);
		break;
	case 2:
		NewObject->SetObjectType(Object_Type::MapObject);
		break;

	}

	Idx = -1;
	bool IsEnemyCheck = m_EnemyCheckBox->GetCheck(0);

	if (IsEnemyCheck)
		NewObject->SetEnemy(true);

	CObjectHierarchyWindow* ObjHierachy = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	if (ObjHierachy)
	{
		ObjHierachy->OnCreateObject(NewObject);
	}
}

 //CGameObject* CObjectCreateModal::OnCreateObject(const char* FullPathMultibyte)
 //{
 //	CScene* CurrentScene = CSceneManager::GetInst()->GetScene();
 //
 //	if (!CurrentScene)
 //		return nullptr;
 //
 //	// Ȯ���� Ȯ��
 //	char	Ext[_MAX_EXT] = {};
 //	char FileName[MAX_PATH] = {};
 //	char FilePathMultibyte[MAX_PATH] = {};
 //
 //	strcpy_s(FilePathMultibyte, FullPathMultibyte);
 //
 //	_splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, FileName, MAX_PATH, Ext, _MAX_EXT);
 //
 //	_strupr_s(Ext);
 //
 //	if (!strcmp(Ext, ".GOBJ") == 0)
 //		return nullptr;
 //
 //
 //	CGameObject* LoadedObject = CSceneManager::GetInst()->GetScene()->LoadGameObject<CGameObject>();
 //
 //	if (!LoadedObject->Load(FullPathMultibyte))
 //		return nullptr;
 //
 //	CIMGUITree* NewObjectTree = nullptr;
 //
 //	CGameObject* NewObject = nullptr;
 //
 //	char Name[256] = {};
 //	strcpy_s(Name, LoadedObject->GetName().c_str());
 //
 //	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);
 //	CSceneComponentHierarchyWindow* SceneCompWindow = (CSceneComponentHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(SCENECOMPONENT_HIERARCHY);
 //	CObjectComponentWindow* ObjCompWindow = (CObjectComponentWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECTCOMPONENT_LIST);
 //
 //	if (Window)
 //	{
 //		// �ش� Object �� Root Component �� Widget ����
 //		CIMGUITree* NewNode = Window->GetRoot()->AddChild(Name);
 //		NewNode->AddSelectCallback<CObjectHierarchyWindow>(Window, &CObjectHierarchyWindow::OnSetSelectNode);
 //		NewNode->AddSelectCallback<CSceneComponentHierarchyWindow>(SceneCompWindow, &CSceneComponentHierarchyWindow::OnUpdateSceneComponetWindow);
 //		NewNode->AddSelectCallback<CObjectComponentWindow>(ObjCompWindow, &CObjectComponentWindow::OnUpdateObjectComponetWindow);
 //		NewNode->SetDragDropSourceCallback<CObjectHierarchyWindow>(Window, &CObjectHierarchyWindow::OnDragDropSrc);
 //		NewNode->SetDragDropDestCallback<CObjectHierarchyWindow>(Window, &CObjectHierarchyWindow::OnDragDropDest);
 //
 //		// �ش� ��带 Select Node �� �����Ѵ�.
 //		Window->OnSetSelectNode(NewNode);
 //
 //		// �ش� Object �� Root Component �� Widget ����
 //		CSceneComponentHierarchyWindow* ComponentWindow = (CSceneComponentHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(SCENECOMPONENT_HIERARCHY);
 //		ComponentWindow->GetSceneComponentCreateModal()->OnLoadComponentRecursive(LoadedObject);
 //	}
 //
 //	return LoadedObject;
 //}

void CObjectCreateModal::SetObjectType(int Index, bool Boolean)
{
}
