#include "AnimationEditor.h"
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUIDummy.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUITable.h"
#include "IMGUITextInput.h"
#include "../EditorManager.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "IMGUISliderFloat.h"
#include "IMGUISliderInt.h"
#include "Animation/AnimationSequenceInstance.h"
#include "../EditorUtil.h"
#include "Engine.h"
#include "Device.h"
#include "../Object/Anim3DObject.h"
#include "Render/RenderManager.h"

CAnimationEditor::CAnimationEditor() :
	m_RenderTargetSet(false)
{
}

CAnimationEditor::~CAnimationEditor()
{
	// �� Animation �� ���� 3DTestObject ���� ���� Animation �̴�. 
	// SAFE_DELETE(m_Animation);


	// Delete �Ͽ� ���� BoneKeyFrame ������ �����ش�.
	while (!m_StackDeleteFrame.empty())
	{
		BoneKeyFrame* BoneFrame = m_StackDeleteFrame.top().second;
		m_StackDeleteFrame.pop();

		for (int i = 0; i < BoneFrame->vecKeyFrame.size(); ++i)
		{
			SAFE_DELETE(BoneFrame->vecKeyFrame[i]);
		}
		/*
		for (int i = 0; i < BoneFrame->vecKeyFrame.size(); ++i)
		{
			--m_vecKeyFrame[i]->iRefCount;

			if (m_vecKeyFrame[i]->iRefCount == 0)
				delete	m_vecKeyFrame[i];
		}
		*/
	}
}

bool CAnimationEditor::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	// Anim Name
	CIMGUILabel* Label = AddWidget<CIMGUILabel>("Anim Name", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(150.f);

	m_CurAnimComboBox = AddWidget<CIMGUIComboBox>("Anim List Combo Box", 300.f, 30.f);
	m_CurAnimComboBox->SetHideName(true);
	m_CurAnimComboBox->SetSelectCallback<CAnimationEditor>(this, &CAnimationEditor::OnClickAnimationSequence);

	// Animation Clip + Table
	Label = AddWidget<CIMGUILabel>("Anim Clip Info", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	m_AnimInfoTable = AddWidget<CIMGUITable>("AnimTable", 600.f, 200.f);

	// Frame Slide 
	Label = AddWidget<CIMGUILabel>("Frame Slide", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(150.f);

	m_FrameSlider = AddWidget<CIMGUISliderInt>("Frame Slider", 100.f, 30.f);
	m_FrameSlider->SetCallBack<CAnimationEditor>(this, &CAnimationEditor::OnAnimationSliderIntCallback);

	// Frame Input
	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 100.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(150.f);

	m_FrameInput = AddWidget<CIMGUITextInput>("Frame Input", 100.f, 30.f);
	m_FrameInput->SetTextType(ImGuiText_Type::Int);
	m_FrameInput->SetCallback<CAnimationEditor>(this, &CAnimationEditor::OnAnimationFrameInputCallback);

	// Frame Delete
	// m_DeleteFrameInput = AddWidget<CIMGUITextInput>("Delete Target Frame", 100.f, 30.f);
	// m_DeleteFrameInput->SetTextType(ImGuiText_Type::Int);

	// Delete Anim Sequence
	m_DeleteAnimSequenceBtn = AddWidget<CIMGUIButton>("Delete Sequence", 100.f, 30.f);
	m_DeleteAnimSequenceBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnDeleteAnimationSequenceData);

	// Btns
	m_AnimSequenceAddBtn = AddWidget<CIMGUIButton>("AddSequences", 100.f, 30.f);
	m_AnimSequenceAddBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnAddAnimationSequence);

	// ex) ZedIdle.sqc --> 
	CIMGUIText* HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 100.f, 30.f);
	HelpText->SetText("ex)  'ZedIdle' -- > ('ZedIdle', 'ZedIdle.sqc') �� SceneResource, ResourceManager�� m_mapSequence �� ����");
	HelpText->SetIsHelpMode(true);
	m_NewAnimSeqName = AddWidget<CIMGUITextInput>("Sequence Name", 200.f, 30.f);;


	HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 100.f, 30.f);
	HelpText->SetText("ex) 'Idle' --> m_Animation->AddAnimation('ZedIdle', 'Idle') ���·�, Key������ AnimationInstance �� ���� �߰�");
	HelpText->SetIsHelpMode(true);
	m_NewAnimSeqDataKeyName = AddWidget<CIMGUITextInput>("Sequence  Key", 200.f, 30.f);;

	// Animation Instance Key Name ���� ���
	HelpText = AddWidget<CIMGUIText>("Anim Key Name Edit Help", 100.f, 30.f);
	HelpText->SetText("ex) 'EditIdle' --> m_Animation->AddAnimation('ZedIdle', 'Idle') ���� ���� ������� m_mapAnimationSequence['Idle'] = 'ZedIdle' ��  m_mapAnimationSequence['EditIdle'] = 'ZedIdle' �� Key �� ���� ");
	HelpText->SetIsHelpMode(true);
	m_EditAnimSeqDataKeyName = AddWidget<CIMGUITextInput>("Edit Sequence Key", 200.f, 30.f);;

	m_EditAnimKeyBtn = AddWidget<CIMGUIButton>("Edit Anim Key Btn", 100.f, 30.f);
	m_EditAnimKeyBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnEditAnimSequenceKey);

	// ���� Render Target
	m_AnimationRenderTarget = AddWidget<CIMGUIImage>("Render Target", 400.f, 400.f);

	// Table Key �� ���� ����
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameRange);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameLength);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameMode);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::PlayTime);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameTime);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::PlayScale);


	// Animation Instance ��ü ����
	m_CreateSample3DBtn = AddWidget<CIMGUIButton>("Sample3DBtn", 100.f, 30.f);
	m_CreateSample3DBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnCreateSample3DObject);

	m_DeltaTimePlayBtn = AddWidget<CIMGUIButton>("Engine Play", 100.f, 30.f);
	m_DeltaTimePlayBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnSetPlayEngineDeltaTime);

	m_AnimationPlayBtn = AddWidget<CIMGUIButton>("Object Play", 100.f, 30.f);
	m_AnimationPlayBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnPlayAnimation);

	m_SaveAnimationInstanceBtn = AddWidget<CIMGUIButton>("Save Instance", 100.f, 30.f);
	m_SaveAnimationInstanceBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnSaveAnimationInstance);
	
	m_LoadAnimationInstanceBtn = AddWidget<CIMGUIButton>("Load Instance", 100.f, 30.f);
	m_LoadAnimationInstanceBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnLoadAnimationInstance);

	// Play Scale ����
	m_PlayScaleInput = AddWidget<CIMGUITextInput>("Play Scale Input", 200.f, 30.f);
	m_PlayScaleInput->SetHideName(true);
	m_PlayScaleInput->SetTextType(ImGuiText_Type::Float);

	m_PlayScaleEditBtn = AddWidget<CIMGUIButton>("Edit Scale", 100.f, 30.f);
	m_PlayScaleEditBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnEditAnimPlayScale);
	
	// Play Time ����
	m_PlayTimeInput = AddWidget<CIMGUITextInput>("Play Time Input", 200.f, 30.f);
	m_PlayTimeInput->SetHideName(true);
	m_PlayTimeInput->SetTextType(ImGuiText_Type::Float);

	m_PlayTimeEditBtn = AddWidget<CIMGUIButton>("Edit Time", 100.f, 30.f);
	m_PlayTimeEditBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnEditAnimPlayTime);

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

	m_Animation->ClearAnimationSequence();

	// Combo Box Clear
	m_CurAnimComboBox->Clear();

	// Table Claer
	m_AnimInfoTable->ClearContents();
}

void CAnimationEditor::OnCreateSample3DObject()
{
	// ������ �� �����ְ�
	/*
	if (m_3DTestObject)
	{
		m_3DTestObject->Destroy();
		m_CurAnimComboBox->Clear();
	}
	*/
	
	if (m_3DTestObject)
		return;

	m_3DTestObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CAnim3DObject>("3D Test");

	// 3DTest Object�� Animation ������ �����´�.
	m_Animation = dynamic_cast<CAnimationMeshComponent*>(m_3DTestObject->GetRootComponent())->CreateBasicAnimationInstance();

	m_Animation->Start();

	// 3DTestObject �� Camera Object �� Scene�� Current Camera �� �����Ѵ�.
	if (CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera() != m_3DTestObject->GetCamera())
	{
		CSceneManager::GetInst()->GetScene()->GetCameraManager()->SetCurrentCamera(m_3DTestObject->GetCamera());
	}
}

void CAnimationEditor::OnPlayAnimation()
{
	if (!m_3DTestObject)
		return;

	CAnimationMeshComponent* RootMeshComponent = dynamic_cast<CAnimationMeshComponent*>(m_3DTestObject->GetRootComponent());

	if (!RootMeshComponent)
		return;

	bool IsAnimPlay = RootMeshComponent->GetAnimationInstance()->IsPlay();

	if (IsAnimPlay)
		RootMeshComponent->GetAnimationInstance()->Stop();
	else
		RootMeshComponent->GetAnimationInstance()->Play();
}

void CAnimationEditor::OnSaveAnimationInstance()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	TCHAR FiilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Animation File\0*.anim");
	OpenFile.lpstrFile = FiilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int  ConvertLength = WideCharToMultiByte(CP_ACP, 0, FiilePath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, FiilePath, -1, FilePathMultibyte, ConvertLength, nullptr, nullptr);

		m_Animation->SaveAnimationFullPath(FilePathMultibyte);
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
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		_splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, nullptr, 0, Ext, _MAX_EXT);

		_strupr_s(Ext);

		// Ȯ���� .anim �� �ƴ϶�� return;
		if (strcmp(Ext, ".ANIM") != 0)
			return;

		if (!m_Animation)
			OnCreateSample3DObject();
		else
		{
			// ���� Animation List�� �������� , ��, ���� Animation�� Added �Ǿ��� ��� Sequence ������ �����ش�
			OnClearExistingAnimationSeqInfos();
		}

		m_Animation->LoadAnimationFullPath(FilePathMultibyte);

		// CurrentAnimation �� ���ٸ�,
		if (!m_Animation->GetCurrentAnimation())
			return;

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
		m_Animation->AddAnimationSequenceToSceneResource();

		// ���� Scene�� ������ m_Scene���� �������ش�
		m_Animation->SetScene(CSceneManager::GetInst()->GetScene());

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
	m_Animation->SetEditorStopTargetFrame(InputFrame );

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

void CAnimationEditor::OnEditAnimPlayTime()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// if (m_PlayTimeInput->Empty())
	//	return;

	m_Animation->GetCurrentAnimation()->GetAnimationSequence()->SetPlayTime(m_PlayTimeInput->GetValueFloat());
	m_Animation->GetCurrentAnimation()->SetPlayTime(m_PlayTimeInput->GetValueFloat());

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

void CAnimationEditor::OnSetPlayEngineDeltaTime()
{
	bool IsEnginePlay = CEngine::GetInst()->IsPlay();

	if (IsEnginePlay)
		CEngine::GetInst()->SetPlay(false);
	else
		CEngine::GetInst()->SetPlay(true);
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
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		if (!m_Animation)
			OnCreateSample3DObject();

		// �̸� �ߺ� X --> Key �̸� �ߺ�
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

		// ���� 
		m_Animation->AddAnimation(m_NewAnimSeqName->GetTextUTF8(), m_NewAnimSeqDataKeyName->GetTextUTF8());

		// Combo Box ���� ����
		std::vector<std::string> vecSequenceNames = {};
		m_Animation->GetAnimationSequenceNames(vecSequenceNames);

		m_CurAnimComboBox->Clear();

		for (const auto& name : vecSequenceNames)
			m_CurAnimComboBox->AddItem(name);

		// Load �� Sequence �� ���� Setting �Ǳ�
		m_CurAnimComboBox->SetSelectIndex(m_CurAnimComboBox->GetItemCount() - 1);

		OnRefreshAnimationClipTable(LoadedSequence);

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

	// Table ���� ����
	OnRefreshAnimationClipTable(SequenceData->GetAnimationSequence());

	// Ŭ���� Animation ���� Current Animation ����
	m_Animation->SetCurrentAnimation(Name);

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

	int SeletedIdx = m_CurAnimComboBox->GetSelectIndex();

	m_CurAnimComboBox->Clear();

	std::vector<std::string> vecCurKeyNames;

	m_Animation->GatherSequenceNames(vecCurKeyNames);

	for (const auto& name : vecCurKeyNames)
		m_CurAnimComboBox->AddItem(name);

	if (m_CurAnimComboBox->Empty())
		return;

	if (SeletedIdx < m_CurAnimComboBox->GetItemCount())
		m_CurAnimComboBox->SetSelectIndex(SeletedIdx);
	else
		m_CurAnimComboBox->SetSelectIndex(m_CurAnimComboBox->GetItemCount() - 1);

}
