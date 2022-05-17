#pragma once

#include "IMGUIWindow.h"

class CObjectHierarchyWindow :
	public CIMGUIWindow
{
public:
	CObjectHierarchyWindow();
	virtual ~CObjectHierarchyWindow();

private:
	std::vector<class CIMGUITree*>	m_vecObjectTree;
	class CIMGUIButton* m_ObjectCreatePopUpButton;
	class CObjectCreateModal* m_ObjectCreateModal;
	class CIMGUIButton* m_ObjectDeleteButton;
	class CGameObject* m_SelectObject;

	class CIMGUITree* m_Root;
	class CIMGUITree* m_SelectNode;
	class CIMGUITree* m_DragSrc;
	class CIMGUITree* m_DragDest;

public:
	class CIMGUITree* GetRoot()	const;
	class CGameObject* GetSelectObject()	const;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	// ������Ʈ ���� �˾�â ����ִ� �ݹ�
	void OnCreateObjectPopUp();
	void OnDeleteObject();

	// 2��° ���ڿ� ���� ���õǾ� �ִ� ��带 m_SelectNode�� �־��ش� �Լ�
	// ������ �Ʒ��� Ž���ϹǷ� 1��° ���ڿ��� ��Ʈ ��带 �־������
	//void FindSelectNode(class CIMGUITree* RootNode);
	CIMGUITree* GetSelectNode()	const;
	void OnSetSelectNode(CIMGUITree* Tree);
	void OnDragDropSrc(class CIMGUITree* SrcTree);
	void OnDragDropDest(class CIMGUITree* DestTree, const std::string& ParentName, const std::string& NewChildName);
};


