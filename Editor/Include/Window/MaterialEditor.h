#pragma once
#include "IMGUIWindow.h"

class CMaterialEditor :
    public CIMGUIWindow
{
	friend class CEditorManager;
	// friend class CAnimationInstanceConvertThread;
public:
	CMaterialEditor();
	~CMaterialEditor();
private :
	class CMaterial* m_SelectedMaterial;
private :
	class CIMGUIText* m_SelectedMaterialName;
	class CIMGUITextInput* m_NewMaterialName; // ���ÿ� ���� �̸����� ����� ���̴�.

	class CIMGUIButton* m_CreateMaterialBtn;
	class CIMGUIButton* m_SaveMaterialBtn;
	class CIMGUIButton* m_LoadMaterialBtn;

	class CIMGUIListBox* m_MadeMaterialList;

	// Shader �� Drag, Drop �� �� �ִ� ���

	// Texture ����� �����ִ� ���
	class CIMGUITableElemList* m_TextureInfoTable;

	// ������ ���� ����� Setting �ϴ� ���
	class CIMGUICheckBox* m_OutLineCheck;
	class CIMGUIColor4* m_OutLineColor;
	class CIMGUIInputFloat* m_OutLineThickNess;
public :
	void SetSelectMaterial(class CMaterial* Material)
	{
		m_SelectedMaterial = Material;
	}
	void OnSaveMaterialCallback();
	void OnLoadMaterialCallback();
	void OnCreateMaterialCallback();
};

