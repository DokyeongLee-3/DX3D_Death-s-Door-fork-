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
#include "IMGUITextInput.h"
#include "IMGUISliderFloat.h"
#include "IMGUISliderInt.h"
// Window
#include "MaterialEditor.h"

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
	
	m_NewMaterialName = AddWidget<CIMGUITextInput>("New Material Name", 150.f, 20.f);
	m_NewMaterialName->SetHideName(true);


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

void CMaterialEditor::OnSaveMaterialCallback()
{
}

void CMaterialEditor::OnLoadMaterialCallback()
{
}

void CMaterialEditor::OnCreateMaterialCallback()
{
}
