#pragma once

#include "IMGUIWidgetList.h"

class CGameObjectWidget :
    public CIMGUIWidgetList
{
public:
    CGameObjectWidget();
    virtual ~CGameObjectWidget();

public:
    virtual bool Init() override;

public:
    void ClearComponentWidget();

public:
    void SetGameObject(class CGameObject* Obj);

public:
    class CGameObject* GetGameObject() const
    {
        return m_Object;
    }

private:
    void CreateSceneComponentWidget(class CSceneComponent* Com);
    void CreateObjectComponentWidget(class CObjectComponent* Com);

private:
    void OnClickRenameButton();
    void OnCheckEnableCheckBox(int Idx, bool Check);

private:
    CSharedPtr<class CGameObject> m_Object;
    class CIMGUITextInput* m_NameInput;
    class CIMGUIButton* m_RenameButton;
    class CIMGUICheckBox* m_EnableCheckBox;

    std::list<class CSceneComponentWidget*> m_SceneComponentWidgetList;
    std::list<class CObjectComponentWidget*> m_ObjectComponentWidgetList;
};

