#pragma once
#include "IMGUIPopUpModal.h"
class CSceneComponentCreateModal :
    public CIMGUIPopUpModal
{
public:
	CSceneComponentCreateModal();
	virtual ~CSceneComponentCreateModal();

private:

	class CIMGUIComboBox* m_ComponentCombo;
	class CIMGUITextInput* m_NameTextInput;
	class CIMGUIButton* m_ComponentCreateButton;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	std::string GetComponentNameInput()	const;

	// Component ���� �˾�â������ Create��ư ������ ȣ��Ǵ� �ݹ�
	void OnCreateComponent();

	// �θ� Component�� �ִٸ� �θ� Component�� �Ͽ��� ���� �ڽ� ��Ͽ��� ���쵵�� �ϴ� �Լ�
	void DisconnetParent();
	// ���ο� �θ� ������ ���ٸ� ���ο� �θ� Component�� �Ͽ��� ���� �ڽ� ��Ͽ��� �߰��ϵ��� �ϴ� �Լ�
	void SetParent();

private :
	// Particle Editor ���� ���� �ڵ�
	void OnSetParticleEffectEditorSetting(CSceneComponent* Com);
};

