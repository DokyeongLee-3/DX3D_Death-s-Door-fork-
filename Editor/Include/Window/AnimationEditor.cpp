// IMGUI
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUICheckBox.h"
#include "IMGUIDummy.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIProgressBar.h"
#include "IMGUIChild.h"
#include "IMGUIText.h"
#include "IMGUITable.h"
#include "IMGUITextInput.h"
#include "IMGUISliderFloat.h"
#include "IMGUISliderInt.h"
// Engine
#include "AnimationEditor.h"
#include "../EditorManager.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Animation/AnimationSequenceInstance.h"
#include "../EditorUtil.h"
#include "Engine.h"
#include "Device.h"
#include "../Object/Anim3DObject.h"
#include "Render/RenderManager.h"
#include "Resource/ResourceManager.h"
#include "../AnimationInstanceConvertThread.h"

CAnimationEditor::CAnimationEditor() :
	m_RenderTargetSet(false)
{
}

CAnimationEditor::~CAnimationEditor()
{
	SAFE_DELETE(m_DummyAnimation);

	// �� Animation �� ���� 3DTestObject ���� ���� Animation �̴�. 
	// SAFE_DELETE(m_Animation);


	// Delete �Ͽ� ���� BoneKeyFrame ������ �����ش�.
	// while (!m_StackDeleteFrame.empty())
	// {
	// 	BoneKeyFrame* BoneFrame = m_StackDeleteFrame.top().second;
	// 	m_StackDeleteFrame.pop();
	// 
	// 	for (int i = 0; i < BoneFrame->vecKeyFrame.size(); ++i)
	// 	{
	// 		SAFE_DELETE(BoneFrame->vecKeyFrame[i]);
	// 	}
	// 	/*
	// 	for (int i = 0; i < BoneFrame->vecKeyFrame.size(); ++i)
	// 	{
	// 		--m_vecKeyFrame[i]->iRefCount;
	// 
	// 		if (m_vecKeyFrame[i]->iRefCount == 0)
	// 			delete	m_vecKeyFrame[i];
	// 	}
	// 	*/
	// }
}

bool CAnimationEditor::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	// Anim Name
	// CIMGUILabel* Label = AddWidget<CIMGUILabel>("Anim Name", 100.f, 30.f);
	// Label->SetColor(0, 0, 255);
	// Label->SetAlign(0.5f, 0.0f);

	m_CurAnimComboBox = AddWidget<CIMGUIComboBox>("Anim List Combo Box", 300.f, 30.f);
	m_CurAnimComboBox->SetHideName(true);
	m_CurAnimComboBox->SetSelectCallback<CAnimationEditor>(this, &CAnimationEditor::OnClickAnimationSequence);

	// ���� Render Target
	m_AnimationRenderTarget = AddWidget<CIMGUIImage>("Render Target", 400.f, 400.f);
	m_AnimationRenderTarget->SetRenderTargetImage(true);
	m_AnimationRenderTarget->SetBorderColor(10, 10, 255);
	m_AnimationRenderTarget->SetTableTitle("Render Target");

	// Clip Info
	m_AnimInfoTable = AddWidget<CIMGUITable>("AnimTable", 600.f, 200.f);

	// Frame Slider 
	m_FrameSlider = AddWidget<CIMGUISliderInt>("Frame Slider", 90.f, 30.f);
	m_FrameSlider->SetCallBack<CAnimationEditor>(this, &CAnimationEditor::OnAnimationSliderIntCallback);

	m_FrameInput = AddWidget<CIMGUITextInput>("Frame Input", 90.f, 30.f);
	m_FrameInput->SetTextType(ImGuiText_Type::Int);
	m_FrameInput->SetCallback<CAnimationEditor>(this, &CAnimationEditor::OnAnimationFrameInputCallback);

	// Edit  -------------------------------------------------------
	// Play Scale ����
	m_PlayScaleInput = AddWidget<CIMGUITextInput>("Play Scale Input", 50.f, 30.f);
	m_PlayScaleInput->SetHideName(true);
	m_PlayScaleInput->SetTextType(ImGuiText_Type::Float);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(60.f);

	m_PlayScaleEditBtn = AddWidget<CIMGUIButton>("Edit Scale", 90.f, 20.f);
	m_PlayScaleEditBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnEditAnimPlayScale);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(155.f);

	// Play Time ����
	m_PlayTimeInput = AddWidget<CIMGUITextInput>("Play Time Input", 50.f, 30.f);
	m_PlayTimeInput->SetHideName(true);
	m_PlayTimeInput->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	m_PlayTimeEditBtn = AddWidget<CIMGUIButton>("Edit Time", 90.f, 20.f);
	m_PlayTimeEditBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnEditAnimPlayTime);

	// ���� üũ �ڽ��� 
	m_DeltaTimeCheckBtn = AddWidget<CIMGUICheckBox>("Engine Play", 90.f, 30.f);
	m_DeltaTimeCheckBtn->AddCheckInfo("Engine Play");
	m_DeltaTimeCheckBtn->SetCallBackLabel<CAnimationEditor>(this, &CAnimationEditor::OnSetPlayEngineDeltaTime);
	// m_DeltaTimeCheckBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnSetPlayEngineDeltaTime);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(110.f);

	m_AnimationCheckBtn = AddWidget<CIMGUICheckBox>("Object Play", 90.f, 30.f);
	m_AnimationCheckBtn->AddCheckInfo("Anim Play");
	m_AnimationCheckBtn->SetCallBackLabel<CAnimationEditor>(this, &CAnimationEditor::OnPlayAnimation);
	// m_AnimationCheckBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnPlayAnimation);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(205.f);

	m_LoopEnableBtn = AddWidget<CIMGUICheckBox>("Loop", 90.f, 30.f);
	m_LoopEnableBtn->AddCheckInfo("Loop");
	m_LoopEnableBtn->SetCallBackLabel<CAnimationEditor>(this, &CAnimationEditor::OnLoopAnimation);

	// Camera
	m_RotationCheckBtn = AddWidget<CIMGUICheckBox>("Camera Rotation", 90.f, 30.f);
	m_RotationCheckBtn->AddCheckInfo("Rotate");
	m_RotationCheckBtn->SetCallBackLabel<CAnimationEditor>(this, &CAnimationEditor::OnRotateAnimationCamera);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(110.f);

	m_ZoomEnableBtn = AddWidget<CIMGUICheckBox>("Camera Zoom In", 90.f, 30.f);
	m_ZoomEnableBtn->AddCheckInfo("Zoom");
	m_ZoomEnableBtn->SetCallBackLabel<CAnimationEditor>(this, &CAnimationEditor::OnZoomAnimationCamera);

	// Sequence Make Widgets ----------------------------------------------------------------------------------------------------------------
	CIMGUIText* HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 90.f, 30.f);
	const char* AddSeqHelpText = R"(ex)  'ZedIdle' -- > pair('ZedIdle', 'ZedIdle.sqc') ���·� 
	SceneResource, ResourceManager�� m_mapSequence �� ����")";
	HelpText->SetText(AddSeqHelpText);
	HelpText->SetIsHelpMode(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 90.f, 30.f);
	const char* ResourceAnimKeyHelpText = R"(ex) 'Idle' --> m_Animation->AddAnimation('ZedIdle', 'Idle') ? 
ZedIdle Key�� ResourceManager �� mapSequence �� ����� Animation Sequence �� 
'Idle' �̶�� �̸��� Key������ AnimationInstance �� ���� �߰�)";
	HelpText->SetText(ResourceAnimKeyHelpText);
	HelpText->SetIsHelpMode(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(200.f);

	HelpText = AddWidget<CIMGUIText>("Anim Key Name Edit Help", 90.f, 30.f);
	const char* AnimInstKeyHelpText = R"(ex) 'EditIdle' --> ������ m_Animation->AddAnimation('ZedIdle', 'Idle') ���� ���� ������� m_mapAnimationSequence['Idle'] = 'ZedIdle' �� 
m_mapAnimationSequence['EditIdle'] = 'ZedIdle' �� Key �� ���� )";
	HelpText->SetText(AnimInstKeyHelpText);
	HelpText->SetIsHelpMode(true);

	// Seq Name Inputs
	m_NewAnimSeqName = AddWidget<CIMGUITextInput>("Sequence Name", 90.f, 30.f);
	m_NewAnimSeqName->SetHideName(true);
	m_NewAnimSeqName->SetHintText("Map Key");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_NewAnimSeqDataKeyName = AddWidget<CIMGUITextInput>("Sequence  Key", 90.f, 30.f);
	m_NewAnimSeqDataKeyName->SetHideName(true);
	m_NewAnimSeqDataKeyName->SetHintText("Anim Key");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(195.f);

	// Animation Instance Key Name ���� ���
	m_EditAnimSeqDataKeyName = AddWidget<CIMGUITextInput>("Edit Sequence Key", 90.f, 30.f);
	m_EditAnimSeqDataKeyName->SetHideName(true);
	m_EditAnimSeqDataKeyName->SetHintText("Edit Key");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(290.f);

	m_EditAnimKeyBtn = AddWidget<CIMGUIButton>("Edit Key", 90.f, 20.f);
	m_EditAnimKeyBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnEditAnimSequenceKey);

	// Sequence Related Btns
	HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 90.f, 30.f);
	HelpText->SetText("���� ��ο� .msh , .bne, .fbm(����) �� �����ؾ� �Ѵ�. \n ex) Idle1.sqc �� Add �Ϸ���, Idle1.msh, Idle1.bne, Idle1.fbm�� ���� ��� ���� �����ؾ� �Ѵ�.");
	HelpText->SetIsHelpMode(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(195.f);

	// Animation Related Btns
	HelpText = AddWidget<CIMGUIText>("Instance Save Btn Help Text", 90.f, 30.f);
	HelpText->SetText("Bin\\Animation ������ .anim ���� Ȯ���ڷ� �����ؾ� �Ѵ�.");
	HelpText->SetIsHelpMode(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(290.f);

	// Animation Related Btns
	HelpText = AddWidget<CIMGUIText>("Instance Load Btn Help Text", 90.f, 30.f);
	const char* InstLoadHelpText = R"(.anim ������ Load �Ϸ���, MESH_PATH (Bin//Mesh) ��ο� ���� .msh , .bne, .fbm(����) �� �����ؾ� �Ѵ�. 
ex) Alistar.anim �� Load �Ϸ���, 
MESH_PATH �� Alistar_Idle.sqc, Alistar_Idle.msh, Alistar_Idle.fbm ��, 
Alistar Animation �� ���õ� ���ϵ��� �ϳ��� ������� �Ѵ�)";
	HelpText->SetText(InstLoadHelpText);
	HelpText->SetIsHelpMode(true);

	m_AnimSequenceAddBtn = AddWidget<CIMGUIButton>("Add Seq", 90.f, 20.f);
	m_AnimSequenceAddBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnAddAnimationSequence);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_DeleteAnimSequenceBtn = AddWidget<CIMGUIButton>("Delete Seq", 90.f, 20.f);
	m_DeleteAnimSequenceBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnDeleteAnimationSequenceData);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(195.f);

	m_SaveAnimationInstanceBtn = AddWidget<CIMGUIButton>("Save Instance", 90.f, 20.f);
	m_SaveAnimationInstanceBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnSaveAnimationInstance);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(290.f);

	m_LoadAnimationInstanceBtn = AddWidget<CIMGUIButton>("Load Instance", 90.f, 20.f);
	m_LoadAnimationInstanceBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnLoadAnimationInstance);

	// Table Key �� ���� ����
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameRange);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameLength);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameMode);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::PlayTime);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameTime);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::PlayScale);

	// Animation Instance Convert Widgets ----------------------------------------------------------------------------------------------------------------
	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 200.f, 30.f);

	// m_AnimInstanceConvertThread = new CAnimationInstanceConvertThread;
	// m_AnimInstanceConvertThread->Init();
	// m_AnimInstanceConvertThread->Start();
	// m_AnimInstanceConvertThread->SetLoadingCallBack(this, &CAnimationEditor::OnAnimInstanceConvertLoading);

	// Common Name
	m_SavedAnimFileName = AddWidget<CIMGUITextInput>("Saved File Name", 150.f, 20.f);
	m_SavedAnimFileName->SetHideName(true);
	m_SavedAnimFileName->SetHintText("Saved File Name");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 90.f, 30.f);
	HelpText->SetText("ZedIdle �̶�� �ϸ�, Bin/Animation ������ ZedIdle.anim �̸����� Animation Instance ����");
	HelpText->SetIsHelpMode(true);

	m_CommonAnimSeqName = AddWidget<CIMGUITextInput>("Common Sqc Name", 150.f, 20.f);
	m_CommonAnimSeqName->SetHideName(true);
	m_CommonAnimSeqName->SetHintText("Common Sqc Name");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(165.f);

	m_SelectAnimInstanceFolderPath = AddWidget<CIMGUIButton>("Select Dir", 90.f, 20.f);
	m_SelectAnimInstanceFolderPath->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnClickSetAnimSeqSrcDirButton);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(260.f);

	m_ConvertAnimInstanceBtn = AddWidget<CIMGUIButton>("Make Inst", 90.f, 20.f);
	m_ConvertAnimInstanceBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnConvertSequencesIntoAnimationInstance);

	// Folder Path
	m_AnimSeqcSrcFolderPath = AddWidget<CIMGUITextInput>("Sqc Folder Path", 350.f, 20.f);
	m_AnimSeqcSrcFolderPath->ReadOnly(true);
	m_AnimSeqcSrcFolderPath->SetHideName(true);
	m_AnimSeqcSrcFolderPath->SetHintText("Set .sqc Folder Path");

	// Convert
	m_AnimInstanceProgressBar = AddWidget<CIMGUIProgressBar>("", 350.f, 0.f);

	m_AnimInstanceConvertLog = AddWidget<CIMGUIChild>("Log", 200.f, 200.f);
	m_AnimInstanceConvertLog->EnableBorder(true);

	return true;
}

void CAnimationEditor::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	// Animation Slider ����
	if (m_Animation && m_Animation->GetCurrentAnimation())
	{
		int NextAnimationIdx = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetCurrentFrameIdx();
		OnAnimationSliderIntCallback(NextAnimationIdx);
	}

	// ���� RenderTarget ���� (ó�� �ѹ���)
	if (!m_RenderTargetSet)
	{
		m_AnimationRenderTarget->SetTexture(CRenderManager::GetInst()->GetAnimationRenderTarget());
		// m_AnimationRenderTarget->SetTexture(CResourceManager::GetInst()->FindTexture("FinalScreen"));
		m_RenderTargetSet = true;
	}
}

void CAnimationEditor::OnClearExistingAnimationSeqInfos()
{
	if (!m_Animation)
		return;

	m_Animation->ClearAnimationSequenceFromAnimationEditor();

	// Combo Box Clear
	m_CurAnimComboBox->Clear();

	// Table Claer
	m_AnimInfoTable->ClearContents();
}

void CAnimationEditor::OnCreateSample3DObject()
{
	if (m_3DTestObject)
		return;

	m_3DTestObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CAnim3DObject>("3DObject");

	// 3DTest Object�� Animation ������ �����´�.
	m_Animation = dynamic_cast<CAnimationMeshComponent*>(m_3DTestObject->GetRootComponent())->CreateBasicAnimationInstance();
}

void CAnimationEditor::OnPlayAnimation(const char* Lable, bool Enable)
{
	if (!m_3DTestObject)
		return;

	CAnimationMeshComponent* RootMeshComponent = dynamic_cast<CAnimationMeshComponent*>(m_3DTestObject->GetRootComponent());

	if (!RootMeshComponent)
		return;

	bool IsAnimPlay = RootMeshComponent->GetAnimationInstance()->IsPlay();

	// if (IsAnimPlay)
	if (!Enable)
		RootMeshComponent->GetAnimationInstance()->Stop();
	else
		RootMeshComponent->GetAnimationInstance()->Play();
}

void CAnimationEditor::OnRotateAnimationCamera(const char*, bool Enable)
{
	if (!m_3DTestObject)
		return;

	// if (IsAnimPlay)
	if (Enable)
		m_3DTestObject->SetCameraRot(true);
	else
		m_3DTestObject->SetCameraRot(false);
}

void CAnimationEditor::OnZoomAnimationCamera(const char*, bool Enable)
{
	if (!m_3DTestObject)
		return;

	// if (IsAnimPlay)
	if (Enable)
		m_3DTestObject->SetCameraZoom(true);
	else
		m_3DTestObject->SetCameraZoom(false);
}

void CAnimationEditor::OnLoopAnimation(const char*, bool Enable)
{
	if (!m_3DTestObject)
		return;

	CAnimationMeshComponent* RootMeshComponent = dynamic_cast<CAnimationMeshComponent*>(m_3DTestObject->GetRootComponent());

	if (!RootMeshComponent)
		return;

	RootMeshComponent->GetAnimationInstance()->SetLoop(Enable);

	// bool IsAnimPlay = RootMeshComponent->GetAnimationInstance()->IsPlay();
	// 
	// // if (IsAnimPlay)
	// if (!Enable)
	// 	RootMeshComponent->GetAnimationInstance()->Stop();
	// else
	// 	RootMeshComponent->GetAnimationInstance()->Play();
}

void CAnimationEditor::OnSaveAnimationInstance()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	TCHAR FiileFullPath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Animation File\0*.anim");
	OpenFile.lpstrFile = FiileFullPath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

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
		if (strcmp(FileExt, ".ANIM") != 0)
		{
			MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("EXT Has To Be .anim"), NULL, MB_OK);
			return ;
		}

		m_Animation->SetSavedFileName(FileName);

		m_Animation->SaveAnimationFullPath(FileFullPathMultibyte);
	}
}

void CAnimationEditor::OnLoadAnimationInstance()
{
	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Animation File\0*.anim");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

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
		if (strcmp(Ext, ".ANIM") != 0)
			return;

		// Animation .anim File ����, .anim ���� Ȯ���ڷ� ����� 
		// ���� �̸����� �ĺ��� ���̴�.
		// ���� Animation File �� Load �ϴ� ���̶�� Skip
		if (m_Animation && strcmp(FileName, m_Animation->GetSavedFileName()) == 0)
			return;

		if (!m_Animation)
		{
			// ex) singed_spell2.sqc �� �����ߴٸ�
			// ���� ���� ��� ������ singed_spell2.msh / singed_spell2.bne �� Load �Ͽ� �����Ѵ�.
			// singed_spell2.msh -> singed_spell2_mesh ��� �̸�����
			// singed_spell2.bne -> singed_spell2_skeleton �̶�� �̸�����
			OnCreateSample3DObject();
		}
		else
		{
			// ���� Animation List�� �������� , ��, ���� Animation�� Added �Ǿ��� ��� Sequence ������ �����ش�
			OnClearExistingAnimationSeqInfos();
		}

		m_Animation->LoadAnimationFullPath(FilePathMultibyte);

		// CurrentAnimation �� ���ٸ�,
		if (!m_Animation->GetCurrentAnimation())
			return;

		// const char* CurSeqFileName = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetSequenceFileNameMultibyte();
		// if (!LoadElementsForSqcLoading(CurSeqFileName))
		// 	return;

		std::pair<bool, std::string> LoadResult = CResourceManager::GetInst()->LoadMeshTextureBoneInfo(m_Animation);

		if (LoadResult.first == false)
			return;

		// �ش� �̸��� 3d Test Object ���� �����ͼ� ����� ���̴�.
		m_3DTestObjectMeshName = LoadResult.second;

		// Set Material, Mesh Info
		SetMeshMaterialReadyForAnimation();

		// CameraObject �����ϱ�
		// CEditorManager::GetInst()->CreateCameraObject();

		// Animation Key Name ������ �Ѵ�. (Ȥ�ó� �̸��� �߸� ����Ǿ� ���� ���� �����Ƿ� )
		// m_Animation->AdjustSequence2DKeyName();

		// ComboBox �� ��� Seq ������ �߰����ش�
		m_CurAnimComboBox->Clear();

		int Size = m_Animation->GetAnimationCount();
		std::vector<std::string> vecSeqNames;
		vecSeqNames.reserve(Size);

		m_Animation->GatherSequenceNames(vecSeqNames);

		for (int i = 0; i < Size; i++)
		{
			m_CurAnimComboBox->AddItem(vecSeqNames[i]);
		}

		// Current Animation ������ �����Ѵ�.
		int CurAnimIdx = m_Animation->GetCurrentAnimationOrder();
		if (CurAnimIdx == -1)
			return;

		m_CurAnimComboBox->SetSelectIndex(CurAnimIdx);

		// ���� Select �� Animation �� �ּ�, �ִ� Frame ���� Frame Slider ���� ����
		m_FrameSlider->SetMin(m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetStartFrame());
		m_FrameSlider->SetMax(m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetEndFrame());

		// Table ���� ����
		OnRefreshAnimationClipTable(m_Animation->GetCurrentAnimation()->GetAnimationSequence());

		// ���� Scene�� ��� Sequence ������ �߰��Ѵ�.
		// m_Animation->AddAnimationSequenceToSceneResource();
		OnRefreshFrameSliderInfo(m_Animation->GetCurrentAnimation()->GetAnimationSequence());

		// ���� Scene�� ������ m_Scene���� �������ش�
		m_Animation->SetScene(CSceneManager::GetInst()->GetScene());

		// Refresh CheckBox Info
		OnRefreshCheckBoxInfo();

		// Animation Play Scale, Time
		OnRefreshScaleAndTimeInputInfo();


		// Animation�� �����Ѵ�..
		m_Animation->Play();

	}
}

void CAnimationEditor::OnAnimationFrameInputCallback()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// ������ �Ѿ�� �������ش�.
	int InputFrame = m_FrameInput->GetValueInt();
	int StFrame = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetStartFrame();
	int EdFrame = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetEndFrame();

	if (InputFrame < StFrame)
		InputFrame = EdFrame;
	else if (InputFrame > EdFrame)
		InputFrame = StFrame;

	m_FrameInput->SetInt(InputFrame);

	// Input �� Frame ���� Animation Frame ������ �������ش�.
	// m_Animation->SetCurrentAnimationFrameIdx(InputFrame);
	m_Animation->SetEditorStopTargetFrame(InputFrame);

}

void CAnimationEditor::OnAnimationSliderIntCallback(int CurrentFrame)
{
	m_FrameSlider->SetValue(CurrentFrame);
}

void CAnimationEditor::OnApplyAnimationSlider(CAnimationSequence* Sequence)
{
	if (!Sequence)
		return;

	m_FrameSlider->SetMin(Sequence->GetStartFrame());
	m_FrameSlider->SetMax(Sequence->GetEndFrame());
}

bool CAnimationEditor::LoadElementsForSqcLoading(const char* SqcFileName)
{
	// ���� Mesh Load �������� �ʿ��� Texture�� ���ٸ� 
	// ex) FBX Convert ����, singed_spell2.sqc �� �ִٸ�, ���� ��γ��� singed_spell2.fbm �̶�� ���丮�� �����ؾ� �Ѵ�.
	// ���� �ش� Folder �� �������� �ʴ´ٸ�, Mesh�� Load �ϴ��� Texture �� ���ٰ� �� ���̴�
	char TextFolderExt[10] = ".fbm";
	char TextFolderFullPath[MAX_PATH] = {};

	// RESOURCE_MESH_PATH �� �ִ� �����κ��� �ҷ��� ���̴�.
	char MeshFileFullPath[MAX_PATH] = {};
	
	const PathInfo* Path = CPathManager::GetInst()->FindPath(MESH_PATH);
	
	if (Path)
		strcpy_s(MeshFileFullPath, Path->PathMultibyte);

	strcpy_s(TextFolderFullPath, MeshFileFullPath);
	strcat_s(TextFolderFullPath, SqcFileName);
	strcat_s(TextFolderFullPath, TextFolderExt);

	std::filesystem::path MeshTextureFolderPath(TextFolderFullPath);

	if (!std::filesystem::exists(MeshTextureFolderPath))
	{
		MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT(".fbm Folder Does Not Exist"), NULL, MB_OK);
		return false;
	}

	// ex) singed_spell2.sqc �� �����ߴٸ�
	// ���� ���� ��� ������ singed_spell2.msh / singed_spell2.bne �� Load �Ͽ� �����Ѵ�.
	// singed_spell2.msh -> singed_spell2_mesh ��� �̸�����
	// singed_spell2.bne -> singed_spell2_skeleton �̶�� �̸�����

	// Load Mesh
	std::string LoadedMeshName = SqcFileName;
	// LoadedMeshName.append("_mesh");

	// �ش� �̸��� 3d Test Object ���� �����ͼ� ����� ���̴�.
	m_3DTestObjectMeshName = LoadedMeshName;

	char MeshExt[10] = ".msh";

	char MshFileName[MAX_PATH] = {};
	TCHAR MshTCHARFileName[MAX_PATH] = {};

	strcpy_s(MshFileName, SqcFileName);
	strcat_s(MshFileName, MeshExt);

	int ConvertLength = MultiByteToWideChar(CP_ACP, 0, MshFileName, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, MshFileName, -1, MshTCHARFileName, ConvertLength);

	if (!CSceneManager::GetInst()->GetScene()->GetResource()->LoadMesh(Mesh_Type::Animation, LoadedMeshName,
		MshTCHARFileName, MESH_PATH))
	{
		MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT(".msh Load Failure"), NULL, MB_OK);
		return false;
	}

	// Bne (Skeleton) Load
	char BneExt[10] = ".bne";

	std::string LoadedBneName = SqcFileName;
	LoadedBneName.append("_skeleton");

	char BneFileName[MAX_PATH] = {};
	TCHAR BneTCHARFileName[MAX_PATH] = {};

	strcpy_s(BneFileName, SqcFileName);
	strcat_s(BneFileName, BneExt);

	ConvertLength = MultiByteToWideChar(CP_ACP, 0, BneFileName, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, BneFileName, -1, BneTCHARFileName, ConvertLength);

	if (!CSceneManager::GetInst()->GetScene()->GetResource()->LoadSkeleton(LoadedBneName, BneTCHARFileName, MESH_PATH))
	{
		MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT(".bne Load Failure"), NULL, MB_OK);
		return false;
	}

	// Mesh �� �ش� Skeleton ����
	CSceneManager::GetInst()->GetScene()->GetResource()->SetMeshSkeleton(LoadedMeshName, LoadedBneName); //


	return true;
}

void CAnimationEditor::SetMeshMaterialReadyForAnimation()
{
	m_3DTestObject->SetMeshAndMaterialInfo();
	m_Animation->Start();
}

void CAnimationEditor::OnEditAnimPlayTime()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// if (m_PlayTimeInput->Empty())
	//	return;

	float SetPlayTime = m_PlayTimeInput->GetValueFloat();

	m_Animation->GetCurrentAnimation()->GetAnimationSequence()->SetPlayTime(SetPlayTime);
	m_Animation->GetCurrentAnimation()->SetPlayTime(SetPlayTime);

	OnRefreshAnimationClipTable(m_Animation->GetCurrentAnimation()->GetAnimationSequence());
}

void CAnimationEditor::OnEditAnimPlayScale()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// if (m_PlayScaleInput->Empty())
	//	return;

	m_Animation->GetCurrentAnimation()->GetAnimationSequence()->SetPlayScale(m_PlayScaleInput->GetValueFloat());
	m_Animation->GetCurrentAnimation()->SetPlayScale(m_PlayScaleInput->GetValueFloat());

	OnRefreshAnimationClipTable(m_Animation->GetCurrentAnimation()->GetAnimationSequence());
}

void CAnimationEditor::OnEditAnimSequenceKey()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// ���� Animation �� ����� Key ��

	// New Key ��
	// EditCurrentSequenceKeyName
	if (!m_Animation->EditCurrentSequenceKeyName(m_EditAnimSeqDataKeyName->GetTextUTF8(), m_CurAnimComboBox->GetSelectItem()))
	{
		assert(false);
		return;
	}

	// �ٲ��� �̸��� Animation ��  Current Animation ���� �����Ѵ�.
	m_Animation->SetCurrentAnimation(m_EditAnimSeqDataKeyName->GetTextUTF8());

	// Combo Box ���� Refresh
	OnRefreshAnimationComboBox();
}

void CAnimationEditor::OnDeleteAnimationSequenceData()
{
	if (!m_Animation)
		return;

	m_Animation->DeleteCurrentAnimation();

	// Combo Box ���� Refresh
	OnRefreshAnimationComboBox();

	if (!m_Animation->GetCurrentAnimation())
		return;

	OnRefreshAnimationClipTable(m_Animation->GetCurrentAnimation()->GetAnimationSequence());

	OnRefreshFrameSliderInfo(m_Animation->GetCurrentAnimation()->GetAnimationSequence());

	OnRefreshScaleAndTimeInputInfo();

	OnRefreshCheckBoxInfo();
}

void CAnimationEditor::OnClickSetAnimSeqSrcDirButton()
{
	if (m_CommonAnimSeqName->Empty())
		return;

	TCHAR Buf[MAX_PATH] = {};

	BROWSEINFO Bi = {};

	Bi.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	Bi.lpszTitle = TEXT("�ϳ��� Instance �� ���� Seq ���ϵ��� �ִ� ���� ����");
	Bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_VALIDATE;
	Bi.lpfn = BrowseCallbackProc;
	//	Bi.lParam = (LPARAM)InitialPath;

	LPITEMIDLIST ItemIDList = ::SHBrowseForFolder(&Bi);

	if (::SHGetPathFromIDList(ItemIDList, Buf))
	{
		int length = WideCharToMultiByte(CP_ACP, 0, Buf, -1, nullptr, 0, nullptr, 0);
		WideCharToMultiByte(CP_ACP, 0, Buf, -1, m_SelectedSeqSrcsDirPath, length, nullptr, 0);
		// strcat_s(m_SrcDirFullPath, "\\");

		// ExtractAnimationSequenceFilesFullPath(m_SelectedSeqSrcsDirPath, m_vecAnimationSeqFilesFullPath);
		std::vector<std::string> vecSeqFilesFullPath;

		CEditorUtil::GetAllFileFullPathInDir(m_SelectedSeqSrcsDirPath, vecSeqFilesFullPath, ".sqc");
		CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, m_vecAnimationSeqFilesFullPath, m_CommonAnimSeqName->GetTextUTF8());

		// �ϳ��� ã�Ƴ��� ���ߴٸ�.
		if (m_vecAnimationSeqFilesFullPath.size() == 0)
			return;

		m_AnimSeqcSrcFolderPath->SetText(m_SelectedSeqSrcsDirPath);
	}
}

// m_ConvertAnimInstanceBtn �� Callback
void CAnimationEditor::OnConvertSequencesIntoAnimationInstance()
{
	if (m_AnimSeqcSrcFolderPath->Empty() || m_SavedAnimFileName->Empty())
		return;
	
	// �α�â Ŭ����
	m_AnimInstanceConvertLog->ClearWidget();

	CIMGUIText* StartText = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("Text");
	StartText->SetText("Convert Start...");


	// ���� ��� �༮��� Mesh Load �ϰ� 
	size_t Size = m_vecAnimationSeqFilesFullPath.size();

	CIMGUIText* Text = nullptr;

	for (size_t i = 0; i < Size; ++i)
	{
		// �����忡 ���� ��û => Engine ������ ��Ƽ �������� �䱸���� �ʾұ� ������, Engine Ȥ�� GPU �޸𸮸� �ǵ� ���, Engine ���� �߻�
		// m_AnimInstanceConvertThread->AddWork(m_vecAnimationSeqFilesFullPath[i]);
		AddSequenceToDummyAnimationInstance(m_vecAnimationSeqFilesFullPath[i].c_str());

		// File �̸� ��Ͽ� �߰�
		Text = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("Text");

		Text->SetText(CEditorUtil::FilterFileName(m_vecAnimationSeqFilesFullPath[i]).c_str());
	}

	m_AnimInstanceProgressBar->SetPercent(100.f);

	Text = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("OK");
	Text->SetText("Complete!");
	MessageBox(nullptr, TEXT("Instance Create �Ϸ�"), TEXT("�Ϸ�"), MB_OK);

	// Dummy Animation Intance �� �����Ѵ�.
	if (!m_DummyAnimation)
		return;

	const PathInfo* Path = CPathManager::GetInst()->FindPath(ANIMATION_PATH);

	char SavedFileNameMultibyte[MAX_PATH] = {};

	if (Path)
		strcpy_s(SavedFileNameMultibyte, Path->PathMultibyte);

	strcat_s(SavedFileNameMultibyte, m_SavedAnimFileName->GetTextUTF8());
	strcat_s(SavedFileNameMultibyte, ".anim");

	m_DummyAnimation->SetSavedFileName(m_SavedAnimFileName->GetTextUTF8());
	m_DummyAnimation->SaveAnimationFullPath(SavedFileNameMultibyte);

	SAFE_DELETE(m_DummyAnimation);
}

void CAnimationEditor::OnAnimInstanceConvertLoading(const LoadingMessage& msg)
{
	m_AnimInstanceProgressBar->SetPercent(msg.Percent);

	CIMGUIText* Text = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("Text");

	std::string LoadingMsg = "Add Success : " + msg.Message;

	Text->SetText(LoadingMsg.c_str());

	if (msg.Complete)
	{
		Text = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("OK");
		Text->SetText("Complete!");
		MessageBox(nullptr, TEXT("Instance Create �Ϸ�"), TEXT("�Ϸ�"), MB_OK);

		// Dummy Animation Intance �� �����Ѵ�.
		if (!m_DummyAnimation)
			return;

		m_DummyAnimation->SaveAnimationFullPath(m_SelectedSeqSrcsDirPath);

		SAFE_DELETE(m_DummyAnimation);
	}
}

void CAnimationEditor::AddSequenceToDummyAnimationInstance(const char* FileFullPath)
{
	if (!m_DummyAnimation)
	{
		m_DummyAnimation = new CAnimationSequenceInstance;

		if (!m_DummyAnimation->Init())
		{
			SAFE_DELETE(m_DummyAnimation);
			return ;
		}
	}

	char FileFullPathCopy[MAX_PATH] = {};

	strcpy_s(FileFullPathCopy, FileFullPath);

	char SqcFileName[MAX_PATH] = {};
	char SqcExt[_MAX_EXT] = {};

	_splitpath_s(FileFullPathCopy, nullptr, 0, nullptr, 0, SqcFileName, MAX_PATH, SqcExt, _MAX_EXT);
	
	_strupr_s(SqcExt);
	
	// Ȯ���� .sqc �� �ƴ϶�� return;
	if (strcmp(SqcExt, ".SQC") != 0)
		return;
	

	// FileFullPath ���� File �̸����� Key ���� �������� ���̴�.
	if (m_DummyAnimation->FindAnimation(SqcFileName))
		return;
	
	// Animation Seq Load
	std::string FSequenceName;
	
	// Load �� ���� ������ �̸��� �����ͼ� �����Ѵ�.
	bool ResultLoadSeq = CSceneManager::GetInst()->GetScene()->GetResource()->LoadAnimationSequenceFullPathMultibyte(true,
		SqcFileName, FileFullPath);
	
	if (!ResultLoadSeq)
		return;
	
	CAnimationSequence* LoadedSequence = CSceneManager::GetInst()->GetScene()->GetResource()->FindAnimationSequence(SqcFileName);
	
	LoadedSequence->SetAnimationFullPathMultibyte(FileFullPath);
	LoadedSequence->SetAnimationFileNameMultibyte(SqcFileName);
	
	// ex) File �̸��� Single_Idle.sqc ���
	// Resource Manager, Animation Instance �� ���
	// �����ϰ� Single_Idle �̶�� Key ������ �ش� ������ ������ ���̴�.
	m_DummyAnimation->AddAnimation(SqcFileName, SqcFileName);
}

/*
void CAnimationEditor::OnDeleteAnimFrame()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	int DeleteFrameIdx = m_DeleteFrameInput->GetValueInt();

	// if (m_PlayScaleInput->Empty())
	//	return;
	BoneKeyFrame* DeleteBoneFrame = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->DeleteAnimationFrame(DeleteFrameIdx);

	m_StackDeleteFrame.push(std::make_pair(DeleteFrameIdx, DeleteBoneFrame));

	// Frame Slider
	OnRefreshFrameSliderInfo(m_Animation->GetCurrentAnimation()->GetAnimationSequence());

	// Anim Table
	OnRefreshAnimationClipTable(m_Animation->GetCurrentAnimation()->GetAnimationSequence());
}
*/

void CAnimationEditor::OnSetPlayEngineDeltaTime(const char* Lable, bool Enable)
{
	// bool IsEnginePlay = CEngine::GetInst()->IsPlay();

	// if (IsEnginePlay)
	if (!Enable)
		CEngine::GetInst()->Pause();
	else
		CEngine::GetInst()->Play();
}

void CAnimationEditor::OnAddAnimationSequence()
{
	// Add �� Animation �� �̸��� �Է��ؾ� �Ѵ�
	if (m_NewAnimSeqName->Empty() || m_NewAnimSeqDataKeyName->Empty())
		return;

	TCHAR LoadFilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.lpstrFile = LoadFilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("�������\0*.*\0*.Scene File\0*.scn");
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MESH_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char	Ext[_MAX_EXT] = {};
		char FilePathMultibyte[MAX_PATH] = {};
		char SqcFileName[MAX_PATH] = {};

		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		_splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, SqcFileName, MAX_PATH, Ext, _MAX_EXT);
		
		_strupr_s(Ext);
		
		// Ȯ���� .sqc �� �ƴ϶�� return;
		if (strcmp(Ext, ".SQC") != 0)
			return;
		
		if (!m_Animation)
		{
			// f (!LoadElementsForSqcLoading(SqcFileName))
			// 	return;
		
			std::pair<bool, std::string> LoadResult = CResourceManager::GetInst()->LoadMeshTextureBoneInfo(SqcFileName);
		
			if (!LoadResult.first)
				return;
		
			m_3DTestObjectMeshName = LoadResult.second;
		
			// Create Object
			OnCreateSample3DObject();
		
			// Set Material, Mesh Info
			SetMeshMaterialReadyForAnimation();
		}
		
		// �̸� �ߺ� X --> Key �̸� �ߺ��Ǵ� Sequence �� �߰� X 
		// ex) AnimationInstance --> ("ZedIdle", "Idle"); --> "ZedIdle" �̶�� Key �� ���� �� �ٸ� Sqc ������ �ε��ϸ� �ȵȴ�.
		if (m_Animation->FindAnimation(m_NewAnimSeqDataKeyName->GetTextUTF8()))
			return;
		
		// Animation Seq Load
		std::string FSequenceName;
		
		// Load �� ���� ������ �̸��� �����ͼ� �����Ѵ�.
		bool ResultLoadSeq = CSceneManager::GetInst()->GetScene()->GetResource()->LoadAnimationSequenceFullPathMultibyte(true,
			m_NewAnimSeqName->GetTextUTF8(), FilePathMultibyte);
		
		if (!ResultLoadSeq)
			return;
		
		CAnimationSequence* LoadedSequence = CSceneManager::GetInst()->GetScene()->GetResource()->FindAnimationSequence(m_NewAnimSeqName->GetTextUTF8());
		
		LoadedSequence->SetAnimationFullPathMultibyte(FilePathMultibyte);
		LoadedSequence->SetAnimationFileNameMultibyte(SqcFileName);
		
		// ���� 
		m_Animation->AddAnimation(m_NewAnimSeqName->GetTextUTF8(), m_NewAnimSeqDataKeyName->GetTextUTF8());
		
		// Combo Box ���� ����
		OnRefreshAnimationClipTable(LoadedSequence);

		OnRefreshFrameSliderInfo(LoadedSequence);

		OnRefreshAnimationComboBox();
		
		OnRefreshScaleAndTimeInputInfo();

		OnRefreshCheckBoxInfo();
		
		// Frame Slider �� �ִ� �ּ� �� �����ϱ�
		m_FrameSlider->SetMin(LoadedSequence->GetStartFrame());
		m_FrameSlider->SetMax(LoadedSequence->GetEndFrame());
	}
}

void CAnimationEditor::OnClickAnimationSequence(int Index, const char* Name)
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	m_CurAnimComboBox->SetSelectIndex(Index);

	const std::string& SelectSequenceName = m_CurAnimComboBox->GetSelectItem();

	CAnimationSequenceData* SequenceData = m_Animation->FindAnimation(SelectSequenceName);

	if (!SequenceData)
	{
		return;
	}

	// Ŭ���� Animation ���� Current Animation ����
	m_Animation->SetCurrentAnimation(Name);

	// Table ���� ����
	OnRefreshAnimationClipTable(SequenceData->GetAnimationSequence());

	OnRefreshFrameSliderInfo(SequenceData->GetAnimationSequence());

	OnRefreshAnimationComboBox();

	OnRefreshScaleAndTimeInputInfo();

	OnRefreshCheckBoxInfo();
}

void CAnimationEditor::OnRefreshAnimationClipTable(CAnimationSequence* Sequence)
{
	if (!Sequence)
	{
		return;
	}
	m_AnimInfoTable->ClearContents();

	m_AnimInfoTable->AddData(AnimationClipInfoKeys::FrameRange, Sequence->GetStartFrame());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::FrameRange, Sequence->GetEndFrame());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::FrameLength, Sequence->GetFrameLength());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::FrameMode, Sequence->GetFrameMode());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::PlayTime, Sequence->GetPlayTime());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::FrameTime, Sequence->GetFrameTime());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::PlayScale, Sequence->GetPlayScale());
}

void CAnimationEditor::OnRefreshFrameSliderInfo(CAnimationSequence* Sequence)
{
	if (!Sequence)
		return;

	m_FrameSlider->SetMin(Sequence->GetStartFrame());
	m_FrameSlider->SetMax(Sequence->GetEndFrame());
}

void CAnimationEditor::OnRefreshAnimationComboBox()
{
	if (!m_Animation)
		return;

	// �� ó�� Sequence�� �߰��ϴ� ��Ȳ�̶��
	int SeletedIdx = m_CurAnimComboBox->GetSelectIndex();

	m_CurAnimComboBox->Clear();

	std::vector<std::string> vecCurKeyNames;

	m_Animation->GatherSequenceNames(vecCurKeyNames);

	for (const auto& name : vecCurKeyNames)
		m_CurAnimComboBox->AddItem(name);

	if (m_CurAnimComboBox->Empty())
		return;

	if (SeletedIdx < m_CurAnimComboBox->GetItemCount())
	{
		// ó�� Animation �� �߰��ϴ� ��Ȳ�̶��
		if (SeletedIdx == -1)
			m_CurAnimComboBox->SetSelectIndex(0);
		else
			m_CurAnimComboBox->SetSelectIndex(SeletedIdx);
	}
	else
		m_CurAnimComboBox->SetSelectIndex(m_CurAnimComboBox->GetItemCount() - 1);


}

void CAnimationEditor::OnRefreshScaleAndTimeInputInfo()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	float PlayScale = m_Animation->GetCurrentAnimation()->GetAnimationPlayScale();
	m_PlayScaleInput->SetFloat(PlayScale);

	float PlayTime = m_Animation->GetCurrentAnimation()->GetAnimationPlayTime();
	m_PlayTimeInput->SetFloat(PlayTime);
}

void CAnimationEditor::OnRefreshCheckBoxInfo()
{
	if (!m_3DTestObject)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	m_DeltaTimeCheckBtn->SetCheck(0, CEngine::GetInst()->IsPlay());
	m_AnimationCheckBtn->SetCheck(0, m_Animation->IsPlay());
	m_RotationCheckBtn->SetCheck(0,m_3DTestObject->IsCameraRot());
	m_ZoomEnableBtn->SetCheck(0, m_3DTestObject->IsCameraRot());
	m_LoopEnableBtn->SetCheck(0, m_Animation->GetCurrentAnimation()->IsLoop());
}
