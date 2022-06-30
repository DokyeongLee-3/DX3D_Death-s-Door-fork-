// IMGUI
#include "IMGUITextInput.h"
#include "IMGUIInputFloat.h"
#include "IMGUIInputInt.h"
#include "IMGUIInputFloat3.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "IMGUIChild.h"
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
#include "IMGUIProgressBar.h"
#include "IMGUITree.h"
// Window
#include "AnimationEditor.h"
#include "../EditorManager.h"
#include "../Window/ResourceDisplayWindow.h"
#include "AnimationDisplayWindow.h"
// Object
#include "../Object/3DParticleObject.h"
// Engine
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
#include "AnimationEditor.h"
#include "../EditorManager.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Animation/AnimationSequenceInstance.h"
#include "../EditorUtil.h"
#include "Device.h"
#include "../Object/Anim3DObject.h"
#include "Render/RenderManager.h"
#include "Resource/ResourceManager.h"
#include "../AnimationInstanceConvertThread.h"

CAnimationDisplayWindow::CAnimationDisplayWindow()
{
}

CAnimationDisplayWindow::~CAnimationDisplayWindow()
{
	SAFE_DELETE(m_DummyAnimation);
}

bool CAnimationDisplayWindow::Init()
{
    m_AnimationRenderTarget = AddWidget<CIMGUIImage>("Render Target", 400.f, 400.f);
    m_AnimationRenderTarget->SetRenderTargetImage(true);
    m_AnimationRenderTarget->SetTexture(CRenderManager::GetInst()->GetAnimationRenderTarget());
    m_AnimationRenderTarget->SetBorderColor(10, 10, 255);
    m_AnimationRenderTarget->SetTableTitle("Render Target");

    // Anim Instance Making
	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 100.f, 20.f);
	CIMGUIText* Text = AddWidget<CIMGUIText>("Create Instance", 100.f, 30.f);
	Text->SetText("Create Instance");

	// Common Name
	m_SavedAnimFileName = AddWidget<CIMGUITextInput>("Saved File Name", 150.f, 20.f);
	m_SavedAnimFileName->SetHideName(true);
	m_SavedAnimFileName->SetHintText("Saved File Name");

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	CIMGUIText* HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 90.f, 30.f);
	HelpText->SetText("ZedIdle �̶�� �ϸ�, Bin/Animation ������ ZedIdle.anim �̸����� Animation Instance ����");
	HelpText->SetIsHelpMode(true);

	m_CommonAnimSeqName = AddWidget<CIMGUITextInput>("Common Sqc Name", 150.f, 20.f);
	m_CommonAnimSeqName->SetHideName(true);
	m_CommonAnimSeqName->SetHintText("Common Sqc Name");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(165.f);

	m_SelectAnimInstanceFolderPath = AddWidget<CIMGUIButton>("Select Dir", 90.f, 20.f);
	m_SelectAnimInstanceFolderPath->SetClickCallback<CAnimationDisplayWindow>(this, &CAnimationDisplayWindow::OnClickSetAnimSeqSrcDirButton);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(260.f);

	m_ConvertAnimInstanceBtn = AddWidget<CIMGUIButton>("Make Inst", 90.f, 20.f);
	m_ConvertAnimInstanceBtn->SetClickCallback<CAnimationDisplayWindow>(this, &CAnimationDisplayWindow::OnConvertSequencesIntoAnimationInstance);

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



void CAnimationDisplayWindow::OnClickSetAnimSeqSrcDirButton()
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

		// ��� .sqc Ȯ���� ���� �̸����� ������. FullPath ���� ��Ƶ� ���̴�.
		std::vector<std::string> vecSeqFilesFullPath;

		// ���� �ش� .sqc Ȯ���ڸ� ����, FullPath ��ϵ��� vector ���·� ��Ƶ־� �Ѵ�.
		// CEditorUtil::GetAllFileNamesInDir(m_SelectedSeqSrcsDirPath, vecSeqFilesFullPath, ".sqc");
		CEditorUtil::GetAllFileFullPathInDir(m_SelectedSeqSrcsDirPath, vecSeqFilesFullPath, ".sqc");

		// vecAllAnimationSeqFilesFullPath �� Ư�� ���ڸ� �����ϴ� (�빮��, �ҹ��� ���� ���) .sqc ���ϵ��� ��� ��Ƴ��� ���̴�.
		std::vector<std::string> vecAllAnimationSeqFilesFullPath;
		vecAllAnimationSeqFilesFullPath.reserve(100);

		m_vecAnimationSeqFilesFullPath.clear();
		m_vecAnimationSeqFilesFullPath.reserve(100);

		// 1. Origin �Է� ���� �״���� ���� �̸��� �����ϴ� ���� ���� ������
		// ex) "zed" => ��� "zed~~" ������ ���� �̸��� ����

		// CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, m_vecAnimationSeqFilesFullPath, m_CommonAnimSeqName->GetTextUTF8());
		CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, vecAllAnimationSeqFilesFullPath, m_CommonAnimSeqName->GetTextUTF8());

		// ��� .sqc ���ϵ��� m_vecAnimationSeqFilesFullPath �� ��Ƶα� 
		size_t AllSeqFilesCnt = vecAllAnimationSeqFilesFullPath.size();

		for (size_t i = 0; i < AllSeqFilesCnt; ++i)
		{
			// �ߺ� ����
			if (std::find(m_vecAnimationSeqFilesFullPath.begin(), m_vecAnimationSeqFilesFullPath.end(), vecAllAnimationSeqFilesFullPath[i]) != m_vecAnimationSeqFilesFullPath.end())
				continue;

			m_vecAnimationSeqFilesFullPath.push_back(std::move(vecAllAnimationSeqFilesFullPath[i]));
		}

		// 2. ���� �ҹ���, �빮��
		//    ��� �ҹ���, ��� �빮��
		std::vector<std::string> vecEachUpperSeqFilesFullPath;
		std::vector<std::string> vecEachLowerSeqFilesFullPath;
		std::string AllUpper;
		std::string AllLower;

		CEditorUtil::GetAllKindsOfTransformedStringVersions(m_CommonAnimSeqName->GetTextUTF8(), vecEachLowerSeqFilesFullPath,
			vecEachUpperSeqFilesFullPath, AllUpper, AllLower);

		// ���� �빮�� ��ϵ� ��Ƶα�
		size_t EachUpperTotSize = vecEachUpperSeqFilesFullPath.size();

		for (size_t i = 0; i < EachUpperTotSize; ++i)
		{
			// FilterSpecificNameIncludedFilePath ������ vecAllAnimationSeqFilesFullPath �� ����ش�.
			CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, vecAllAnimationSeqFilesFullPath, vecEachUpperSeqFilesFullPath[i].c_str());

			AllSeqFilesCnt = vecAllAnimationSeqFilesFullPath.size();

			for (size_t i = 0; i < AllSeqFilesCnt; ++i)
			{
				if (std::find(m_vecAnimationSeqFilesFullPath.begin(), m_vecAnimationSeqFilesFullPath.end(), vecAllAnimationSeqFilesFullPath[i]) != m_vecAnimationSeqFilesFullPath.end())
					continue;

				m_vecAnimationSeqFilesFullPath.push_back(std::move(vecAllAnimationSeqFilesFullPath[i]));
			}
		}

		// ���� �ҹ��� ��ϵ� ��Ƶα�
		size_t EachLowerTotSize = vecEachLowerSeqFilesFullPath.size();

		for (size_t i = 0; i < EachLowerTotSize; ++i)
		{
			// FilterSpecificNameIncludedFilePath ������ vecAllAnimationSeqFilesFullPath �� ����ش�.
			CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, vecAllAnimationSeqFilesFullPath, vecEachLowerSeqFilesFullPath[i].c_str());

			AllSeqFilesCnt = vecAllAnimationSeqFilesFullPath.size();

			for (size_t i = 0; i < AllSeqFilesCnt; ++i)
			{
				if (std::find(m_vecAnimationSeqFilesFullPath.begin(), m_vecAnimationSeqFilesFullPath.end(), vecAllAnimationSeqFilesFullPath[i]) != m_vecAnimationSeqFilesFullPath.end())
					continue;

				m_vecAnimationSeqFilesFullPath.push_back(std::move(vecAllAnimationSeqFilesFullPath[i]));
			}
		}


		// All �빮�� ��ϵ� ��Ƶα�
		CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, vecAllAnimationSeqFilesFullPath, AllUpper.c_str());

		AllSeqFilesCnt = vecAllAnimationSeqFilesFullPath.size();

		for (size_t i = 0; i < AllSeqFilesCnt; ++i)
		{
			if (std::find(m_vecAnimationSeqFilesFullPath.begin(), m_vecAnimationSeqFilesFullPath.end(), vecAllAnimationSeqFilesFullPath[i]) != m_vecAnimationSeqFilesFullPath.end())
				continue;

			m_vecAnimationSeqFilesFullPath.push_back(std::move(vecAllAnimationSeqFilesFullPath[i]));
		}

		// All �ҹ��� ��ϵ� ��Ƶα�
		CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, vecAllAnimationSeqFilesFullPath, AllLower.c_str());

		AllSeqFilesCnt = vecAllAnimationSeqFilesFullPath.size();

		for (size_t i = 0; i < AllSeqFilesCnt; ++i)
		{
			if (std::find(m_vecAnimationSeqFilesFullPath.begin(), m_vecAnimationSeqFilesFullPath.end(), vecAllAnimationSeqFilesFullPath[i]) != m_vecAnimationSeqFilesFullPath.end())
				continue;

			m_vecAnimationSeqFilesFullPath.push_back(std::move(vecAllAnimationSeqFilesFullPath[i]));
		}


		// �ϳ��� ã�Ƴ��� ���ߴٸ�.
		if (m_vecAnimationSeqFilesFullPath.size() == 0)
		{
			MessageBox(nullptr, TEXT("No .sqc Files Found"), TEXT(".sqc File ã�� ����"), MB_OK);
			return;
		}

		m_AnimSeqcSrcFolderPath->SetText(m_SelectedSeqSrcsDirPath);
	}
}

// m_ConvertAnimInstanceBtn �� Callback
void CAnimationDisplayWindow::OnConvertSequencesIntoAnimationInstance()
{
	//m_SavedAnimFileName : Instance �� ���� ����, .anim ���Ϸ� ������ ���� ���� �̸�
	if (m_AnimSeqcSrcFolderPath->Empty() || m_SavedAnimFileName->Empty())
		return;

	// ���� m_SavedAnimFileName ��, Bin/Animation �� �̹� �����ϴ��� Ȯ�� + �ߺ� ����
	// return ���� true ���, ���� X (�ߺ��� �̸��� .anim ������ �̹� �����Ѵٴ� �ǹ��̱� �����̴�)
	bool DuplicateResult = CheckSavedFileNameDuplicated();

	if (DuplicateResult)
	{
		MessageBox(nullptr, TEXT("�ش� Folder �� ���� �̸��� .anim File ����"), TEXT(".anim File Duplicate"), MB_OK);
		return;
	}

	// Animation Instance �� ���� .sqc ���� ��ϵ��� �ִ��� �˻��Ѵ�.
	if (m_vecAnimationSeqFilesFullPath.size() <= 0)
	{
		MessageBox(nullptr, TEXT("�ش� Folder �� �ش� Common �̸��� ������ .sqc ���� ���� X"), TEXT("Common Name Error"), MB_OK);
		return;
	}
	
	// �ٽ� �ѹ�, ���� ��Ƶ� m_vecAnimationSeqFilesFullPath ����, Common Name �� ���ԵǾ� �ִ��� �ѹ� �� �˻��Ѵ�.
	// ù��° File �θ� �˻��� ���̴�.
	bool CheckCommonNameResult = CheckGatheredSeqFilesIncludeCommonName();

	if (!CheckCommonNameResult)
	{
		MessageBox(nullptr, TEXT("Folder ��ο�, Common �̸��� Ȯ���ϼ���. (�߰��� Common �̸��� �����ߴ��� Ȯ��)"), TEXT("Common Name Error"), MB_OK);
		return;
	}

	// �α�â Ŭ����
	m_AnimInstanceConvertLog->ClearWidget();

	CIMGUIText* StartText = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("Text");
	StartText->SetText("Convert Start...");


	// ���� ��� �༮��� Mesh Load �ϰ� 
	size_t Size = m_vecAnimationSeqFilesFullPath.size();

	CIMGUIText* Text = nullptr;

	for (size_t i = 0; i < Size; ++i)
	{
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

void CAnimationDisplayWindow::OnAnimInstanceConvertLoading(const LoadingMessage& msg)
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

void CAnimationDisplayWindow::AddSequenceToDummyAnimationInstance(const char* FileFullPath)
{
	if (!m_DummyAnimation)
	{
		m_DummyAnimation = new CAnimationSequenceInstance;

		if (!m_DummyAnimation->Init())
		{
			SAFE_DELETE(m_DummyAnimation);
			return;
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
	// �ߺ� ���� 
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

// Return ���� false ���, ��� �����Ͽ�, Make Inst
// true ��� , �ߺ� ������ �߻��ϴ� ���̹Ƿ�, Make Inst X
bool CAnimationDisplayWindow::CheckSavedFileNameDuplicated()
{
	// SaveFileName Input �� Text �� ����ִٸ� skip
	if (m_SavedAnimFileName->Empty())
		return true;

	const PathInfo* Info = CPathManager::GetInst()->FindPath(ANIMATION_PATH);

	if (!Info)
	{
		assert(false);
	}

	std::string SavedFileName = m_SavedAnimFileName->GetTextUTF8();

	if (SavedFileName.find(".anim") == std::string::npos)
		SavedFileName.append(".anim");

	return CEditorUtil::IsFileExistInDir(ANIMATION_PATH, SavedFileName);
}

bool CAnimationDisplayWindow::CheckGatheredSeqFilesIncludeCommonName()
{
	if (m_vecAnimationSeqFilesFullPath.size() > 0)
	{
		bool CommonNameIncluded = false;

		// && m_vecAnimationSeqFilesFullPath[0].find(m_CommonAnimSeqName->GetTextUTF8()) == std::string::npos
		std::vector<std::string> vecEachUpperSeqFilesFullPath;
		std::vector<std::string> vecEachLowerSeqFilesFullPath;
		std::string AllUpper;
		std::string AllLower;

		CEditorUtil::GetAllKindsOfTransformedStringVersions(m_CommonAnimSeqName->GetTextUTF8(), vecEachLowerSeqFilesFullPath,
			vecEachUpperSeqFilesFullPath, AllUpper, AllLower);

		size_t EachUpperTotalSize = vecEachUpperSeqFilesFullPath.size();

		for (size_t i = 0; i < EachUpperTotalSize; ++i)
		{
			// ���࿡ �ϳ��� �����Ѵٸ�, ���� Folder ��ο� Common Name �� �� ¦������ ���̴�.
			if (m_vecAnimationSeqFilesFullPath[0].find(vecEachUpperSeqFilesFullPath[i]) != std::string::npos)
			{
				CommonNameIncluded = true;
				break;
			}
		}

		if (!CommonNameIncluded)
		{
			size_t EachLowerTotalSize = vecEachLowerSeqFilesFullPath.size();

			for (size_t i = 0; i < EachLowerTotalSize; ++i)
			{
				// ���࿡ �ϳ��� �����Ѵٸ�, ���� Folder ��ο� Common Name �� �� ¦������ ���̴�.
				if (m_vecAnimationSeqFilesFullPath[0].find(vecEachLowerSeqFilesFullPath[i]) != std::string::npos)
				{
					CommonNameIncluded = true;
					break;
				}
			}
		}

		if (!CommonNameIncluded)
		{
			// ���࿡ �ϳ��� �����Ѵٸ�, ���� Folder ��ο� Common Name �� �� ¦������ ���̴�.
			if (m_vecAnimationSeqFilesFullPath[0].find(AllUpper) != std::string::npos)
				CommonNameIncluded = true;
		}

		if (CommonNameIncluded)
		{
			// ���࿡ �ϳ��� �����Ѵٸ�, ���� Folder ��ο� Common Name �� �� ¦������ ���̴�.
			if (m_vecAnimationSeqFilesFullPath[0].find(AllLower) != std::string::npos)
				CommonNameIncluded = true;
		}

		// ���� Folder �� Ư�� �̸��� �����ϴ� File ���� ��Ƴ��µ�
		// �ش� File �� ��, ù��° ������, Common Name �� Include ���� �ʴ´ٴ� �ǹ̴�
		// �߰��� Common NAme �� �ٲٴ� ���� ��ȭ�� �־��ٴ� ���̴�.
		if (!CommonNameIncluded)
			return false;
	}

	return true;
}
