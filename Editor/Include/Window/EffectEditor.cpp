// IMGUI
#include "IMGUITextInput.h"
#include "IMGUIInputFloat.h"
#include "IMGUIInputInt.h"
#include "IMGUIInputFloat3.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "../EditorInfo.h"
#include "../EditorUtil.h"
#include "IMGUICheckBox.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIImage.h"
#include "IMGUISliderFloat.h"
#include "../Window/ObjectHierarchyWindow.h"
#include "IMGUIManager.h"
#include "IMGUIColor3.h"
#include "IMGUIColor4.h"
#include "IMGUIInputFloat.h"
#include "IMGUIComboBox.h"
#include "IMGUIDummy.h"
#include "IMGUITree.h"
// Window
#include "EffectEditor.h"
#include "../EditorManager.h"
#include "../Window/ResourceDisplayWindow.h"
// Object
#include "../Object/3DParticleObject.h"
// Engine
#include "EngineUtil.h"
#include "Component/ParticleComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/Arm.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Render/RenderManager.h"
#include "Resource/Particle/ParticleManager.h"
#include "Scene/SceneResource.h"
#include "GameObject/SkyObject.h"

CEffectEditor::CEffectEditor() :
    m_StartEdit(false)
{
}

CEffectEditor::~CEffectEditor()
{
}

bool CEffectEditor::Init()
{
    // Save, Load

    m_SaveParticleBtn = AddWidget<CIMGUIButton>("Save Particle", 90.f, 20.f);
    m_SaveParticleBtn->SetClickCallback<CEffectEditor>(this, &CEffectEditor::OnSaveParticleClass);

    CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(105);

    m_LoadParticleBtn = AddWidget<CIMGUIButton>("Load Particle", 90.f, 20.f);
    m_LoadParticleBtn->SetClickCallback<CEffectEditor>(this, &CEffectEditor::OnLoadParticleClass);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(210);

    CIMGUIText* HelpText = AddWidget<CIMGUIText>("ParticleSaveLoad", 90.f, 30.f);
    const char* ParticleSaveLoadText = R"(ex) Particle �� ���, Bin/ParticleClass ������ Save Ȥ��, Bin/ParticleClass �κ��� Load �ؾ� �Ѵ�.)";
    HelpText->SetText(ParticleSaveLoadText);
    HelpText->SetIsHelpMode(true);

    // Set Texture
    m_SetMaterialTextureButton = AddWidget<CIMGUIButton>("Set Texture", 90.f, 20.f);
    m_SetMaterialTextureButton->SetClickCallback<CEffectEditor>(this, &CEffectEditor::OnSetParticleTexture);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(105);

    m_StartEditBtn = AddWidget<CIMGUIButton>("Start Edit", 90.f, 20.f);
    m_StartEditBtn->SetClickCallback<CEffectEditor>(this, &CEffectEditor::SetStartEditing);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(205);

    m_RestartBtn = AddWidget<CIMGUIButton>("ReStart", 80.f, 20.f);
    m_RestartBtn->SetClickCallback<CEffectEditor>(this, &CEffectEditor::OnRestartParticleComponentButton);

    // Particle Name
    m_CurrentParticleName = AddWidget<CIMGUITextInput>("Particle Name", 180.f, 30.f);
    m_CurrentParticleName->SetHintText("Current Particle Name");

    // Material
    m_CurrentMaterialName = AddWidget<CIMGUITextInput>("Current Mtrl Name(Drop)", 180.f, 30.f);
    m_CurrentMaterialName->SetHintText("Drop Here");
    m_CurrentMaterialName->SetDropCallBack<CEffectEditor>(this, &CEffectEditor::OnDropMaterialToParticle);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(350.f);

    HelpText = AddWidget<CIMGUIText>("MtrlFileName", 90.f, 30.f);
    const char* DropMtrlText = R"(ex) ResourceDisplayWindow �κ��� Material Key �� Drop �ϰų�,
    Bin/Material �� �ִ� .mtrl ������ Drop �ϸ�, �ش� Material �� ���õȴ�.)";
    HelpText->SetText(DropMtrlText);
    HelpText->SetIsHelpMode(true);


    m_LoadedMaterialFileName = AddWidget<CIMGUITextInput>("Loaded Mtrl File", 180.f, 30.f);
    m_LoadedMaterialFileName->SetHintText("Not Loaded From Disk");

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(290.f);
HelpText = AddWidget<CIMGUIText>("MtrlFileName", 90.f, 30.f);
    const char* LoadedMtrlHelpText = R"(ex)  �ϵ��ũ�κ���, Material File�� Load �Ͽ� ������ ��� 
    Load�� Material File�� �̸��� �����ش�.)";
    HelpText->SetText(LoadedMtrlHelpText);
    HelpText->SetIsHelpMode(true);

    m_MaterialLoadButton = AddWidget<CIMGUIButton>("Load/Set Material", 180.f, 20.f);
    m_MaterialLoadButton->SetClickCallback<CEffectEditor>(this, &CEffectEditor::OnLoadParticleMaterialCallback);

    // Camera
    CIMGUITree* Tree = AddWidget<CIMGUITree>("Camera");

    // Rotate
    m_IsRotateEdit = Tree->AddWidget<CIMGUICheckBox>("Rotate", 80.f);
    m_IsRotateEdit->AddCheckInfo("Rotate");
    m_IsRotateEdit->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsCameraRotateEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(100.f);

    m_IsRotateInv = Tree->AddWidget<CIMGUICheckBox>("Inv", 80.f);
    m_IsRotateInv->AddCheckInfo("Inv");
    m_IsRotateInv->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnCameraRotateInvEdit);


    m_RotateSpeedSliderBar = Tree->AddWidget<CIMGUISliderFloat>("Rotate Speed", 100.f, 20.f);
    m_RotateSpeedSliderBar->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnSetCameraRotateSpeed);
    m_RotateSpeedSliderBar->SetMin(10.f);
    m_RotateSpeedSliderBar->SetMax(90.f);

    // Zoom
    m_IsZoomEdit = Tree->AddWidget<CIMGUICheckBox>("Zoom", 80.f);
    m_IsZoomEdit->AddCheckInfo("Zoom");
    m_IsZoomEdit->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsCameraZoomEdit);

    m_ZoomSpeedSliderBar = Tree->AddWidget<CIMGUISliderFloat>("Zoom Speed", 100.f, 20.f);
    m_ZoomSpeedSliderBar->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnSetCameraZoomSpeed);
    m_ZoomSpeedSliderBar->SetMin(0.2f);
    m_ZoomSpeedSliderBar->SetMax(50.f);

    m_CameraYOffsetBar = Tree->AddWidget<CIMGUISliderFloat>("Camera Y Offset", 100.f, 20.f);
    m_CameraYOffsetBar->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnSetCameraYOffset);
    m_CameraYOffsetBar->SetMin(-50.f);
    m_CameraYOffsetBar->SetMax(50.f);

    m_CameraXRotSlideBar = Tree->AddWidget<CIMGUISliderFloat>("Camera X Rot", 100.f, 20.f);
    m_CameraXRotSlideBar->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnSetCameraXRot);
    m_CameraXRotSlideBar->SetMin(-88.f);
    m_CameraXRotSlideBar->SetMax(88.f);

    // ParticleEffectRenderTarget
   // m_ParticleRenderTarget = AddWidget<CIMGUIImage>("Render Target", 500.f, 400.f);
   // m_ParticleRenderTarget->SetRenderTargetImage(true);
   // m_ParticleRenderTarget->SetTexture(CRenderManager::GetInst()->GetParticleEffectRenderTarget());
   // m_ParticleRenderTarget->SetBorderColor(10, 10, 255);
   // m_ParticleRenderTarget->SetTableTitle("Render Target");

    // Particle Texture
    m_ParticleTexture = AddWidget<CIMGUIImage>("Particle Texture", 200.f, 200.f);
    m_ParticleTexture->SetBorderColor(10, 10, 255);
    m_ParticleTexture->SetTableTitle("Texture");

    // BaseTexture
    Tree = AddWidget<CIMGUITree>("Ground Texture");
    m_GroundTextureScale =  Tree->AddWidget<CIMGUISliderFloat>("Ground Texture", 100.f, 20.f);
    m_GroundTextureScale->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnEditBaseGroundSize);
    m_GroundTextureScale->SetMin(10.f);
    m_GroundTextureScale->SetMax(800.f);

    // Bounce
    Tree = AddWidget<CIMGUITree>("Bounce");
    m_IsBounce = Tree->AddWidget<CIMGUICheckBox>("Bounce", 80.f);
    m_IsBounce->AddCheckInfo("Bounce");
    m_IsBounce->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsBounceEdit);

    m_BounceResistance = Tree->AddWidget<CIMGUISliderFloat>("Bounce Resist", 100.f, 20.f);
    m_BounceResistance->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnEditBounceResistance);
    m_BounceResistance->SetMin(0.01f);
    m_BounceResistance->SetMax(0.99f);

    // Generate Circle
    Tree = AddWidget<CIMGUITree>("Circle Generate");

    m_IsGenerateCircle = Tree->AddWidget<CIMGUICheckBox>("Circle", 80.f);
    m_IsGenerateCircle->AddCheckInfo("Circle");
    m_IsGenerateCircle->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsGenerateCircleEdit);

    m_GenerateCircleRadius = Tree->AddWidget<CIMGUISliderFloat>("Radius", 100.f, 20.f);
    m_GenerateCircleRadius->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnEditGenerateCircleRadius);
    m_GenerateCircleRadius->SetMin(0.0f);
    m_GenerateCircleRadius->SetMax(100.f);

    m_IsLoopGenerateCircle = Tree->AddWidget<CIMGUICheckBox>("Loop", 80.f);
    m_IsLoopGenerateCircle->AddCheckInfo("Loop");
    m_IsLoopGenerateCircle->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsLoopGenerateCircleEdit);
    

    // Movement
    Tree = AddWidget<CIMGUITree>("Movement");

    m_IsMoveEdit = Tree->AddWidget<CIMGUICheckBox>("Move", 80.f);
    m_IsMoveEdit->AddCheckInfo("Move");
    m_IsMoveEdit->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsMoveEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(90.f);

    m_IsGravityEdit = Tree->AddWidget<CIMGUICheckBox>("Gravity", 80.f);
    m_IsGravityEdit->AddCheckInfo("Gravity");
    m_IsGravityEdit->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsGravityEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(170.f);

    // m_IsRandomMoveEdit = Tree->AddWidget<CIMGUICheckBox>("Random", 80.f);
    // m_IsRandomMoveEdit->AddCheckInfo("Random");
    // m_IsRandomMoveEdit->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsRandomMoveEdit);

    // Line = Tree->AddWidget<CIMGUISameLine>("Line");
    // Line->SetOffsetX(260.f);

    m_IsPauseResumeToggle = Tree->AddWidget<CIMGUICheckBox>("Toggle", 80.f);
    m_IsPauseResumeToggle->AddCheckInfo("Toggle");
    m_IsPauseResumeToggle->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnPauseResumeToggle);
    m_IsPauseResumeToggle->SetCheck(0, true);

    // Spawn Time, Spawn Count
    Tree  = AddWidget<CIMGUITree>("Spawn Time, Count");

    m_SpawnTimeMaxEdit = Tree->AddWidget<CIMGUIInputFloat>("Spawn Time", 150.f);
    m_SpawnTimeMaxEdit->SetCallBack(this, &CEffectEditor::OnSpawnTimeMaxEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(270.f);

    m_SpawnCountMaxEdit = Tree->AddWidget<CIMGUIInputInt>("Spawn Max", 150.f);
    m_SpawnCountMaxEdit->SetCallBack(this, &CEffectEditor::OnSpawnCountMaxEdit);

    // Start Min, Start Max
    Tree = AddWidget<CIMGUITree>("Start Min, Max");

    m_StartMinEdit = Tree->AddWidget<CIMGUIInputFloat3>("StartMin", 150.f);
    m_StartMinEdit->SetCallBack(this, &CEffectEditor::OnStartMinEdit);

    m_StartMaxEdit = Tree->AddWidget<CIMGUIInputFloat3>("StartMax", 150.f);
    m_StartMaxEdit->SetCallBack(this, &CEffectEditor::OnStartMaxEdit);

    // Scale Min, Start Max
    Tree = AddWidget<CIMGUITree>("Scale Min, Max");

    m_ScaleMinEdit = Tree->AddWidget<CIMGUIInputFloat3>("ScaleMin", 150.f);
    m_ScaleMinEdit->SetCallBack(this, &CEffectEditor::OnScaleMinEdit);

    m_ScaleMaxEdit = Tree->AddWidget<CIMGUIInputFloat3>("ScaleMax", 150.f);
    m_ScaleMaxEdit->SetCallBack(this, &CEffectEditor::OnScaleMaxEdit);

    // LifeTime Min, Max
    Tree = AddWidget<CIMGUITree>("LifeTime Min, Max");

    m_LifeTimeMinEdit = Tree->AddWidget<CIMGUIInputFloat>("Life T Min", 150.f);
    m_LifeTimeMinEdit->SetCallBack(this, &CEffectEditor::OnLifeTimeMinEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(250.f);

    m_LifeTimeMaxEdit = Tree->AddWidget<CIMGUIInputFloat>("Life T Max", 150.f);
    m_LifeTimeMaxEdit->SetCallBack(this, &CEffectEditor::OnLifeTimeMaxEdit);

    // Speed Min, Max
    Tree = AddWidget<CIMGUITree>("Speed Min, Max");

    m_SpeedMinEdit = Tree->AddWidget<CIMGUIInputFloat>("Speed Min", 150.f);
    m_SpeedMinEdit->SetCallBack(this, &CEffectEditor::OnSpeedMinEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(250.f);

    m_SpeedMaxEdit = Tree->AddWidget<CIMGUIInputFloat>("Speed Max", 150.f);
    m_SpeedMaxEdit->SetCallBack(this, &CEffectEditor::OnSpeedMaxEdit);

    // Color Min, Max
    Tree = AddWidget<CIMGUITree>("Color Min, Max");

    m_ColorMinEdit = Tree->AddWidget<CIMGUIColor4>("Color Min", 150.f);
    m_ColorMinEdit->SetCallBack(this, &CEffectEditor::OnColorMinEdit);

    m_ColorMaxEdit = Tree->AddWidget<CIMGUIColor4>("Color Max", 150.f);
    m_ColorMaxEdit->SetCallBack(this, &CEffectEditor::OnColorMaxEdit);

    // Alpha Min, Max
    Tree = AddWidget<CIMGUITree>("Alpha Min, Max");

    m_AlphaBlendEnableButton = Tree->AddWidget<CIMGUIButton>("Set Alpha Blend", 150.f, 20.f);
    m_AlphaBlendEnableButton->SetClickCallback<CEffectEditor>(this, &CEffectEditor::OnSetAlphaBlendToMaterialCallback);

    m_AlphaMinEdit = Tree->AddWidget<CIMGUIInputFloat>("Alpha Min", 150.f);
    m_AlphaMinEdit->SetCallBack(this, &CEffectEditor::OnAlphaMinEdit);

    m_AlphaMaxEdit = Tree->AddWidget<CIMGUIInputFloat>("Alpha Max", 150.f);
    m_AlphaMaxEdit->SetCallBack(this, &CEffectEditor::OnAlphaMaxEdit);

    // Move Dir, Angle
    Tree = AddWidget<CIMGUITree>("Move Angle, Dir");

    m_MoveDirEdit = Tree->AddWidget<CIMGUIInputFloat3>("Move Dir", 150.f);
    m_MoveDirEdit->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnMoveDirEdit);

    m_MoveAngleEdit = Tree->AddWidget<CIMGUIInputFloat3>("Move Angle", 150.f);
    m_MoveAngleEdit->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnMoveAngleEdit);

    // m_GravityAccelEdit = AddWidget<CIMGUIInputFloat>("Gravity Accel", 100.f);
    // m_StartDelayEdit = AddWidget<CIMGUIInputFloat>("Start Delay T// ", 100.f);

    SetGameObjectReady();

	return true;
}

void CEffectEditor::OnSaveParticleObjectButton()
{
}

void CEffectEditor::OnLoadParticleObjectButton()
{
}

void CEffectEditor::OnRestartParticleComponentButton()
{
    // ���� ���۵� ���ߴٸ� ���� X
    if (!m_ParticleObject)
        return;

    if (!m_ParticleClass)
        return;

    // ������ �����ϴ� Particle Object �� ������� �Ѵ�
    if (m_ParticleObject)
    {
        m_ParticleObject->Destroy();
    }

    BackUpParticleObjectInfo();

    m_ParticleObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<C3DParticleObject>("Particle Effect Base Ground");

    SetParticleToParticleComponent(dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent()), m_ParticleClass);

    ReflectBackUpParticleObjectInfo();

    // IMGUI�� Particle ���� �ݿ��ϰ� �ϱ� 
    SetIMGUIReflectParticle(m_ParticleClass);

    // IMGUI�� Paritlc Object ���� �ݿ��ϰ� �ϱ� 
    SetIMGUIReflectObjectCamera();
}

void CEffectEditor::OnLoadParticleMaterialCallback()
{
    // Load �ϰ�
    const PathInfo* MaterialPathInfo = CPathManager::GetInst()->FindPath(MATERIAL_PARTICLE_PATH);

    // Bin//Material//ParticleMaterial ������ �ִ��� Ȯ���ϰ� ������ش�.
    CEngineUtil::CheckAndMakeDirectory(MaterialPathInfo);

    TCHAR FilePath[MAX_PATH] = {};

    OPENFILENAME OpenFile = {};
    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Material File\0*.mtrl");
    OpenFile.lpstrFile = FilePath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = MaterialPathInfo->Path;

    if (GetOpenFileName(&OpenFile) != 0)
    {
        char	Ext[_MAX_EXT] = {};

        char FilePathMultibyte[MAX_PATH] = {};
        char FileName[MAX_PATH] = {};

        int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

        _splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, FileName, MAX_PATH, Ext, _MAX_EXT);

        _strupr_s(Ext);

        // ���� Load�ϴ� Directory�� Bin/Material/ParticleMaterial ���� Ȯ���ϱ� => �ƴ϶��, Load
        std::string PathInfoBeforeFileName;
        CEditorUtil::GetPathInfoBeforeFileName(FilePathMultibyte, PathInfoBeforeFileName);

        if (strcmp(MaterialPathInfo->PathMultibyte, PathInfoBeforeFileName.c_str()) != 0)
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Particle Material �� ���, �ݵ�� Bin/Material/ParticleMaterial �κ��� Load �ؾ� �Ѵ�."), NULL, MB_OK);
            return;
        }
        // Ȯ���� .anim �� �ƴ϶�� return;
        if (strcmp(Ext, ".MTRL") != 0)
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("EXT Has To Be .mtrl"), NULL, MB_OK);
            return;
        }

        // ���� �̸���, Material �� �����ϴ� Key ������ Ȱ���� ���̴�.
        CMaterial* LoadedMaterial = CResourceManager::GetInst()->LoadMaterialFullPathMultibyte(FilePathMultibyte);

        if (!LoadedMaterial)
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Load Failure"), NULL, MB_OK);
            return;
        }
  
        std::string MaterialFileName = LoadedMaterial->GetName();

        if (MaterialFileName.find(".mtrl") == std::string::npos)
            MaterialFileName.append(".mtrl");

        // Hard Disk �κ���, File�� Drop���� ��쿡��, FileName�� �����Ѵ�.
        m_LoadedMaterialFileName->SetText(MaterialFileName.c_str());

        // Particle �� �����ϰ�
        ApplyNewMaterial(LoadedMaterial);

        // ResourceDisplay Window �� �ִ� Texture ��ϵ� Resource Window �� �߰��ؼ� �����ֱ�
        CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedTextureResources();
        CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedMaterialResources();
        CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedRenderStateResources();
        CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedShaderResources();
    }
}

void CEffectEditor::OnEditBaseGroundSize(float Size)
{
    m_GroundTextureScale->SetValue(Size);
    m_BaseGroundObject->SetWorldScale(Size, Size, 1.f);
}

void CEffectEditor::OnIsBounceEdit(const char*, bool Enable)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetBounceEnable(Enable);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetBounceEnable(Enable);
}

void CEffectEditor::OnEditBounceResistance(float Resist)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetBounceResistance(Resist);
   dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetBounceResist(Resist);
}

void CEffectEditor::OnIsGenerateCircleEdit(const char*, bool Enable)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetGenerateCircleEnable(Enable);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetGenerateCircleEnable(Enable);
}

void CEffectEditor::OnIsLoopGenerateCircleEdit(const char*, bool Enable)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetLoopGenerateCircle(Enable);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetLoopGenerateCircle(Enable);
}

void CEffectEditor::OnEditGenerateCircleRadius(float Radius)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetGenerateCircleRadius(Radius);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetGenerateCircleRadius(Radius);
}

void CEffectEditor::OnSpawnTimeMaxEdit(float Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetSpawnTimeMax(Num);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->SetSpawnTime(Num);
}

// StartMin, Max , SpawnCountMax �� ���, Particle Component ���� Particle �� ��� ���۷� ���� ������ �ٷ� ���ͼ� Post Update ���� ���
// ���� Particle �� ��� ���� ������ �ٲ��ָ� �ȴ�.
void CEffectEditor::OnStartMinEdit(const Vector3& Pos)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetStartMin(Pos);
    // m_ParticleComponent->GetParticle()->SetStartMin(Pos);
}

void CEffectEditor::OnStartMaxEdit(const Vector3& Pos)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetStartMax(Pos);
    // m_ParticleComponent->GetParticle()->SetStartMax(Pos);
}

void CEffectEditor::OnSpawnCountMaxEdit(int Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetSpawnCountMax(Num);
    // m_ParticleComponent->GetCBuffer()->SetSpawnCountMax(Num);
}

void CEffectEditor::OnScaleMinEdit(const Vector3& Scale)
{
    if (!m_ParticleClass)
        return;

    // m_ParticleClass->SetScaleMin(m_ScaleMinEdit->GetValue());
    m_ParticleClass->SetScaleMin(Scale);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetScaleMin(Scale);
}

void CEffectEditor::OnScaleMaxEdit(const Vector3& Scale)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetScaleMax(Scale);

    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetScaleMax(Scale);
    // m_ParticleComponent->GetCBuffer()->SetScaleMax(Scale);
}

void CEffectEditor::OnLifeTimeMinEdit(float Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetLifeTimeMin(Num);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetLifeTimeMin(Num);
    // m_ParticleComponent->GetCBuffer()->SetLifeTimeMin(Num);
}

void CEffectEditor::OnLifeTimeMaxEdit(float Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetLifeTimeMax(Num);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetLifeTimeMax(Num);
    // m_ParticleComponent->GetCBuffer()->SetLifeTimeMax(Num);
}

void CEffectEditor::OnSpeedMinEdit(float Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetSpeedMin(Num);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetSpeedMin(Num);
    // m_ParticleComponent->GetCBuffer()->SetSpeedMin(Num);
}

void CEffectEditor::OnSpeedMaxEdit(float Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetSpeedMax(Num);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetSpeedMax(Num);
    // m_ParticleComponent->GetCBuffer()->SetSpeedMax(Num);
}

void CEffectEditor::OnColorMinEdit(const Vector4& Color)
{
    if (!m_ParticleClass)
        return;

    // Alpha ���� 0���� �Ѵ�.

    //m_ParticleClass->SetColorMin(Color.x, Color.y, Color.z, 1.f);
    m_ParticleClass->SetColorMin(Color);
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetColorMin(Color.x, Color.y, Color.z, 1.f);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetColorMin(Color);
    // m_ParticleComponent->GetCBuffer()->SetColorMin(Color);
}

void CEffectEditor::OnColorMaxEdit(const Vector4& Color)
{
    if (!m_ParticleClass)
        return;

    // m_ParticleClass->SetColorMax(Color.x, Color.y, Color.z, 1.f);
    m_ParticleClass->SetColorMax(Color);
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetColorMax(Color.x, Color.y, Color.z, 1.f);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetColorMax(Color);
    // m_ParticleComponent->GetCBuffer()->SetColorMax(Color);
}

void CEffectEditor::OnAlphaMinEdit(float Alpha)
{
    if (!m_ParticleClass)
        return;

    // Alpha ���� 0���� �Ѵ�.

    // m_ParticleClass->SetColorMin(Color.x, Color.y, Color.z, 1.f);
    m_ParticleClass->SetMinAlpha(Alpha);
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetColorMin(Color.x, Color.y, Color.z, 1.f);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetMinAlpha(Alpha);
}

void CEffectEditor::OnAlphaMaxEdit(float Alpha)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetMaxAlpha(Alpha);
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetColorMin(Color.x, Color.y, Color.z, 1.f);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetMaxAlpha(Alpha);
}

void CEffectEditor::OnSetAlphaBlendToMaterialCallback()
{
    if (!m_ParticleClass)
        return;

    if (m_ParticleClass->GetMaterial())
    {
        CRenderState* FoundRenderState = CRenderManager::GetInst()->FindRenderState("AlphaBlend");
        
        if (!FoundRenderState)
        {
            assert(false);
        }

        // Particle Class �� Material �� Render State �����ϱ� (�̷��߸� ����� ��, Render State �� ������ ä�� Material ������ ����ȴ�.)
        m_ParticleClass->GetMaterial()->SetRenderState(FoundRenderState);

        // ���� Render �Ǵ� Particle Component �󿡵� ��������� �Ѵ�.
        dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetMaterial()->SetRenderState(FoundRenderState);

        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Alpha Blend Set Success"), NULL, MB_OK);
    }
}


void CEffectEditor::OnIsMoveEdit(const char*, bool Enable)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetMove(Enable);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetMove(Enable);
    // m_ParticleComponent->GetCBuffer()->SetMove(Enable);
}

void CEffectEditor::OnIsGravityEdit(const char*, bool Enable)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetGravity(Enable);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetGravity(Enable);
    // m_ParticleComponent->GetCBuffer()->SetGravity(Enable);
}

// void CEffectEditor::OnIsRandomMoveEdit(const char*, bool Enable)
// {
//     if (!m_ParticleClass)
//         return;
// 
//     m_ParticleClass->SetApplyRandom(Enable); //
//     dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetApplyRandom(Enable);
//     // m_ParticleComponent->GetCBuffer()->SetApplyRandom(Enable);
// }

void CEffectEditor::OnPauseResumeToggle(const char*, bool Enable)
{
    if (!m_ParticleClass)
        return;

    m_ParticleObject->GetRootComponent()->Enable(Enable);
}

void CEffectEditor::OnIsCameraRotateEdit(const char*, bool Enable)
{
    m_ParticleObject->SetCameraRotate(Enable);
}

void CEffectEditor::OnCameraRotateInvEdit(const char*, bool Enable)
{
    m_ParticleObject->SetRotateInv(Enable);
}

void CEffectEditor::OnSetCameraRotateSpeed(float Speed)
{
    m_RotateSpeedSliderBar->SetValue(Speed);

    m_ParticleObject->SetCameraRotateSpeed(Speed);
}

void CEffectEditor::OnIsCameraZoomEdit(const char*, bool Enable)
{
    m_ParticleObject->SetCameraZoom(Enable);
}

void CEffectEditor::OnSetCameraZoomSpeed(float Speed)
{
    m_ZoomSpeedSliderBar->SetValue(Speed);

    m_ParticleObject->SetCameraZoomSpeed(Speed);
}

void CEffectEditor::OnSetCameraYOffset(float Offset)
{
    m_ParticleObject->SetYOffset(Offset);
}

void CEffectEditor::OnSetCameraXRot(float Rot)
{
    m_ParticleObject->SetRelativeRotationXPos(Rot);
}

void CEffectEditor::OnMoveDirEdit(const Vector3& Dir)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetMoveDir(Dir);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetMoveDir(Dir);
}

void CEffectEditor::OnMoveAngleEdit(const Vector3& Angle)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetMoveAngle(Angle);

    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetMoveAngle(Angle);
}


void CEffectEditor::OnSetParticleTexture()
{
    TCHAR FilePath[MAX_PATH] = {};

    OPENFILENAME OpenFile = {};

    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle(); // handle to window that owns the dialog box
    OpenFile.lpstrFilter =
        TEXT("�������\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
    OpenFile.lpstrFile = FilePath;
    // buffer ! filename used to initialize the file name edit control -> ���������� save,open�ҽ� ���⿡ filePath ������ ���´�.
    OpenFile.nMaxFile = MAX_PATH; // size of buffer, pointed to by lpstrFile
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path; // Initial Directory

    if (GetOpenFileName(&OpenFile) != 0) // NonZero --> specifies file name, clicks ok button
    {
        int   TextureIndex = 0;
        TCHAR OriginFileName[MAX_PATH] = {};
        TCHAR FileName[MAX_PATH] = {};
        TCHAR Ext[_MAX_EXT] = {};
        _wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, MAX_PATH, Ext, _MAX_EXT);

        lstrcpy(OriginFileName, FileName);

        lstrcat(FileName, Ext);

        // Texture �����
        char ConvertFileName[MAX_PATH] = {};
        int  Length = WideCharToMultiByte(CP_ACP, 0, OriginFileName, -1, nullptr, 0, nullptr, nullptr);
        WideCharToMultiByte(CP_ACP, 0, OriginFileName, -1, ConvertFileName, Length, nullptr, nullptr);

        CMaterial* Material = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BasicParticleMaterial");

        if (!Material)
            return;

        // m_ParticleTexture->SetTextureFullPath(ConvertFileName, FilePath);
        m_ParticleTexture->SetTexture(ConvertFileName, FileName, PARTICLE_PATH);

        // ���� Particle �� ����ϴ� Material �� Texture ��ü
        Material->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, ConvertFileName, FileName, PARTICLE_PATH);

        // ���� ���� ������ �״�� �ݿ��Ѵ�.
        // OnReflectCurrentParticleSetting();
    }
}

void CEffectEditor::OnSaveParticleClass()
{
    if (!m_ParticleObject || !dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle())
        return;

    const PathInfo* ParticlePathInfo = CPathManager::GetInst()->FindPath(PARTICLE_PATH);

    // Bin//ParticleClass �� Path ���� �����ϴ��� Ȯ���ϱ� , ������ ����� 
    CEngineUtil::CheckAndMakeDirectory(ParticlePathInfo);

    TCHAR FiileFullPath[MAX_PATH] = {};

    OPENFILENAME OpenFile = {};
    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Animation File\0*.anim");
    OpenFile.lpstrFile = FiileFullPath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = ParticlePathInfo->Path; // Bin//ParticleClass

    if (GetSaveFileName(&OpenFile) != 0)
    {
        char FileFullPathMultibyte[MAX_PATH] = {};
        char FileName[MAX_PATH] = {};
        char FileExt[_MAX_EXT] = {};

        int  ConvertLength = WideCharToMultiByte(CP_ACP, 0, FiileFullPath, -1, nullptr, 0, nullptr, nullptr);

        WideCharToMultiByte(CP_ACP, 0, FiileFullPath, -1, FileFullPathMultibyte, ConvertLength, nullptr, nullptr);

        _splitpath_s(FileFullPathMultibyte, nullptr, 0, nullptr, 0, FileName, MAX_PATH, FileExt, _MAX_EXT);

        _strupr_s(FileExt);

        // Ȯ���� .anim �� �ƴ϶�� return;
        if (strcmp(FileExt, ".PRTC") != 0)
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("EXT Has To Be .prtc"), NULL, MB_OK);
            return;
        }

        // ���� �����ϴ� Directory�� Bin/ParticleClass ���� Ȯ���ϱ� => �ƴ϶��, Save ����
        std::string PathInfoBeforeFileName;
        CEditorUtil::GetPathInfoBeforeFileName(FileFullPathMultibyte, PathInfoBeforeFileName);

        if (strcmp(ParticlePathInfo->PathMultibyte, PathInfoBeforeFileName.c_str()) != 0)
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Particle Class �� ���, �ݵ�� Bin/ParticleClass �� ����"), NULL, MB_OK);
            return;
        }

        // �ش� PARTICLE_PATH ���� �ߺ��� �̸��� �ִ��� Ȯ���ϱ�
        // .prtc Ȯ���� ���̱�
        char CheckFileName[MAX_PATH] = {};
        strcpy_s(CheckFileName, FileName);
        strcat_s(CheckFileName, ".prtc");

        bool IsSameFileNameExist = CEditorUtil::IsFileExistInDir(PARTICLE_PATH, CheckFileName);

        if (IsSameFileNameExist)
        {
            if (MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("���� �̸��� .prtc ������ �����մϴ�. �����Ͻðڽ��ϱ�?"), NULL, MB_YESNO) != IDYES)
                return;
        }

        // Particle Manager �󿡼� ���� �̸����� ����Ǿ��ִ� Particle �� Key ���� ���� ������ ���� �̸����� ���������ش�.
        const std::string& PrevParticleName = m_ParticleClass->GetName();

        CResourceManager::GetInst()->GetParticleManager()->ChangeParticleKeyName(PrevParticleName, FileName);

        // Resource Display Window �ٽ� Update
        CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedParticleResources();
        
        // ���� �����ϴ� ���� �̸����� PArticle Class �� �̸� �����ϱ� 
        m_ParticleClass->SetName(FileName);

        // �ش� Particle File �����ϱ� 
        m_ParticleClass->SaveFile(FileFullPathMultibyte);

        // IMGUI �ѹ� �� Update
        SetIMGUIReflectParticle(m_ParticleClass);
    }
}

void CEffectEditor::OnLoadParticleClass()
{
   //  if (!m_ParticleObject || !dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle())
   //      return;

    // Ȥ�� �𸣴� Particle Material ���� ���� ������ ������ش�.
    const PathInfo* ParticlePathInfo = CPathManager::GetInst()->FindPath(PARTICLE_PATH);
    CEngineUtil::CheckAndMakeDirectory(ParticlePathInfo);

    TCHAR FilePath[MAX_PATH] = {};

    OPENFILENAME OpenFile = {};
    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Particle File\0*.prtc");
    OpenFile.lpstrFile = FilePath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = ParticlePathInfo->Path;

    if (GetOpenFileName(&OpenFile) != 0)
    {
        // ��� ����
        char	Ext[_MAX_EXT] = {};

        char FilePathMultibyte[MAX_PATH] = {};
        char FileName[MAX_PATH] = {};

        int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

        _splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, FileName, MAX_PATH, Ext, _MAX_EXT);

        // ���� Load�Ǵ� Directory�� Bin/ParticleClass ���� Ȯ���ϱ� => �ƴ϶��, Load ����
        std::string PathInfoBeforeFileName;
        CEditorUtil::GetPathInfoBeforeFileName(FilePathMultibyte, PathInfoBeforeFileName);

        if (strcmp(ParticlePathInfo->PathMultibyte, PathInfoBeforeFileName.c_str()) != 0)
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Particle Class �� ���, �ݵ�� Bin/ParticleClass ���� Load �ؾ� �Ѵ�."), NULL, MB_OK);
            return;
        }

        _strupr_s(Ext);

        // Ȯ���� .anim �� �ƴ϶�� return;
        if (strcmp(Ext, ".PRTC") != 0)
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Ȯ���ڰ� .prtc �̾�� �մϴ�."), NULL, MB_OK);
            return;
        }

        // Edit State ó�� 
        m_StartEdit = true;

        // Particle ���� Load �ϱ�
        // SAFE_DELETE(m_ParticleClass); --> Particle Manager ���� �˾Ƽ� �������� ���̴�.
        m_ParticleClass = CResourceManager::GetInst()->CreateParticleEmpty<CParticle>();
        m_ParticleClass->LoadFile(FilePathMultibyte);

        // ���� Particle Object �� ī�޶� ���� ���� ���
        BackUpParticleObjectInfo();

         // �ʿ��� Object ��� ���� => Particle Object �� ����
        SetGameObjectReady();

        // Particle Object Enable ó��
        m_ParticleObject->Enable(true);
        m_ParticleObject->GetRootComponent()->Enable(true);

        // ��� ���� �ݿ�
        ReflectBackUpParticleObjectInfo();

        // Particle Component �� Particle ����
        SetParticleToParticleComponent(dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent()), m_ParticleClass);

        // IMGUI Update
        SetIMGUIReflectParticle(m_ParticleClass);
        SetIMGUIReflectObjectCamera();

        // Particle Manager �� Map �� �߰��ϱ�
        CResourceManager::GetInst()->GetParticleManager()->AddParticle(m_ParticleClass);

        // Resource Display Window �����ϱ�
        CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedParticleResources();
        CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedMaterialResources();
    }
}

void CEffectEditor::OnSetBasicParticleMaterialSetting(CSceneComponent* Com)
{
    // �⺻ Particle Setting, ���� Component �� Particle Setting �ϱ�
    // 1) Particle Material ����
    CSceneManager::GetInst()->GetScene()->GetResource()->CreateMaterial<CMaterial>("BasicParticleMaterial");
    m_ParticleMaterial = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BasicParticleMaterial");
    m_ParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Bubble", TEXT("Particle/Bubbles99px.png"));
    m_ParticleMaterial->SetShader("ParticleRenderShader");
    m_ParticleMaterial->SetRenderState("AlphaBlend");

    // 2) Particle ����
    CSceneManager::GetInst()->GetScene()->GetResource()->CreateParticle("BasicParticle");
    m_ParticleClass = CSceneManager::GetInst()->GetScene()->GetResource()->FindParticle("BasicParticle");
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetMaterial(m_ParticleMaterial);

    SetBasicDefaultParticleInfos(m_ParticleClass);

    // �ش� ������ UI �� �����ϱ�
    SetParticleToParticleComponent(dynamic_cast<CParticleComponent*>(Com), "BasicParticle");

    // �⺻ Z Pos ����
    Com->SetWorldPos(Com->GetWorldPos().x, Com->GetWorldPos().y, 10.f);

    // Resource Window Display
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedTextureResources();
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedMaterialResources();
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedParticleResources();
}

void CEffectEditor::OnReflectCurrentParticleSetting()
{
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->Set2D(false);
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetSpawnCountMax(m_SpawnCountMaxEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetSpawnTime(m_SpawnTimeMaxEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetLifeTimeMin(m_LifeTimeMinEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetLifeTimeMax(m_LifeTimeMaxEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetScaleMin(m_ScaleMinEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetScaleMax(m_ScaleMaxEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetSpeedMin(m_SpeedMinEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetSpeedMax(m_SpeedMaxEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetMoveDir(m_MoveDirEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetStartMin(m_StartMinEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetStartMax(m_StartMaxEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetColorMin(m_ColorMinEdit->GetRGBA());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetColorMax(m_ColorMaxEdit->GetRGBA());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetMoveAngle(m_MoveAngleEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetGravity(m_IsGravityEdit->GetCheck(0));
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetMove(m_IsMoveEdit->GetCheck(0));
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetApplyRandom(m_IsRandomMoveEdit->GetCheck(0));
}
void CEffectEditor::SetGameObjectReady()
{
    // SkyObject
   // m_SkyObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSkyObject>("Particle Effect Editor Sky");
   // m_SkyObject->GetRootComponent()->SetLayerName("ParticleEditorLayer");
   // m_SkyObject->SetScene(CSceneManager::GetInst()->GetScene());
   // m_SkyObject->Init();

    // Particle Object ����
    if (m_ParticleObject)
    {
        m_ParticleObject->Destroy();
    }

    m_ParticleObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<C3DParticleObject>("Particle Effect Base Ground");
    // ó������ Enable False �� �Ͽ� ������ �ʰ� �Ѵ�.
    m_ParticleObject->Enable(false);
    m_ParticleObject->GetRootComponent()->Enable(false);

    // ���� Base Ground Object �� �����.
    if (!m_BaseGroundObject)
    {
        m_BaseGroundObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CGameObject>("Particle Effect Base Ground");
        m_BaseGroundObject->CreateComponent<CSpriteComponent>("Simple Sprite");
        m_BaseGroundObject->GetRootComponent()->SetLayerName("ParticleEditorLayer");
        m_BaseGroundObject->SetPivot(0.5, 0.5, 0.5);
        m_BaseGroundObject->SetWorldScale(300.f, 300.f, 1.f);
        m_BaseGroundObject->AddWorldRotationX(90.f);
        m_BaseGroundObject->AddWorldPos(0.f, -30.f, 0.f);
        m_BaseGroundObject->ExcludeFromSceneSave();

        CSpriteComponent* BaseGroundComponent = dynamic_cast<CSpriteComponent*>(m_BaseGroundObject->GetRootComponent());
        BaseGroundComponent->SetMaterial(CResourceManager::GetInst()->FindMaterial("ParticleEditorBaseGround"));

        // BaseGround Object �� ũ�⸦ IMGUI �� �ݿ��ϱ�
        const Vector3& WorldScale = m_BaseGroundObject->GetWorldScale();
        m_GroundTextureScale->SetValue(WorldScale.x);
    }
}

void CEffectEditor::SetBasicDefaultParticleInfos(CParticle* Particle)
{
    // �ݵ�� 3D �� �����Ѵ�.
    Particle->Set2D(false);

    // Spawn Time, Count
    Particle->SetSpawnCountMax(1000);
    Particle->SetSpawnTimeMax(0.05f);

    // Life Time
    Particle->SetLifeTimeMin(5.f);
    Particle->SetLifeTimeMax(10.f);

    // Scale
    Particle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
    Particle->SetScaleMax(Vector3(50.f, 50.f, 1.f));

    // Speed
    Particle->SetSpeedMin(10.f);
    Particle->SetSpeedMax(30.f);

    // Start Min,Max
    Particle->SetStartMin(Vector3(-30.f, -30.f, 0.f));
    Particle->SetStartMax(Vector3(30.f, 30.f, 0.f));

    // Color Min, Max
    Particle->SetColorMin(Vector4(0.2f, 0.1f, 0.8f, 1.f));
    Particle->SetColorMax(Vector4(0.6f, 0.8f, 0.8f, 1.f));

    // Alpha
    Particle->SetMinAlpha(1.f);
    Particle->SetMaxAlpha(1.f);

    // Move Dir
    Particle->SetMoveDir(Vector3(0.f, 1.f, 0.f)); 
    // Move Angle
    Particle->SetMoveAngle(Vector3(0.f, 0.f, 30.f));

    // IsGravity
    // IsMove
    // IsRandomMove
    // IsPauseResume -> ������ Enable true �� ���� �����Ѵ�.
    Particle->SetGravity(true);
    Particle->SetMove(true);
    Particle->Enable(true);

    // Bounce
    Particle->SetBounceEnable(false);
    Particle->SetBounceResistance(0.98f);
}

void CEffectEditor::SetStartEditing()
{
    if (m_StartEdit)
        return;

    m_StartEdit = true;

    // �̹� Particle Object �� ������� �ִٸ� => Particle Editing �� ������ �����̹Ƿ�
    m_ParticleObject->Enable(true);
    m_ParticleObject->GetRootComponent()->Enable(true);

    OnSetBasicParticleMaterialSetting(m_ParticleObject->GetRootComponent());

    SetIMGUIReflectParticle(m_ParticleClass);

    SetIMGUIReflectObjectCamera();

}

void CEffectEditor::OnDropMaterialToParticle(const std::string& InputName)
{

    if (!m_ParticleClass)
    {
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("No Particle Set"), NULL, MB_OK);
        
        // �ٽ� ���� ���� ������� �Ѵ�.
        m_CurrentMaterialName->ResetText();

        return;
    }

    // Key �� ���·� Resource Manager ���� Mateirla �� ���� ã�´�.
    CMaterial* FoundMaterial = CResourceManager::GetInst()->FindMaterial(InputName);

    if (FoundMaterial)
    {
        ApplyNewMaterial(FoundMaterial);
        return;
    }

    // �� ���� �ϵ� ��ũ���� ã�´�.
    // Drop �� Input �ȿ� �ִ� Texture ���� �ҷ����� 
    // Texture File �� File ����, Texture �� �����ϴ� Key ������ ����� ���̴�.
    std::string MaterialKeyName;

    // Bin//Material//ParticleClass ��� �������� ã�� ���̴�.
    // std::optional<std::string> FoundResult = CEditorUtil::GetFullPathOfTargetFileNameInDir(MATERIAL_PATH, InputName, MaterialKeyName);
    std::optional<std::string> FoundResult = CEditorUtil::GetFullPathOfTargetFileNameInDir(MATERIAL_PARTICLE_PATH, InputName, MaterialKeyName);

    // ã�� ���ߴٸ� 
    if (!FoundResult.has_value())
    {
        // New Texture Load Failure Message Box
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Load Failure From Bin/Material/ParticleMaterial"), NULL, MB_OK);

        // �ٽ� ���� ���� ������� �Ѵ�.
        m_CurrentMaterialName->ResetText();

        return;
    }

    // Ȯ���� Ȯ��
    std::string ExtractFileName;
    std::string ExtractFileExt;

    CEditorUtil::ExtractFileNameAndExtFromPath(FoundResult.value(), ExtractFileName, ExtractFileExt);

    // �빮��ȭ
    std::transform(ExtractFileExt.begin(), ExtractFileExt.end(), ExtractFileExt.begin(), ::toupper);
    
    if (ExtractFileExt != ".MTRL")
    {
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Ȯ���ڰ� .mtrl�� �ƴմϴ�."), NULL, MB_OK);

        // �ٽ� ���� ���� ������� �Ѵ�.
        m_CurrentMaterialName->ResetText();

        return;
    }

    // Load �õ�
    FoundMaterial = CResourceManager::GetInst()->LoadMaterialFullPathMultibyte(FoundResult.value().c_str());
    
    if (!FoundMaterial)
    {
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Load Process Failure"), NULL, MB_OK);

        // �ٽ� ���� ���� ������� �Ѵ�.
        m_CurrentMaterialName->ResetText();

        return;
    }

    // Hard Disk �κ���, File�� Drop���� ��쿡��, FileName�� �����Ѵ�.
    m_LoadedMaterialFileName->SetText((ExtractFileName + ".mtrl").c_str());
    
    ApplyNewMaterial(FoundMaterial);
}

void CEffectEditor::ApplyNewMaterial(CMaterial* FoundMaterial)
{
    if (!m_ParticleClass)
    {
        // ����� ���õǾ��ٴ� Message
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Particle�� Setting ���� �ʾҽ��ϴ�."), NULL, MB_OK);

        // Ȥ�ó� Material Loaded FileName �̸��� �ش� �Լ��� ������ ���� ���õǾ��� ���� �����Ƿ� 
        // �ٽ� �ǵ��� ���´�.
        m_LoadedMaterialFileName->ResetText();

        return;
    }

    if (!FoundMaterial)
        return;

    m_CurrentMaterialName->SetText(FoundMaterial->GetName().c_str());

    m_ParticleClass->SetMaterial(FoundMaterial);
    m_ParticleMaterial = FoundMaterial;

    // �ش� Material �� Texture �� �ҷ��ͼ� Image �� �����ϱ� => ù��° Texture �� �������ش�.
    if (FoundMaterial->GetTextureInfo().size() > 0)
        m_ParticleTexture->SetTexture(FoundMaterial->GetTexture());

    // Particle �� Material ������ Particle Component �� �ٽ� �ݿ��Ѵ�.
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->SetMaterial(m_ParticleClass->CloneMaterial());

    // ����� ���õǾ��ٴ� Message
    MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Set SuccessFully"), NULL, MB_OK);

    // Resource Window Display Update
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedTextureResources();
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedMaterialResources();
}

void CEffectEditor::BackUpParticleObjectInfo()
{
    if (!m_ParticleObject)
        return;

    m_BackUpStruct.IsCameraRotate = m_ParticleObject->IsCameraRotate();
    m_BackUpStruct.IsCameraRotateInv = m_ParticleObject->IsCameraRotateInv();
    m_BackUpStruct.IsCameraRotateSpeed = m_ParticleObject->GetCameraRotateSpeed();
    m_BackUpStruct.IsCameraZoom = m_ParticleObject->IsCameraZoom();
    m_BackUpStruct.CameraZoomSpeed = m_ParticleObject->GetCameraZoomSpeed();


    m_BackUpStruct.CameraYOffset = m_ParticleObject->GetCameraYOffset();
    m_BackUpStruct.CameraXRot = m_ParticleObject->GetCameraRelativeRotation().x;
    m_BackUpStruct.TargetDist = m_ParticleObject->GetArmComponent()->GetTargetDistance();
    m_BackUpStruct.WorldRotY = m_ParticleObject->GetCameraYOffset();
}

void CEffectEditor::ReflectBackUpParticleObjectInfo()
{
    if (!m_ParticleObject)
        return;

    m_ParticleObject->SetCameraRotate(m_BackUpStruct.IsCameraRotate);
    m_ParticleObject->SetRotateInv(m_BackUpStruct.IsCameraRotateInv);
    m_ParticleObject->SetCameraRotateSpeed(m_BackUpStruct.IsCameraRotateSpeed);
    m_ParticleObject->SetCameraZoom(m_BackUpStruct.IsCameraZoom);
    m_ParticleObject->SetCameraZoomSpeed(m_BackUpStruct.CameraZoomSpeed);

    m_ParticleObject->GetArmComponent()->SetOffset(0.f, m_BackUpStruct.CameraYOffset, 0.f);
    m_ParticleObject->GetArmComponent()->SetRelativeRotation(m_BackUpStruct.CameraXRot, 0.f, 0.f);
    m_ParticleObject->GetArmComponent()->SetTargetDistance(m_BackUpStruct.TargetDist);
    m_ParticleObject->GetArmComponent()->SetRelativeRotationY(m_BackUpStruct.WorldRotY);
}

void CEffectEditor::SetParticleToParticleComponent(CParticleComponent* Component,  const char* ParticleName)
{
    if (!Component)
        return;

    Component->SetParticle(ParticleName);
}

void CEffectEditor::SetParticleToParticleComponent(CParticleComponent* Component, CParticle* Particle)
{
    if (!Component)
        return;

    Component->SetParticle(Particle);
}

void CEffectEditor::SetIMGUIReflectParticle(CParticle* Particle)
{
    // Particle �̸� ����
    m_CurrentParticleName->SetText(Particle->GetName().c_str());

    // Material �̸� ����
    if (Particle->GetMaterial())
    {
        std::string MaterialName = Particle->GetMaterial()->GetName();
        m_CurrentMaterialName->SetText(MaterialName.c_str());

        // Material File Name ����
        if (MaterialName.find(".mtrl") == std::string::npos)
            MaterialName.append(".mtrl");
        
        m_LoadedMaterialFileName->SetText(MaterialName.c_str());
    }

    // �ݵ�� 3D �� �����Ѵ�.
    Particle->Set2D(false);

    m_SpawnCountMaxEdit->SetVal(Particle->GetSpawnCountMax());
    m_SpawnTimeMaxEdit->SetVal(Particle->GetSpawnTimeMax());

    m_LifeTimeMinEdit->SetVal(Particle->GetLifeTimeMin());
    m_LifeTimeMaxEdit->SetVal(Particle->GetLifeTimeMax());

    m_ScaleMinEdit->SetVal(Particle->GetScaleMin());
    m_ScaleMaxEdit->SetVal(Particle->GetScaleMax());

    m_SpeedMinEdit->SetVal(Particle->GetSpeedMin());
    m_SpeedMaxEdit->SetVal(Particle->GetSpeedMax());

    m_StartMinEdit->SetVal(Particle->GetStartMin());
    m_StartMaxEdit->SetVal(Particle->GetStartMax());

    m_ColorMinEdit->SetRGBA(Particle->GetColorMin());
    m_ColorMaxEdit->SetRGBA(Particle->GetColorMax());

    m_AlphaMinEdit->SetVal(Particle->GetMinAlpha());
    m_AlphaMaxEdit->SetVal(Particle->GetMaxAlpha());

    m_MoveDirEdit->SetVal(Particle->GetMoveDir());
    m_MoveAngleEdit->SetVal(Particle->GetMoveAngle());

    m_IsGravityEdit->SetCheck(0, Particle->GetGravity());
    m_IsMoveEdit->SetCheck(0, Particle->GetMove());
    m_IsPauseResumeToggle->SetCheck(0, true);

    // Bounce
    m_IsBounce->SetCheck(0, Particle->IsBounceEnable() == 1 ? true : false);
    m_BounceResistance->SetValue(Particle->GetBounceResistance());

    // Generate Circle
    m_IsGenerateCircle->SetCheck(0, Particle->IsGenerateCircle() == 1 ? true : false);
    m_GenerateCircleRadius->SetValue(Particle->GetGenerateCircleRadius());
    m_IsLoopGenerateCircle->SetCheck(0, Particle->IsLoopGenerateCircle());
}

void CEffectEditor::SetIMGUIReflectObjectCamera()
{
    // ���ο� Particle Object �� ������ ���õǾ��� Camera ���� ������ �ݿ����ش�.

    m_IsRotateEdit->SetCheck(0, m_ParticleObject->IsCameraRotate());
    m_IsRotateInv->SetCheck(0, m_ParticleObject->IsCameraRotateInv());
    m_RotateSpeedSliderBar->SetValue(m_ParticleObject->GetCameraRotateSpeed());
    m_IsZoomEdit->SetCheck(0, m_ParticleObject->IsCameraZoom());
    m_ZoomSpeedSliderBar->SetValue(m_ParticleObject->GetCameraZoomSpeed());

    m_CameraYOffsetBar->SetValue(m_ParticleObject->GetCameraYOffset());
    m_CameraXRotSlideBar->SetValue(m_ParticleObject->GetCameraRelativeRotation().x);
}
