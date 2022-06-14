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
#include "Component/ParticleComponent.h"
#include "Component/SpriteComponent.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Render/RenderManager.h"
#include "Resource/Particle/ParticleManager.h"
#include "Scene/SceneResource.h"
#include "GameObject/SkyObject.h"

CEffectEditor::CEffectEditor()
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
    m_ParticleRenderTarget = AddWidget<CIMGUIImage>("Render Target", 500.f, 400.f);
    m_ParticleRenderTarget->SetRenderTargetImage(true);
    m_ParticleRenderTarget->SetTexture(CRenderManager::GetInst()->GetParticleEffectRenderTarget());
    m_ParticleRenderTarget->SetBorderColor(10, 10, 255);
    m_ParticleRenderTarget->SetTableTitle("Render Target");

    // Particle Texture
    m_ParticleTexture = AddWidget<CIMGUIImage>("Particle Texture", 200.f, 200.f);
    // m_ParticleRenderTarget = AddWidget<CIMGUIImage>("Render Target", 500.f, 500.f);
    // m_ParticleRenderTarget->SetRenderTargetImage(true);
    // m_ParticleRenderTarget->SetTexture(CRenderManager::GetInst()->GetParticleEffectRenderTarget());
    m_ParticleTexture->SetBorderColor(10, 10, 255);
    m_ParticleTexture->SetTableTitle("Texture");

    // Material
    m_MaterialName = AddWidget<CIMGUITextInput>("Current Material", 150.f, 30.f);
    m_MaterialName->SetHideName(true);
    m_MaterialName->SetHintText("Current Material");
    m_MaterialName->SetDropCallBack<CEffectEditor>(this, &CEffectEditor::OnDropMaterialToParticle);

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

    m_IsRandomMoveEdit = Tree->AddWidget<CIMGUICheckBox>("Random", 80.f);
    m_IsRandomMoveEdit->AddCheckInfo("Random");
    m_IsRandomMoveEdit->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsRandomMoveEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(260.f);

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

    // Move Dir, Angle
    Tree = AddWidget<CIMGUITree>("Move Angle, Dir");

    m_MoveDirEdit = Tree->AddWidget<CIMGUIInputFloat3>("Move Dir", 150.f);
    m_MoveDirEdit->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnMoveDirEdit);

    m_MoveAngleEdit = Tree->AddWidget<CIMGUIInputFloat3>("Move Angle", 150.f);
    m_MoveAngleEdit->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnMoveAngleEdit);

    // m_GravityAccelEdit = AddWidget<CIMGUIInputFloat>("Gravity Accel", 100.f);
    // m_StartDelayEdit = AddWidget<CIMGUIInputFloat>("Start Delay T// ", 100.f);

    // Camera ����
    // OnSetCameraSetting();

    SetGameObjectReady();

    // OnSetParticleMaterialSetting(m_ParticleSampleObject->GetRootComponent());
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
    // SAFE_DELETE(m_ParticleObject);
    m_CameraXRotSlideBar->SetValue(m_ParticleObject->GetCameraRelativeRotation().x);
    m_CameraYOffsetBar->SetValue(m_ParticleObject->GetCameraOfffset().y);

    m_ParticleObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<C3DParticleObject>("Particle Effect Base Ground");

    SetParticleToParticleComponent(dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent()), m_ParticleClass);
}

void CEffectEditor::OnSpawnTimeMaxEdit(float Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetSpawnTime(m_SpawnTimeMaxEdit->GetVal());
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

    m_ParticleClass->SetColorMin(Color);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetColorMin(Color);
    // m_ParticleComponent->GetCBuffer()->SetColorMin(Color);
}

void CEffectEditor::OnColorMaxEdit(const Vector4& Color)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetColorMax(Color);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetColorMax(Color);
    // m_ParticleComponent->GetCBuffer()->SetColorMax(Color);
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

void CEffectEditor::OnIsRandomMoveEdit(const char*, bool Enable)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetApplyRandom(Enable);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetApplyRandom(Enable);
    // m_ParticleComponent->GetCBuffer()->SetApplyRandom(Enable);
}

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

        // �ٲ� Particle ������ Clone �ؼ� �ٽ� �����Ѵ�.
        // m_ParticleComponent->SetParticle("BasicParticle");

        // ���� ���� ������ �״�� �ݿ��Ѵ�.
        OnReflectCurrentParticleSetting();
    }
}

void CEffectEditor::OnSaveParticleClass()
{
    if (!m_ParticleObject || !dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle())
        return;

    TCHAR FiileFullPath[MAX_PATH] = {};

    OPENFILENAME OpenFile = {};
    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Animation File\0*.anim");
    OpenFile.lpstrFile = FiileFullPath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(PARTICLE_PATH)->Path;

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

        m_ParticleClass->SaveFile(FileFullPathMultibyte);
    }
}

void CEffectEditor::OnLoadParticleClass()
{
   //  if (!m_ParticleObject || !dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle())
   //      return;

    TCHAR FilePath[MAX_PATH] = {};

    OPENFILENAME OpenFile = {};
    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Animation File\0*.anim");
    OpenFile.lpstrFile = FilePath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(PARTICLE_PATH)->Path;

    if (GetOpenFileName(&OpenFile) != 0)
    {
        char	Ext[_MAX_EXT] = {};

        char FilePathMultibyte[MAX_PATH] = {};
        char FileName[MAX_PATH] = {};

        int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

        _splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, FileName, MAX_PATH, Ext, _MAX_EXT);

        _strupr_s(Ext);

        // Ȯ���� .anim �� �ƴ϶�� return;
        if (strcmp(Ext, ".PRTC") != 0)
            return;

        SAFE_DELETE(m_ParticleClass);

        m_ParticleClass = CResourceManager::GetInst()->CreateParticleEmpty<CParticle>();

        m_ParticleClass->LoadFile(FilePathMultibyte);

         // �ʿ��� Object ��� ����
        SetGameObjectReady();

        // �̹� Particle Object �� ������� �ִٸ� => Particle Editing �� ������ �����̹Ƿ�
        m_BaseGroundObject->GetRootComponent()->Enable(true);

        // Particle �� �����ֱ� ���� Particle Component �� ������
        // m_Particle �� �������ְ�
        m_ParticleObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<C3DParticleObject>("Particle Effect Base Ground");

        m_CameraXRotSlideBar->SetValue(m_ParticleObject->GetCameraRelativeRotation().x);
        m_CameraYOffsetBar->SetValue(m_ParticleObject->GetCameraOfffset().y);

        SetParticleToParticleComponent(dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent()), m_ParticleClass);

  
        // IMGUI Update
        SetIMGUIReflectPartice(m_ParticleClass);

        // Particle Manager �� Map �� �߰��ϱ�
        CResourceManager::GetInst()->GetParticleManager()->AddParticle(m_ParticleClass);

        // Resource Display Window �����ϱ�
        CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedParticleResources();
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

    SetIMGUIReflectPartice(m_ParticleClass);

    // �ش� ������ UI �� �����ϱ�
    SetParticleToParticleComponent(dynamic_cast<CParticleComponent*>(Com), "BasicParticle");


    // Move ����, Gravity ����, Random ���� �����ϱ�
    m_IsMoveEdit->SetCheck(0, m_ParticleClass->GetCBuffer()->GetMove());
    m_IsGravityEdit->SetCheck(0, m_ParticleClass->GetCBuffer()->GetGravity());
    m_IsRandomMoveEdit->SetCheck(0, m_ParticleClass->GetCBuffer()->GetApplyRandom());
    
    // Particle ����

    // �⺻ Z Pos ����
    Com->SetWorldPos(Com->GetWorldPos().x, Com->GetWorldPos().y, 10.f);

    // Resource Window Display
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedTextureResources();
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedMaterialResources();
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

    // ���� Base Ground Object �� �����.
    if (m_BaseGroundObject)
        m_BaseGroundObject->Destroy();

    m_BaseGroundObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CGameObject>("Particle Effect Base Ground");
    m_BaseGroundObject->CreateComponent<CSpriteComponent>("Simple Sprite");
    m_BaseGroundObject->GetRootComponent()->SetLayerName("ParticleEditorLayer");
    m_BaseGroundObject->SetPivot(0.5, 0.5, 0.5);
    m_BaseGroundObject->SetWorldScale(300.f, 300.f, 1.f);
    m_BaseGroundObject->AddWorldRotationX(90.f);
    m_BaseGroundObject->AddWorldPos(0.f, -30.f, 0.f);
    m_BaseGroundObject->ExcludeFromSceneSave();

    // ó������ Enable �� false �� �ּ� �׸��� �ʰ� �Ѵ�.
    m_BaseGroundObject->GetRootComponent()->Enable(false);

    // m_ParticleSampleObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CGameObject>("Particle Object");
    // m_ParticleSampleObject->CreateComponent<CParticleComponent>("Particle Sprite");
    // m_ParticleSampleObject->GetRootComponent()->SetLayerName("ParticleEditorLayer");
    // 
    // CCameraComponent* ParticleCamera = m_ParticleSampleObject->CreateComponent<CCameraComponent>("Particle Camera");
    // m_ParticleSampleObject->GetRootComponent()->AddChild(ParticleCamera);
    // 
    // // Camera Setting
    // CSceneManager::GetInst()->GetScene()->GetCameraManager()->SetParticleEditorCamera(ParticleCamera);
    // 
    // ParticleCamera->SetCameraType(Camera_Type::Camera3D);
    // ParticleCamera->SetViewAngle(27.f);
    // 
    // ParticleCamera->AddRelativePos(0.f, 30.f, -100.f);
    // ParticleCamera->SetRelativeRotationX(45.f); 
}

void CEffectEditor::SetStartEditing()
{
    // �̹� Particle Object �� ������� �ִٸ� => Particle Editing �� ������ �����̹Ƿ�
    if (m_BaseGroundObject->GetRootComponent()->IsEnable() == false)
        m_BaseGroundObject->GetRootComponent()->Enable(true);

    SAFE_DELETE(m_ParticleObject);

    m_ParticleObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<C3DParticleObject>("Particle Effect Base Ground");

    m_CameraXRotSlideBar->SetValue(m_ParticleObject->GetCameraRelativeRotation().x);
    m_CameraYOffsetBar->SetValue(m_ParticleObject->GetCameraOfffset().y);

    OnSetBasicParticleMaterialSetting(m_ParticleObject->GetRootComponent());
}

void CEffectEditor::OnDropMaterialToParticle(const std::string& InputName)
{
    if (!m_ParticleClass)
    {
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("No Particle Set"), NULL, MB_OK);
        return;
    }

    // Key �� ���·� Resource Manager ���� ���� ã�´�.
    CMaterial* FoundMaterial = CResourceManager::GetInst()->FindMaterial(InputName);

    if (FoundMaterial)
    {
        // �ش� Material �� Texture �� �ҷ��ͼ� Image �� �����ϱ�
        // -> Material �� Particle �� �����Ѵ�.
        m_ParticleClass->SetMaterial(FoundMaterial);

        // �ش� Material �� Texture �� �ҷ��ͼ� Image �� �����ϱ� => ù��° Texture �� �������ش�.
        if (FoundMaterial->GetTextureInfo().size() > 0)
            m_ParticleTexture->SetTexture(FoundMaterial->GetTexture());

        // ����� ���õǾ��ٴ� Message
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Set SuccessFully"), NULL, MB_OK);

        return;
    }

    // �� ���� �ϵ� ��ũ���� ã�´�.
    // Drop �� Input �ȿ� �ִ� Texture ���� �ҷ����� 
    // Texture File �� File ����, Texture �� �����ϴ� Key ������ ����� ���̴�.
    std::string MaterialKeyName;

    std::optional<std::string> FoundResult = CEditorUtil::GetFullPathOfTargetFileNameInDir(MATERIAL_PATH,
        InputName, MaterialKeyName);

    // ã�� ���ߴٸ� 
    if (!FoundResult.has_value())
    {
        // New Texture Load Failure Message Box
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Load Failure From HardDisk"), NULL, MB_OK);
        return;
    }

    FoundMaterial = CResourceManager::GetInst()->LoadMaterialFullPathMultibyte(FoundResult.value().c_str());
    
    if (!FoundMaterial)
    {
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Load Failure From HardDisk"), NULL, MB_OK);
        return;
    }

    m_ParticleClass->SetMaterial(FoundMaterial);

    // �ش� Material �� Texture �� �ҷ��ͼ� Image �� �����ϱ� => ù��° Texture �� �������ش�.
    if (FoundMaterial->GetTextureInfo().size() > 0)
        m_ParticleTexture->SetTexture(FoundMaterial->GetTexture());

    // ����� ���õǾ��ٴ� Message
    MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Set SuccessFully"), NULL, MB_OK);

    // Resource Window Display Update
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedTextureResources();
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedMaterialResources();

    m_ParticleMaterial = FoundMaterial;
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

void CEffectEditor::SetIMGUIReflectPartice(CParticle* Particle)
{
    // �ݵ�� 3D �� �����Ѵ�.
    Particle->Set2D(false);

    Particle->SetSpawnCountMax(1000);
    m_SpawnCountMaxEdit->SetVal(Particle->GetSpawnCountMax());

    Particle->SetLifeTimeMin(70.f);
    m_LifeTimeMinEdit->SetVal(Particle->GetLifeTimeMin());

    Particle->SetLifeTimeMax(80.f);
    m_LifeTimeMaxEdit->SetVal(Particle->GetLifeTimeMax());

    Particle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
    m_ScaleMinEdit->SetVal(Particle->GetScaleMin());

    Particle->SetScaleMax(Vector3(50.f, 50.f, 1.f));
    m_ScaleMaxEdit->SetVal(Particle->GetScaleMax());

    Particle->SetSpeedMin(10.f);
    m_SpeedMinEdit->SetVal(Particle->GetSpeedMin());

    Particle->SetSpeedMax(30.f);
    m_SpeedMaxEdit->SetVal(Particle->GetSpeedMax());

    Particle->SetMoveDir(Vector3(0.f, 1.f, 1.f)); // Z �� �������ε� �̵��� �� �ְ� �����Ѵ�.
    m_MoveDirEdit->SetVal(Particle->GetMoveDir());

    Particle->SetStartMin(Vector3(-30.f, -30.f, 0.f));
    m_StartMinEdit->SetVal(Particle->GetStartMin());

    Particle->SetStartMax(Vector3(30.f, 30.f, 0.f));
    m_StartMaxEdit->SetVal(Particle->GetStartMax());

    Particle->SetColorMin(Vector4(0.2f, 0.1f, 0.8f, 1.f));
    m_ColorMinEdit->SetRGBA(Particle->GetColorMin());

    Particle->SetColorMax(Vector4(0.2f, 0.1f, 0.8f, 1.f));
    m_ColorMaxEdit->SetRGBA(Particle->GetColorMax());

    Particle->SetMoveAngle(Vector3(0.f, 0.f, 30.f));
    m_MoveAngleEdit->SetVal(Particle->GetMoveAngle());

    Particle->SetSpawnTime(0.05f);
    m_SpawnTimeMaxEdit->SetVal(0.05f);

    Particle->SetGravity(true);
    Particle->SetMove(true);
}
