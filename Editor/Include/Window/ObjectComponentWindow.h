#pragma once

#include "IMGUIWindow.h"

class CObjectComponentWindow :
    public CIMGUIWindow
{
public:
	CObjectComponentWindow();
	virtual ~CObjectComponentWindow();

private:
	class CIMGUIButton*					m_ComponentCreatePopUpButton;
	class CObjectComponentCreateModal*	m_ComponentCreateModal;
	class CIMGUIListBox*				m_ComponentListBox;


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	// Component ���� �˾�â ����ִ� �ݹ�
	void OnCreateComponentPopUp();
	std::string GetComponentNameInput()	const;

public:
	// List���� Select�� Component ���� Ž��
	//void FindSelectComponent(class CIMGUITree* RootNode);
};

