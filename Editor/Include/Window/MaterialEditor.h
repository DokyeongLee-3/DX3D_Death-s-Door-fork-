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
	class CIMGUITextInput* m_NewMaterialName; // ���ÿ� ���� �̸����� ����� ���̴�.
	class CIMGUIButton* m_CreateMaterialBtn;

	class CIMGUITextInput* m_SelectedMaterialName;  // ���� ���õ� Material �̸�

	// class CIMGUIListBox* m_MadeMaterialList;

	// �ش� Material �� ������ �����ִ� Info Table
	class CIMGUITable* m_MtrlInfoTable;

	// Texture ����� �����ִ� ���
	class CIMGUITableElemList* m_TextureInfoTable;

	// �������� ���� ����� Setting �ϴ� ���
	class CIMGUICheckBox* m_OutLineCheck;
	class CIMGUIColor3* m_OutLineColor;
	class CIMGUIInputFloat* m_OutLineThickNess;

	// Shader �� Drag, Drop �� �� �ִ� ���
	class CIMGUITextInput* m_ShaderSetInput;

	// Drop �ؼ� Set Texture �ϱ�
	class CIMGUITextInput* m_AddTextureInput; // ���ÿ� ���� �̸����� ����� ���̴�.
	class CIMGUIButton* m_AddTextureBtn;

	// Drop �ؼ� Add Texture �ϱ� 
	class CIMGUIInputInt* m_TextureIndex; // ���ÿ� ���� �̸����� ����� ���̴�.
	class CIMGUITextInput* m_SetTextureInput; // ���ÿ� ���� �̸����� ����� ���̴�.
	class CIMGUIButton* m_SetTextureBtn;

	// Drop �ؼ� Image �����ִ� Image Widget
	// ���� Btn ��
	class CIMGUIButton* m_SaveMaterialBtn;
	class CIMGUIButton* m_LoadMaterialBtn;
public:
	virtual bool Init();
public :
	void SetSelectMaterial(class CMaterial* Material)
	{
		m_SelectedMaterial = Material;
	}
private :
	void OnCreateMaterialCallback();

	void OnDropAndCreateMaterialCallback(const std::string&);

	void OnIsOutLineEdit(const char*, bool Enable);
	void OnSetOutLineColor(const Vector3& Color);
	void OnSetOutLineThickNess(float ThickNess);

	void OnSetTextureBtn();
	void OnAddTextureBtn();

	void OnSaveMaterial();
	void OnLoadMaterial();

	void RefreshMaterialDisplayInfo(class CMaterial* Material);
};
