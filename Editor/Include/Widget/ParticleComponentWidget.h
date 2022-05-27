#pragma once
#include "SceneComponentWidget.h"

class CParticleComponentWidget :
    public CSceneComponentWidget
{
public:
    CParticleComponentWidget();
    virtual ~CParticleComponentWidget();
public:
    virtual bool Init() override;
public:
    // �ݵ�� Init ���� �� �Լ� ȣ���ؾ� ��
    // SceneComponent ��� ���� ���鶧���� �������̵� �ؾ� ��
    virtual void SetSceneComponent(class CSceneComponent* Com) override;
private:
    class CIMGUIInputFloat* m_DurationEdit;

    class CIMGUIInputFloat* m_StartMinEdit;
    class CIMGUIInputFloat* m_StartMaxEdit;

    class CIMGUIInputInt* m_SpawnCountMaxEdit;

    class CIMGUIInputFloat* m_ScaleMinEdit;
    class CIMGUIInputFloat* m_ScaleMaxEdit;

    class CIMGUIInputFloat* m_LifeTimeMinEdit;
    class CIMGUIInputFloat* m_LifeTimeMaxEdit;

    class CIMGUIInputFloat* m_SpeedMinEdit;
    class CIMGUIInputFloat* m_SpeedMaxEdit;

    class CIMGUIColor3* m_ColorMinEdit;
    class CIMGUIColor3* m_ColorMaxEdit;

    class CIMGUICheckBox* m_IsLoopEdit;
    class CIMGUICheckBox* m_IsMoveEdit;
    class CIMGUICheckBox* m_IsGravityEdit;
    // class CIMGUICheckBox* m_Is3DEdit;

    class CIMGUIInputFloat3* m_MoveDirEdit;
    class CIMGUIInputFloat3* m_MoveAngleEdit;

    // Unity ���� ���� �߰�
    class CIMGUIInputFloat* m_GravityAccelEdit;
    class CIMGUIInputFloat* m_StartDelayEdit;
    class CIMGUIButton* m_SetMaterialTextureButton;
    // Render Target

    // Material
    
    // Bubble
};

