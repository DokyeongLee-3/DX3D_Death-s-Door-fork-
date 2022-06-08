// IMGUI
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUICheckBox.h"
#include "IMGUIDummy.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIProgressBar.h"
#include "IMGUIChild.h"
#include "IMGUIText.h"
#include "IMGUITable.h"
#include "IMGUIInputFloat.h"
#include "IMGUIInputInt.h"
#include "IMGUIColor3.h"
#include "IMGUITextInput.h"
#include "IMGUISliderFloat.h"
#include "IMGUISliderInt.h"
#include "IMGUITableElemList.h"
// Window
#include "MaterialEditor.h"
// Engine
#include "Resource/Material/Material.h"
#include "Resource/ResourceManager.h"
#include "PathManager.h"
#include "Engine.h"

CMaterialEditor::CMaterialEditor()
{
}

CMaterialEditor::~CMaterialEditor()
{
}

bool CMaterialEditor::Init()
{
	// class CIMGUIText* m_SelectedMaterialName;  // ���� ���õ� Material �̸�
	// class CIMGUITextInput* m_NewMaterialName; // ���ÿ� ���� �̸����� ����� ���̴�.
	
	// New Material
	m_NewMaterialName = AddWidget<CIMGUITextInput>("New Mtrl Name", 150.f, 20.f);
	m_NewMaterialName->SetHideName(true);
	m_NewMaterialName->SetHintText("New Mtrl Name");

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	m_CreateMaterialBtn = AddWidget<CIMGUIButton>("Create", 90.f, 20.f);
	m_CreateMaterialBtn->SetClickCallback<CMaterialEditor>(this, &CMaterialEditor::OnCreateMaterialCallback);

	// ���� ���õ� Material ����
	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 150.f, 20.f);

	m_SelectedMaterialName = AddWidget<CIMGUITextInput>("Current Mtrl Name", 150.f, 20.f);
	m_SelectedMaterialName->SetHideName(true);
	m_SelectedMaterialName->SetHintText("Current Mtrl Name");
	m_SelectedMaterialName->SetDropCallBack(this, &CMaterialEditor::OnDropAndCreateMaterialCallback);

	m_TextureInfoTable = AddWidget<CIMGUITableElemList>("Texture Info", 400.f, 150.f);
	m_TextureInfoTable->SetTableTitle("Texture Info");

	m_MtrlInfoTable = AddWidget<CIMGUITable>("Mtrl Info", 200.f, 150.f);
	m_MtrlInfoTable->SetTableTitle("Mtrl Info");

	m_OutLineCheck = AddWidget<CIMGUICheckBox>("Outline", 80.f);
	m_OutLineCheck->AddCheckInfo("Outline");
	m_OutLineCheck->SetCallBackLabel<CMaterialEditor>(this, &CMaterialEditor::OnIsOutLineEdit);

	m_OutLineColor = AddWidget<CIMGUIColor3>("Color", 150.f);
	m_OutLineColor->SetCallBack<CMaterialEditor>(this, &CMaterialEditor::OnSetOutLineColor);

	m_OutLineThickNess = AddWidget<CIMGUIInputFloat>("ThickNess", 150.f);
	m_OutLineThickNess->SetCallBack<CMaterialEditor>(this, &CMaterialEditor::OnSetOutLineThickNess);

	// Add Texture
	Dummy = AddWidget<CIMGUIDummy>("Dummy", 150.f, 20.f);

	// Set Texture
	m_TextureIndex = AddWidget<CIMGUIInputInt>("Index", 150.f, 20.f);

	m_SetTextureInput = AddWidget<CIMGUITextInput>("Set Texture", 150.f, 20.f);
	m_SetTextureInput->SetHideName(true);
	m_SetTextureInput->SetHintText("Key Name");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	m_SetTextureBtn = AddWidget<CIMGUIButton>("Set Texture", 90.f, 20.f);
	m_SetTextureBtn->SetClickCallback<CMaterialEditor>(this, &CMaterialEditor::OnSetTextureBtn);

	m_AddTextureInput = AddWidget<CIMGUITextInput>("Key Name", 150.f, 20.f);
	m_AddTextureInput->SetHideName(true);
	m_AddTextureInput->SetHintText("Key Name");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	m_AddTextureBtn = AddWidget<CIMGUIButton>("Add Texture", 90.f, 20.f);
	m_AddTextureBtn->SetClickCallback<CMaterialEditor>(this, &CMaterialEditor::OnAddTextureBtn);

	// Load & Save Btn
	Dummy = AddWidget<CIMGUIDummy>("Dummy", 150.f, 20.f);

	m_SaveMaterialBtn = AddWidget<CIMGUIButton>("Save Mtrl", 90.f, 20.f);
	m_SaveMaterialBtn->SetClickCallback<CMaterialEditor>(this, &CMaterialEditor::OnSaveMaterial);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_LoadMaterialBtn = AddWidget<CIMGUIButton>("Load Mtrl", 90.f, 20.f);
	m_LoadMaterialBtn->SetClickCallback<CMaterialEditor>(this, &CMaterialEditor::OnLoadMaterial);


	// class CIMGUIButton* m_CreateMaterialBtn;
	// class CIMGUIButton* m_SaveMaterialBtn;
	// class CIMGUIButton* m_LoadMaterialBtn;
	// 
	// class CIMGUIListBox* m_MadeMaterialList;
	// 
	// // Shader �� Drag, Drop �� �� �ִ� ���
	// 
	// // Texture ����� �����ִ� ���
	// class CIMGUITableElemList* m_TextureInfoTable;
	// 
	// // Drop �ؼ� Set Texture �ϱ�
	// 
	// // Drop �ؼ� Add Texture �ϱ� 
	// 
	// // Drop �ؼ� Image �����ִ� Image Widget
	// 
	// 
	// // ������ ���� ����� Setting �ϴ� ���
	// class CIMGUICheckBox* m_OutLineCheck;
	// class CIMGUIColor4* m_OutLineColor;
	// class CIMGUIInputFloat* m_OutLineThickNess;


	return true;
}

void CMaterialEditor::OnCreateMaterialCallback()
{
	if (m_NewMaterialName->Empty())
		return;

	CMaterial* NewMaterial = CResourceManager::GetInst()->CreateMaterial<CMaterial>(m_NewMaterialName->GetTextUTF8());

	// �̹� ���õ� ������ �ִٸ� Update
	if (m_SelectedMaterial)
	{
		m_SelectedMaterial = NewMaterial;

		RefreshMaterialDisplayInfo(m_SelectedMaterial);
	}
}

void CMaterialEditor::OnDropAndCreateMaterialCallback(const std::string& MaterialName)
{
	// �ߺ� ����
	if (m_SelectedMaterial && m_SelectedMaterial->GetName() == MaterialName)
		return;
	// ���� Material Name

	// 1) Key ������ �̿��ؼ� Material Manager ���� ����
	CMaterial* FoundMaterial = CResourceManager::GetInst()->FindMaterial(MaterialName);

	if (FoundMaterial)
	{
		m_SelectedMaterial = FoundMaterial;

		RefreshMaterialDisplayInfo(m_SelectedMaterial);

		return;
	}

	// 2) ������, �ش� �ϵ��ũ ���� �̸��� �̿��ؼ� Bin//Material ������ �ִ� File �� ã�Ƽ�
	// �ش� Material ������ Material Manager �� Load �ϰ�
	// m_SeleteMaterial �� �����Ѵ�.
	char MaterialLoadFullPathMultibyte[MAX_PATH] = {};
	// TCHAR MaterialLoadFullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(MATERIAL_PATH);

	if (Info)
		strcpy_s(MaterialLoadFullPathMultibyte, Info->PathMultibyte);

	strcat_s(MaterialLoadFullPathMultibyte, MaterialName.c_str());

	m_SelectedMaterial = CResourceManager::GetInst()->LoadMaterialFullPathMultibyte(MaterialLoadFullPathMultibyte, MaterialName);

	RefreshMaterialDisplayInfo(m_SelectedMaterial);
}

void CMaterialEditor::OnIsOutLineEdit(const char*, bool Enable)
{
	if (!m_SelectedMaterial)
		return;
	m_SelectedMaterial->SetOutLineEnable(Enable);
}

void CMaterialEditor::OnSetOutLineColor(const Vector3& Color)
{
	if (!m_SelectedMaterial)
		return;
	m_SelectedMaterial->SetOutLineColor(Color);
}

void CMaterialEditor::OnSetOutLineThickNess(float ThickNess)
{
	if (!m_SelectedMaterial)
		return;
	m_SelectedMaterial->SetOutLineThickNess(ThickNess);
}

void CMaterialEditor::OnSetTextureBtn()
{
	if (!m_SelectedMaterial )
		return;

	int SetTextureIndex = m_TextureIndex->GetVal();

	if (SetTextureIndex >= m_SelectedMaterial->GetTextureInfo().size())
	{
		MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Texture Index Exceed Texture Size"), NULL, MB_OK);
		return;
	}

	// Drop �� Input �ȿ� �ִ� Texture ���� �ҷ����� 
	// Editor Util �Լ� �ȿ�, --> Ư�� ����, File �ȿ� �̸� �־��ָ�, �� ���鼭, ���� ã���ִ� �Լ� ����
	// �׷��� �ؼ� Full Path, File �̸� �޾ƿ���
	// �̸� ���� ResourceManager, Texture Manager ���� �ҷ�����
	// �� �ȿ����� ������ �״�� ����, ������ Load ���� ����
	CTexture* TargetTexture;

	// if (TargetTexture == nullptr)
	// 	return;

	// int CurrentTextureRegister = m_SelectedMaterial->GetTextureInfo()[SetTextureIndex].Register;
	// 
	// m_SelectedMaterial->SetTextureInfoResource(SetTextureIndex, TargetTexture);
}

void CMaterialEditor::OnAddTextureBtn()
{
	if (!m_SelectedMaterial)
		return;
	// m_SelectedMaterial->SetOutLineThickNess(ThickNess);
}

void CMaterialEditor::OnSaveMaterial()
{
}

void CMaterialEditor::OnLoadMaterial()
{
}

void CMaterialEditor::RefreshMaterialDisplayInfo(class CMaterial* Material)
{
	if (!Material)
		return;

	// Texture Table �� Texture ������ ����
	// - Name, Sampler Type, Texture Name, FileName + EXT, Register ����


	// Info Table ����
	// Shader Name
	// BaseColor, Ambient Color, Specular Color, EmmisiveColor
	
	// OutLine ���� ����

	// Material
}
