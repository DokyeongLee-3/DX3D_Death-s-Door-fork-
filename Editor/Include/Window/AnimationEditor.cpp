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

CAnimationEditor::CAnimationEditor()
{
}

CAnimationEditor::~CAnimationEditor()
{
	// �� Animation �� ���� 3DTestObject ���� ���� Animation �̴�. 
	// SAFE_DELETE(m_Animation);
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

	// Btns
	m_AnimSeqLoadBtn = AddWidget<CIMGUIButton>("AddSequences", 100.f, 30.f);
	m_AnimSeqLoadBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnAddAnimationSequence);

	// ex) ZedIdle.sqc --> 
	CIMGUIText* HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 100.f, 30.f);
	HelpText->SetText("ex)  'ZedIdle' -- > ('ZedIdle', 'ZedIdle.sqc') �� SceneResource, ResourceManager�� m_mapSequence �� ����");
	HelpText->SetIsHelpMode(true);

	m_NewAnimSeqName = AddWidget<CIMGUITextInput>("Sequence Name", 200.f, 30.f);;
	HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 100.f, 30.f);
	HelpText->SetText("ex) 'Idle' --> m_Animation->AddAnimation('ZedIdle', 'Idle') ���·�, Key������ AnimationInstance �� ���� �߰�");
	HelpText->SetIsHelpMode(true);

	m_NewAnimSeqDataKeyName = AddWidget<CIMGUITextInput>("Sequence  Key", 200.f, 30.f);;


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
}

void CAnimationEditor::OnCreateSample3DObject()
{
	// ������ �� �����ְ�
	if (m_3DTestObject)
	{
		m_3DTestObject->Destroy();
		m_CurAnimComboBox->Clear();
	}

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
	m_Animation->SetCurrentAnimationFrameIdx(InputFrame);

	// ���� �ϴ� Animation �� �����.
	m_Animation->Stop();
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
