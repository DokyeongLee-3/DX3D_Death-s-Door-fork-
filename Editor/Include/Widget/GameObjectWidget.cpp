#include "GameObjectWidget.h"
#include "GameObject/GameObject.h"
#include "IMGUITextInput.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "IMGUICheckBox.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIDummy.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/LightComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/LandScape.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"
#include "../Component/BuildingComponent.h"
#include "../Widget/StaticMeshComponentWidget.h"
#include "../Widget/LightComponentWidget.h"
#include "../Widget/ObjectComponentWidget.h"
#include "../Widget/ParticleComponentWidget.h"
#include "../Widget/AnimationMeshWidget.h"
#include "../Window/ObjectHierarchyWindow.h"
#include "../Widget/LandScapeWidget.h"
#include "../Widget/ColliderComponentWidget.h"
#include "../Widget/ColliderSphereWidget.h"
#include "../Widget/BuildingComponentWidget.h"
#include "IMGUIManager.h"
#include "../EditorInfo.h"

CGameObjectWidget::CGameObjectWidget()	:
	m_NameInput(nullptr),
	m_RenameButton(nullptr),
	m_EnableCheckBox(nullptr)
{
}

CGameObjectWidget::~CGameObjectWidget()
{
}

bool CGameObjectWidget::Init()
{
	// AddWidget
	CIMGUIText* Text = AddWidget<CIMGUIText>("Text");
	Text->SetText("GameObject Name");

	AddWidget<CIMGUISameLine>("Line");

	m_EnableCheckBox = AddWidget<CIMGUICheckBox>("ObjEnable");
	m_EnableCheckBox->AddCheckInfo("Enable");

	m_NameInput = AddWidget<CIMGUITextInput>("Name");
	AddWidget<CIMGUISameLine>("Line");
	m_RenameButton = AddWidget<CIMGUIButton>("Rename", 0.f, 0.f);

	m_EnemyCheckBox = AddWidget<CIMGUICheckBox>("IsEnemy");
	m_EnemyCheckBox->AddCheckInfo("IsEnemy");

	AddWidget<CIMGUISeperator>("Sep");

	// CallBack
	m_RenameButton->SetClickCallback(this, &CGameObjectWidget::OnClickRenameButton);
	m_EnableCheckBox->SetCallBackIdx(this, &CGameObjectWidget::OnCheckEnableCheckBox);
	m_EnemyCheckBox->SetCallBackIdx(this, &CGameObjectWidget::OnCheckEnemyCheckBox);

	return true;
}

void CGameObjectWidget::ClearComponentWidget()
{
	auto iter = m_SceneComponentWidgetList.begin();
	auto iterEnd = m_SceneComponentWidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}

	m_SceneComponentWidgetList.clear();

	auto iterO = m_ObjectComponentWidgetList.begin();
	auto iterOEnd = m_ObjectComponentWidgetList.end();

	for (; iterO != iterOEnd; ++iterO)
	{
		SAFE_DELETE(*iterO);
	}

	m_ObjectComponentWidgetList.clear();
	
	mVecChild.resize(8);
}

void CGameObjectWidget::SetGameObject(CGameObject* Obj)
{
	m_Object = Obj;

	if (m_Object->IsEnemy())
		m_EnemyCheckBox->SetCheck(0, true);
	else
		m_EnemyCheckBox->SetCheck(0, false);

	if (Obj)
	{
		m_EnableCheckBox->SetCheck(0, m_Object->IsEnable());
		m_NameInput->SetText(m_Object->GetName().c_str());

		// Component Pointer�� �޾ƿ� Type�� �´� Widget�� ����
		std::vector<CSceneComponent*> vecSceneComp;
		m_Object->GetAllSceneComponentsPointer(vecSceneComp);

		size_t Size = vecSceneComp.size();
		for (size_t i = 0; i < Size; ++i)
		{
			CreateSceneComponentWidget(vecSceneComp[i]);
		}

		std::vector<CObjectComponent*> vecObjComp;
		m_Object->GetAllObjectComponentsPointer(vecObjComp);

		Size = vecObjComp.size();
		for (size_t i = 0; i < Size; ++i)
		{
			CreateObjectComponentWidget(vecObjComp[i]);
		}
	}
}

void CGameObjectWidget::CreateSceneComponentWidget(CSceneComponent* Com)
{
	size_t TypeID = Com->GetTypeID();

	CSceneComponentWidget* Widget = nullptr;

	// TODO : ������Ʈ�� ���� �߰�
	if (TypeID == typeid(CStaticMeshComponent).hash_code())
	{
	 	Widget = AddWidget<CStaticMeshComponentWidget>("StaticMeshWidget");
	}
	else if (TypeID == typeid(CAnimationMeshComponent).hash_code())
	{
		Widget = AddWidget<CAnimationMeshWidget>("AnimationMeshWidget");
	}
	else if (TypeID == typeid(CLightComponent).hash_code())
	{
	 	Widget = AddWidget<CLightComponentWidget>("LightWidget");
	}
	else if (TypeID == typeid(CParticleComponent).hash_code())
	{
		Widget = AddWidget<CParticleComponentWidget>("ParticleWidget");
	}
	else if (TypeID == typeid(CLandScape).hash_code())
	{
		Widget = AddWidget<CLandScapeWidget>("LandScapeWidget");
	}
	else if (TypeID == typeid(CColliderBox3D).hash_code())
	{
		Widget = AddWidget<CColliderComponentWidget>("ColliderComponentWidget");
	}
	else if (TypeID == typeid(CColliderSphere).hash_code())
	{
		Widget = AddWidget<CColliderSphereWidget>("ColliderSphereWidget");
	}
	else if (TypeID == typeid(CBuildingComponent).hash_code())
	{
		Widget = AddWidget<CBuildingComponentWidget>("BuildingComponentWidget");
	}
	else
	{
	 	Widget = AddWidget<CSceneComponentWidget>("SceneWidget");
	}

	// Component �־��ָ鼭 ���� Widget�� ����
	Widget->SetSceneComponent(Com);

	m_SceneComponentWidgetList.push_back(Widget);
}

void CGameObjectWidget::CreateObjectComponentWidget(CObjectComponent* Com)
{
	size_t TypeID = Com->GetTypeID();

	// TODO : ������Ʈ�� ���� �߰�
}

void CGameObjectWidget::DeleteSceneComponentWidget(CSceneComponent* Com)
{
	auto iter = m_SceneComponentWidgetList.begin();
	auto iterEnd = m_SceneComponentWidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->IsMyWidget(Com))
		{
			DeleteWidget((*iter));
			m_SceneComponentWidgetList.erase(iter);
			break;
		}
	}
}

bool CGameObjectWidget::FindSceneComponent(CSceneComponent* Com)
{
	auto iter = m_SceneComponentWidgetList.begin();
	auto iterEnd = m_SceneComponentWidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetComponent() == Com)
			return true;
	}

	return false;
}

void CGameObjectWidget::OnClickRenameButton()
{
	m_Object->SetName(m_NameInput->GetTextMultibyte());

	// Hierachy ����
	CObjectHierarchyWindow* Hierachy = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);
	Hierachy->OnRenameObject(m_NameInput->GetTextMultibyte());
}

void CGameObjectWidget::OnCheckEnableCheckBox(int Idx, bool Check)
{
	m_Object->Enable(Check);

	auto iter = m_SceneComponentWidgetList.begin();
	auto iterEnd = m_SceneComponentWidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->OnGameObjectEnable(Check);
	}

	auto iterO = m_ObjectComponentWidgetList.begin();
	auto iterOEnd = m_ObjectComponentWidgetList.end();

	for (; iterO != iterOEnd; ++iterO)
	{
		(*iterO)->OnGameObjectEnable(Check);
	}
}

void CGameObjectWidget::OnCheckEnemyCheckBox(int Idx, bool Check)
{
	m_Object->SetEnemy(Check);
}
