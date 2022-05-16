#pragma once

#include "IMGUIWindow.h"

class CComponentHierarchyWindow :
    public CIMGUIWindow
{
public:
	CComponentHierarchyWindow();
	virtual ~CComponentHierarchyWindow();

private:
	std::vector<class CIMGUITree*>	m_vecComponentTree;
	class CIMGUIButton*				m_ComponentCreatePopUpButton;
	class CComponentCreateModal*		m_ComponentCreateModal;

	class CIMGUIButton*				m_ComponentDelButton;
	class CIMGUIButton*				m_ComponentDelHierarchyButton;
	class CIMGUITree*				m_Root;
	class CIMGUITree*				m_SelectNode;

public:
	class CIMGUITree* GetRoot()	const;
	class CIMGUITree* GetSelectNode()	const
	{
		return m_SelectNode;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	// Component ���� �˾�â ����ִ� �ݹ�
	void OnCreateComponentPopUp();
	// Delete Button ������ ȣ��Ǵ� �ݹ�
	void OnDeleteComponent();
	// Delete Hierarchy
	void OnDeleteHierarchyComponent();

public:
	// 2��° ���ڿ� ���� ���õǾ� �ִ� ��带 �־��ִ� �Լ�
	// ������ �Ʒ��� Ž���ϹǷ� 1��° ���ڿ��� ��Ʈ ��带 �־������
	void FindSelectNode(class CIMGUITree* RootNode);
};

