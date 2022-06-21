#pragma once
#include "IMGUIWindow.h"

struct Particle3DObjectBackUpStruct
{
    bool IsCameraRotate;
    bool IsCameraRotateInv;
    bool IsCameraRotateSpeed;
    bool IsCameraZoom;
    float CameraZoomSpeed;
    float CameraYOffset;
    float CameraXRot;
    float TargetDist;
    float WorldRotY;
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
private :
    class CIMGUITextInput* m_CurrentParticleName;
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

    class CIMGUIInputFloat* m_AlphaMinEdit;
    class CIMGUIInputFloat* m_AlphaMaxEdit;
    class CIMGUIButton* m_AlphaBlendEnableButton;

    // Material
    class CIMGUITextInput* m_CurrentMaterialName;
    class CIMGUITextInput* m_LoadedMaterialFileName;

    // Ground Texture
    class CIMGUISliderFloat* m_GroundTextureScale;

    class CIMGUICheckBox* m_IsMoveEdit;
    class CIMGUICheckBox* m_IsGravityEdit;
    // class CIMGUICheckBox* m_IsRandomMoveEdit;
    class CIMGUICheckBox* m_IsPauseResumeToggle;

    // Camera Related
    class CIMGUICheckBox* m_IsRotateEdit;
    class CIMGUICheckBox* m_IsRotateInv;
    class CIMGUISliderFloat* m_RotateSpeedSliderBar;
    class CIMGUICheckBox* m_IsZoomEdit;
    class CIMGUISliderFloat* m_ZoomSpeedSliderBar;

    // Bounce Effect
    class CIMGUICheckBox* m_IsBounce;
    class CIMGUISliderFloat* m_BounceResistance; // ������

    // Circle Generate
    class CIMGUICheckBox* m_IsGenerateCircle;
    class CIMGUISliderFloat* m_GenerateCircleRadius; // ������
    class CIMGUICheckBox* m_IsLoopGenerateCircle; // ������

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
    // class CIMGUIImage* m_ParticleRenderTarget;
private :
    class CParticle* m_ParticleClass;
    class CMaterial* m_ParticleMaterial;
private :
    Particle3DObjectBackUpStruct m_BackUpStruct;
    bool m_StartEdit;
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

    // Base Ground Size
    void OnEditBaseGroundSize(float Speed);

    // Bounce
    void OnIsBounceEdit(const char*, bool);
    void OnEditBounceResistance(float Speed);

    // Generate Circle
    void OnIsGenerateCircleEdit(const char*, bool);
    void OnIsLoopGenerateCircleEdit(const char*, bool);
    void OnEditGenerateCircleRadius(float Radius);

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

    void OnAlphaMinEdit(float Alpha);
    void OnAlphaMaxEdit(float Alpha);

    // ���� Material�� Alpha Blend �����ϱ�
    void OnSetAlphaBlendToMaterialCallback();

    void OnIsMoveEdit(const char*, bool);
    void OnIsGravityEdit(const char*, bool);

    // void OnIsRandomMoveEdit(const char*, bool);
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

    // Base Ground Object �غ��Ű��
    void SetGameObjectReady();

    // �⺻ Particle ���� �����ϱ�
    void SetBasicDefaultParticleInfos(class CParticle* Particle);

    // Material Setting
    void OnDropMaterialToParticle(const std::string& InputName);
    void ApplyNewMaterial(class CMaterial* Material);

    // Particle Object ���, 
    void BackUpParticleObjectInfo();
    void ReflectBackUpParticleObjectInfo();

    // Edit ���� �ϱ� (�⺻ ���� ����)
    void SetStartEditing();

    // Particle Component �� Particle �����ϱ� 
    void SetParticleToParticleComponent(class CParticleComponent* Component, const char* ParticleName);
    void SetParticleToParticleComponent(class CParticleComponent* Component, CParticle* Particle);

    // IMGUI�� Particle ���� �ݿ��ϰ� �ϱ� 
    void SetIMGUIReflectParticle(class CParticle* Particle);

    // IMGUI�� Paritlc Object ���� �ݿ��ϰ� �ϱ� 
    void SetIMGUIReflectObjectCamera();
};

