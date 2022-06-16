#pragma once
#include "IMGUIWindow.h"
#include "../EditorInfo.h"
#include <filesystem>


class CAnimationEditor :
	public CIMGUIWindow
{
	friend class CEditorManager;
	// friend class CAnimationInstanceConvertThread;
public:
	CAnimationEditor();
	~CAnimationEditor();
private:
	// Anim Combo Box
	class CIMGUIComboBox* m_CurAnimComboBox;
	// Anim Clip Table
	class CIMGUITable* m_AnimInfoTable;
	// Frame ����
	class CIMGUITextInput* m_FrameInput;
	class CIMGUISliderInt* m_FrameSlider;
	class CIMGUIButton* m_SetOriginalPlayTimeBtn;

	class CIMGUIButton* m_AnimSequenceAddBtn;
	class CIMGUIButton* m_CreateSample3DBtn;
	// Animation Seq �����
	class CIMGUIButton* m_DeleteAnimSequenceBtn;
	// Animation Play ����
	class CIMGUICheckBox* m_DeltaTimeCheckBtn;
	class CIMGUICheckBox* m_AnimationCheckBtn;
	class CIMGUICheckBox* m_RotationCheckBtn;
	class CIMGUICheckBox* m_ZoomEnableBtn;
	class CIMGUICheckBox* m_LoopEnableBtn;
	// New Anim Names
	class CIMGUITextInput* m_NewAnimSeqName;
	class CIMGUITextInput* m_NewAnimSeqDataKeyName;
	class CIMGUITextInput* m_EditAnimSeqDataKeyName;
	class CIMGUIButton* m_EditAnimKeyBtn;
	// Save Load
	class CIMGUIButton* m_SaveAnimationInstanceBtn;
	class CIMGUIButton* m_LoadAnimationInstanceBtn;
	// Play Scale, Time
	class CIMGUITextInput* m_PlayScaleInput;
	class CIMGUIButton* m_PlayScaleEditBtn;
	class CIMGUITextInput* m_PlayTimeInput;
	class CIMGUIButton* m_PlayTimeEditBtn;
	// Render Target
	bool m_RenderTargetSet;
	class CIMGUIImage* m_AnimationRenderTarget;
private:
	class CAnim3DObject* m_3DTestObject;
	std::string m_3DTestObjectMeshName;
	class CAnimationSequenceInstance* m_Animation;
	// --------------------------------------------------------------------
private :
	class CAnimationInstanceConvertThread* m_AnimInstanceConvertThread;
	// ������ Folder
	class CIMGUITextInput* m_AnimSeqcSrcFolderPath;
	// ���� File Name
	class CIMGUITextInput* m_CommonAnimSeqName;
	// Convert Btn
	class CIMGUIButton* m_ConvertAnimInstanceBtn;
	// Folder ����
	class CIMGUIButton* m_SelectAnimInstanceFolderPath;
	// Log
	class CIMGUIChild* m_AnimInstanceConvertLog;
	// ProgressBar
	class CIMGUIProgressBar* m_AnimInstanceProgressBar;
	// ������ Animation File Name
	class CIMGUITextInput* m_SavedAnimFileName;
private :
	// FullPath ��� ��Ƶ� Vector 
	std::vector<std::string> m_vecAnimationSeqFilesFullPath;
	
	// FileName ��� ��Ƶ� Vector
	// std::vector<std::string> m_vecAnimationSeqFileNames;

	char m_SelectedSeqSrcsDirPath[MAX_PATH];
	// Animation Instance �� File���·� �����ϱ� ���� �ӽ������� ����ϴ� Dummy Animation Instance
	class CAnimationSequenceInstance* m_DummyAnimation;
public:
	const std::string& Get3DTestObjectMeshName() const
	{
		return m_3DTestObjectMeshName;
	}
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private:
	// Refresh 
	void OnRefreshAnimationClipTable(class CAnimationSequence* SequenceData);
	void OnRefreshFrameSliderInfo(class CAnimationSequence* SequenceData);
	void OnRefreshAnimationComboBox();
	void OnRefreshScaleAndTimeInputInfo();
	void OnRefreshCheckBoxInfo();
	// Play
	void OnCreateSample3DObject();
	void OnSetPlayEngineDeltaTime(const char*, bool);
	void OnPlayAnimation(const char*, bool);
	void OnRotateAnimationCamera(const char*, bool);
	void OnZoomAnimationCamera(const char*, bool);
	// Loop
	void OnLoopAnimation(const char*, bool);
	// Save Load
	void OnAddAnimationSequence();
	void OnSaveAnimationInstance();
	void OnLoadAnimationInstance();
	void OnApplyAnimationSlider(CAnimationSequence* Sequence);
	bool LoadElementsForSqcLoading(const char* SqcFileName);
	void SetMeshMaterialReadyForAnimation();
	// Edit
	void OnSetOriginalAnimPlayTime(); // ���� Sequence�� PlayTime �� ����Ͽ� PlayTime �� �ٽ� �����Ѵ�.
	void OnEditAnimPlayTime();
	void OnEditAnimPlayScale();
	void OnEditAnimSequenceKey();
	void OnDeleteAnimationSequenceData(); // Ư�� Animation �����
	// Animation Sequence Delete -> ���� �ڵ�� BoneKeyFrame �� �ϳ� �����ϴ� �����̴�.
	// �׷��� �̷��� �ϸ� Animation �� �̻��ϰ� �����ϰ� �ȴ�. �Ƹ��� 
	// void OnDeleteAnimFrame();

// Helper Functions
	void OnDeleteExisting3DObject();
	void OnClearExistingAnimationSeqInfos();
	void OnClickAnimationSequence(int, const char*);
	void OnSetAnimationComboBoxCallback(const std::string& AnimSequenceName);
	void OnAnimationSliderIntCallback(int);
	void OnAnimationFrameInputCallback();
	// Convert Animation Instance  Functions
private :
	void OnClickSetAnimSeqSrcDirButton();
	void OnConvertSequencesIntoAnimationInstance();
	void OnAnimInstanceConvertLoading(const LoadingMessage& msg);
	// Helper Functions
private :
	void AddSequenceToDummyAnimationInstance(const char* FileFullPath);
};

