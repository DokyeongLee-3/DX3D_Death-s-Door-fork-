#pragma once
#include "IMGUIWindow.h"

enum class ParticlePreset
{
    Ripple,
    Ring,
    RingWall,
    Torch,
    FireSmall,
    FireWide,
    Spark,
    SparkBounce,
    SimpleMeteor,
    Max
};

static const char* ParticlePresetNames[] = {
    "Ripple",
    "Ring",
    "RingWall",
    "Torch",
    "FireSmall",
    "FireWide",
    "Spark",
    "SparkBounce",
    "SimpleMeteor" };

static const char* ParticleShapeNames[] = {
    "YUpDirRing", // �� ������ ���� Ring
    "Circle", // Circle (���� ���� �̳� ������ ��ġ�� ����)
    "Torch", // Torch (��� �� ���� ����)
    "ZMinusRing" // ����� ���� �ٶ� ���·� Ring ����
};

static const char* ParticleMoveDirType[] = {
    "YGoingUpRandomDir", // ���� �̵��ϸ鼭, xz ��� �������� ���� �̵�
    "XZSpread", //  xz ��� �������� �̵� y�� 0
    "XYSpread" //  xy ��� �������� �̵� z �� 0
};

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
    class CIMGUISliderFloat* m_GenerateRadius;
private :
    class CIMGUITextInput* m_CurrentParticleName;
private:
    class CIMGUIComboBox* m_ParticlePreset;

    class CIMGUIInputFloat* m_SpawnTimeMaxEdit;
    class CIMGUIInputInt* m_SpawnCountMaxEdit;

    class CIMGUIInputFloat3* m_StartMinEdit;
    class CIMGUIInputFloat3* m_StartMaxEdit;

    class CIMGUIInputFloat3* m_ScaleMinEdit;
    class CIMGUIInputFloat3* m_ScaleMaxEdit;

    // UV Move
    class CIMGUICheckBox* m_IsMoveEnableEdit;
    class CIMGUIInputInt*   m_UVRowN;
    class CIMGUIInputInt*   m_UVColN;

    // Life Time
    class CIMGUICheckBox* m_IsLifeTimeLinearFromCenterEdit;
    class CIMGUIInputFloat* m_LifeTimeMinEdit;
    class CIMGUIInputFloat* m_LifeTimeMaxEdit;

    class CIMGUIInputFloat* m_SpeedMinEdit;
    class CIMGUIInputFloat* m_SpeedMaxEdit;

    class CIMGUIColor4* m_ColorMinEdit;
    class CIMGUIColor4* m_ColorMaxEdit;

    class CIMGUIInputFloat* m_AlphaStartEdit;
    class CIMGUIInputFloat* m_AlphaEndEdit;
    class CIMGUIButton* m_AlphaBlendEnableButton;

    // Material
    class CIMGUITextInput* m_CurrentMaterialName;
    class CIMGUITextInput* m_LoadedMaterialFileName;
    class CIMGUIButton*    m_MaterialLoadButton;

    // Ground Texture
    class CIMGUISliderFloat* m_GroundTextureScale;

    class CIMGUICheckBox* m_IsMoveEdit;
    class CIMGUICheckBox* m_IsGravityEdit;
    class CIMGUICheckBox* m_IsPauseResumeToggle;

    // Camera Related
    class CIMGUICheckBox* m_IsRotateEdit;
    class CIMGUICheckBox* m_IsRotateInv;
    class CIMGUISliderFloat* m_RotateSpeedSliderBar;
    class CIMGUISliderFloat* m_YRotateSliderBar;
    class CIMGUICheckBox* m_IsZoomEdit;
    class CIMGUISliderFloat* m_ZoomSpeedSliderBar;

    // Bounce Effect
    class CIMGUICheckBox* m_IsBounce;
    class CIMGUISliderFloat* m_BounceResistance; // ������

    // Ring Generate
    class CIMGUIComboBox* m_ParticleShapeType;
    class CIMGUICheckBox* m_IsLoopGenerateRing; 

    // Circle Generate
    // class CIMGUICheckBox* m_IsGenerateRing;
    // class CIMGUICheckBox* m_IsGenerateCircle;
    // class CIMGUICheckBox* m_IsGenerateTorch;
    // class CIMGUICheckBox* m_ZMinusRing;

    // ī�޶��� Y �� ��ġ (���� ����, �Ʒ��� ���� ����)
    class CIMGUISliderFloat* m_CameraYOffsetBar;

    // ī�޶��� X �� ���� ȸ�� ���� (���� ����, �Ʒ��� ����)
    class CIMGUISliderFloat* m_CameraXRotSlideBar;

    // Move Dir, Angle
    class CIMGUIComboBox* m_SpecialMoveDirType;
    class CIMGUIInputFloat3* m_MoveDirEdit;
    class CIMGUIInputFloat3* m_MoveAngleEdit;

    // Rotation  Angle
    class CIMGUIInputFloat3* m_MinSeperateRotAngleEdit;
    class CIMGUIInputFloat3* m_MaxSeperateRotAngleEdit;

    // Particle �� ����ϴ� Material Texture
    class CIMGUIImage* m_ParticleTexture;
private :
    class CParticle* m_ParticleClass;
    class CMaterial* m_ParticleMaterial;
private :
    Particle3DObjectBackUpStruct m_BackUpStruct;
    bool m_StartEdit;
private :
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

    // UV Move
    void OnIsUVMoveEnableEdit(const char*, bool);
    void OnUVRowNEdit(int Num);
    void OnUVColNEdit(int Num);

    // Generate Radius
    void OnEditGenerateRadius(float Radius); //

    // Material Load By Btn
    void OnLoadParticleMaterialCallback();

    // Base Ground Size
    void OnEditBaseGroundSize(float Speed);

    // Bounce
    void OnIsBounceEdit(const char*, bool);
    void OnEditBounceResistance(float Speed);

    // Generate Ring
    void OnClickParticleShape(int, const char*);
    void OnIsLoopGenerateRingEdit(const char*, bool);

    // Spawn Time, Count
    void OnSpawnTimeMaxEdit(float Num);
    void OnSpawnCountMaxEdit(int Num);

    // StartMin,Max
    void OnStartMinEdit(const Vector3&);
    void OnStartMaxEdit(const Vector3&);

    // Scale Min, Max
    void OnScaleMinEdit(const Vector3&);
    void OnScaleMaxEdit(const Vector3&);

    // LifeTime Min, Max
    void OnLifeTimeMinEdit(float Num);
    void OnLifeTimeMaxEdit(float Num);
    void OnIsLifeTimeLinearFromCenter(const char*, bool);

    // Speed Min, Max
    void OnSpeedMinEdit(float Num);
    void OnSpeedMaxEdit(float Num);

    // Color Min, Max
    void OnColorMinEdit(const Vector4& Color);
    void OnColorMaxEdit(const Vector4& Color);

    // Alpha Min, Max
    void OnAlphaStartEdit(float Alpha);
    void OnAlphaEndEdit(float Alpha);

    // ���� Material�� Alpha Blend �����ϱ�
    void OnSetAlphaBlendToMaterialCallback();

    // MoveMent
    void OnIsMoveEdit(const char*, bool);
    void OnIsGravityEdit(const char*, bool);
    void OnPauseResumeToggle(const char*, bool);

    // Camera
    void OnIsCameraRotateEdit(const char*, bool);
    void OnCameraRotateInvEdit(const char*, bool);
    void OnSetCameraRotateSpeed(float Speed);
    void OnSetCameraYAxisRotate(float Speed);
    void OnIsCameraZoomEdit(const char*, bool);
    void OnSetCameraZoomSpeed(float Speed);
    void OnSetCameraYOffset(float Offset);
    void OnSetCameraXRot(float Rot);

    // Move Dir, Angle
    void OnMoveDirEdit(const Vector3& Dir);
    void OnMoveAngleEdit(const Vector3& Angle);

    void OnClickSpecialMoveDirType(int Index, const char* Type);

    // Rotation Angle
    void OnMinSeperateRotAngleEdit(const Vector3& RotAngle);
    void OnMaxSeperateRotAngleEdit(const Vector3& RotAngle);

    // Save, Load
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

    // Particle Component �� Particle �����ϱ� .
    void SetParticleToParticleComponent(class CParticleComponent* Component, const char* ParticleName);
    void SetParticleToParticleComponent(class CParticleComponent* Component, CParticle* Particle);

    // IMGUI�� Particle ���� �ݿ��ϰ� �ϱ� 
    void SetIMGUIReflectParticle(class CParticle* Particle);

    // IMGUI�� Paritlc Object ���� �ݿ��ϰ� �ϱ� 
    void SetIMGUIReflectObjectCamera();

    // PresetHelper Function
private :
    void OnClickParticlePreset(int, const char*);
    void OnRipplePreset();
    void OnRingPreset();
    void OnRingWallPreset();
    void OnTorchPreset();
    void OnFireSmallPreset();
    void OnFireWidePreset();
    void OnSparkPreset();
    void OnSparkBouncePreset();
    void OnSimpleMeteorPreset();
};

