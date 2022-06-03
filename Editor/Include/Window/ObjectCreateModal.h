#pragma once

#include "IMGUIPopUpModal.h"

class CObjectCreateModal :
    public CIMGUIPopUpModal
{
public:
	CObjectCreateModal();
	virtual ~CObjectCreateModal();

private:
	//class CIMGUIPopUpModal* m_ObjectCreatePopUp;
	class CIMGUIComboBox* m_ObjectCombo;
	class CIMGUITextInput* m_NameTextInput;
	class CIMGUIButton* m_ObjectCreateButton;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	std::string GetObjectNameInput()	const;

	// ������Ʈ ���� �˾�â������ Create��ư ������ ȣ��Ǵ� �ݹ�
	void OnCreateObject();

	// Load Object �ÿ�, Type Index �� �޾Ƽ� Object �� ������ ���
	void OnCreateObject(const char* FullPathMultibyte);
};

