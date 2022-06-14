#pragma once
#include "IMGUIWindow.h"

struct Particle3DObjectBackUpStruct
{
    bool IsCameraRotate;
    bool IsRotateInv;
};

// Particle �� �����ϴ� Editor
class CEffectEditor :
    public CIMGUIWindow
{
    friend class CEditorManager;
public:
    CEffectEditor();
    ~CEffectEditor();
private:
    class CIMGUIButton* m_SaveParticleBtn;
    class CIMGUIButton* m_LoadParticleBtn;
    class CIMGUIButton* m_StartEditBtn;
    class CIMGUIButton* m_RestartBtn;

private:
    class CIMGUIInputFloat* m_SpawnTimeMaxEdit;
    // class CIMGUIButton* m_RestartButton;

    class CIMGUIInputFloat3* m_StartMinEdit;
    class CIMGUIInputFloat3* m_StartMaxEdit;

    class CIMGUIInputInt* m_SpawnCountMaxEdit;
    // class CIMGUIButton* m_RestartButton;

    class CIMGUIInputFloat3* m_ScaleMinEdit;
    class CIMGUIInputFloat3* m_ScaleMaxEdit;

    class CIMGUIInputFloat* m_LifeTimeMinEdit;
    class CIMGUIInputFloat* m_LifeTimeMaxEdit;

    class CIMGUIInputFloat* m_SpeedMinEdit;
    class CIMGUIInputFloat* m_SpeedMaxEdit;

    class CIMGUIColor4* m_ColorMinEdit;
    class CIMGUIColor4* m_ColorMaxEdit;

    // Material
    class CIMGUITextInput* m_MaterialName;

    class CIMGUICheckBox* m_IsMoveEdit;
    class CIMGUICheckBox* m_IsGravityEdit;
    class CIMGUICheckBox* m_IsRandomMoveEdit;
    class CIMGUICheckBox* m_IsPauseResumeToggle;

    // Camera Related
    class CIMGUICheckBox* m_IsRotateEdit;
    class CIMGUICheckBox* m_IsRotateInv;
    class CIMGUISliderFloat* m_RotateSpeedSliderBar;
    class CIMGUICheckBox* m_IsZoomEdit;
    class CIMGUISliderFloat* m_ZoomSpeedSliderBar;

    // ī�޶��� Y �� ��ġ (���� ����, �Ʒ��� ���� ����)
    class CIMGUISliderFloat* m_CameraYOffsetBar;

    // ī�޶��� X �� ���� ȸ�� ���� (���� ����, �Ʒ��� ����)
    class CIMGUISliderFloat* m_CameraXRotSlideBar;

    class CIMGUIInputFloat3* m_MoveDirEdit;
    class CIMGUIInputFloat3* m_MoveAngleEdit;

    // Camera ���� ����
    // class CIMGUICheckBox* m_CameraZoom;
    // class CIMGUICheckBox* m_CameraLookDownFromUp;
    // class CIMGUICheckBox* m_CameraRotate;

    // Unity ���� ���� �߰�
    class CIMGUIInputFloat* m_GravityAccelEdit;
    class CIMGUIInputFloat* m_StartDelayEdit;
    class CIMGUIButton* m_SetMaterialTextureButton;
    class CIMGUIButton* m_RestartButton;

    // Render Target
    class CIMGUIImage* m_ParticleTexture;
    class CIMGUIImage* m_ParticleRenderTarget;
private :
    class CParticle* m_ParticleClass;
    class CMaterial* m_ParticleMaterial;
private :
    // class CParticleComponent* m_ParticleComponent;
    // class CStaticMeshComponent* m_BaseGroundComponent;
    // class CStaticMeshComponent* m_SkyComponent;
    class C3DParticleObject* m_ParticleObject;
    class CGameObject* m_ParticleSampleObject;
    class CGameObject* m_BaseGroundObject;
    class CGameObject* m_SkyObject;
public :
    virtual bool Init() override;
private:
    void OnSaveParticleObjectButton();
    void OnLoadParticleObjectButton();
    void OnRestartParticleComponentButton();

    void OnSpawnTimeMaxEdit(float Num);

    void OnStartMinEdit(const Vector3&);
    void OnStartMaxEdit(const Vector3&);

    void OnSpawnCountMaxEdit(int Num);

    void OnScaleMinEdit(const Vector3&);
    void OnScaleMaxEdit(const Vector3&);

    void OnLifeTimeMinEdit(float Num);
    void OnLifeTimeMaxEdit(float Num);

    void OnSpeedMinEdit(float Num);
    void OnSpeedMaxEdit(float Num);

    void OnColorMinEdit(const Vector4& Color);
    void OnColorMaxEdit(const Vector4& Color);

    void OnIsMoveEdit(const char*, bool);
    void OnIsGravityEdit(const char*, bool);
    void OnIsRandomMoveEdit(const char*, bool);
    void OnPauseResumeToggle(const char*, bool);

    void OnIsCameraRotateEdit(const char*, bool);
    void OnCameraRotateInvEdit(const char*, bool);
    void OnSetCameraRotateSpeed(float Speed);

    void OnIsCameraZoomEdit(const char*, bool);
    void OnSetCameraZoomSpeed(float Speed);

    void OnSetCameraYOffset(float Offset);
    void OnSetCameraXRot(float Rot);

    void OnMoveDirEdit(const Vector3& Dir);
    void OnMoveAngleEdit(const Vector3& Angle);

    void OnSetParticleTexture();

    void OnSaveParticleClass();
    void OnLoadParticleClass();
private:
    // Particle �⺻ ����
    void OnSetBasicParticleMaterialSetting(class CSceneComponent* Com);
    void OnReflectCurrentParticleSetting();
    void OnReflectCurrentCameraSetting();
    void SetGameObjectReady();
    void SetStartEditing();

    // Material Setting
    void OnDropMaterialToParticle(const std::string& InputName);

    // Helper 
    void SetParticleToParticleComponent(class CParticleComponent* Component, const char* ParticleName);
    void SetParticleToParticleComponent(class CParticleComponent* Component, CParticle* Particle);
    void SetIMGUIReflectPartice(class CParticle* Particle);
};

