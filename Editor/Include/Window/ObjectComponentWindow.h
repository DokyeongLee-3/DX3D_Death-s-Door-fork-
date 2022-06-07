#pragma once

#include "IMGUIWindow.h"
#include "IMGUIListBox.h"

class CObjectComponentWindow :
    public CIMGUIWindow
{
public:
	CObjectComponentWindow();
	virtual ~CObjectComponentWindow();

private:
	class CIMGUIButton*					m_ComponentCreatePopUpButton;
	class CIMGUIButton*					m_ComponentDeleteButton;
	class CObjectComponentCreateModal*	m_ComponentCreateModal;
	class CIMGUIListBox*				m_ComponentListBox;
	int m_SelectIndex;
	std::string m_SelectLabel;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void OnRenameComponent(const std::string& NewName, const std::string& PrevName);
	// Component ���� �˾�â ����ִ� �ݹ�
	void OnCreateComponentPopUp();
	std::string GetComponentNameInput()	const;
	int AddObjectComponent(const std::string& Name);
	void OnSelectComponent(int Index, const char* Label);
	void OnDeleteComponent();
	void OnUpdateObjectComponetWindow(class CIMGUITree* SelectObjectNode);

	// Component Save&Load
	void OnSaveComponent();
	void OnLoadComponent();

public:
	int GetSelectIndex()	const
	{
		return m_SelectIndex;
	}

	std::string GetSelectLabel()	const
	{
		return m_SelectLabel;
	}

	void ClearListBox()
	{
		m_ComponentListBox->Clear();
	}


public:
	template <typename T>
	void SetSelectCallback(int Index, void(T::*Func)(int, const char*))
	{
		m_ComponentListBox->SetSelectCallback(this, &CObjectComponentWindow::OnSelectComponent);
	}
	// List���� Select�� Component ���� Ž��
	//void FindSelectComponent(class CIMGUITree* RootNode);
};

