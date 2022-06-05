#pragma once

#include "IMGUIWidgetList.h"

class CSceneComponentWidget :
    public CIMGUIWidgetList
{
public:
    CSceneComponentWidget ();
    virtual ~CSceneComponentWidget();

public:
    virtual bool Init() override;

public:
    // �ݵ�� Init ���� �� �Լ� ȣ���ؾ� ��
    // SceneComponent ��� ���� ���鶧���� �������̵� �ؾ� ��
    virtual void SetSceneComponent(class CSceneComponent* Com);

public:
    void OnGameObjectEnable(bool Enable);

private:
    void OnClickRenameButton();
    void OnCheckEnableCheckBox(int Idx, bool Check);
protected:
    CSharedPtr<class CSceneComponent> m_Component;
    class CIMGUIText* m_ComponentTypeText;
    class CIMGUITextInput* m_NameInput;
    class CIMGUIButton* m_RenameButton;
    class CIMGUICheckBox* m_EnableCheckBox;
    class CTransformWidget* m_TransformWidget;
    std::string m_PrevName;
public :
    CSceneComponent* GetComponent() const
    {
        return m_Component;
    }
};

