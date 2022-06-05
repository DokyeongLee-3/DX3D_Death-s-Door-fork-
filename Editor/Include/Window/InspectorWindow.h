#pragma once

#include "IMGUIWindow.h"

class CInspectorWindow :
    public CIMGUIWindow
{
public:
    CInspectorWindow();
    virtual ~CInspectorWindow();

public:
    virtual bool Init() override;

public:
    void OnSelectGameObject(class CGameObject* Obj);
    void OnCreateSceneComponent(class CSceneComponent* Com);
    void OnCreateObjectComponent(class CObjectComponent* Com);
    void OnClearGameObject();
private :
    // �ߺ��Ͽ�, Scene Component Widget �� �߰����� �����ϱ� ���� ����
    bool FindSceneComponent(class CSceneComponent* Com);
private:
    class CGameObjectWidget* m_ObjWidget;
};

