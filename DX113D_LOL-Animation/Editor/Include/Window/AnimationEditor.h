#pragma once
#include "IMGUIWindow.h"

namespace AnimationClipInfoKeys
{
	const std::string FrameRange = "Frame Range";
	const std::string FrameLength = "Frame Length";
	const std::string FrameMode = "Frame Mode"; // 24, 30, 60
	const std::string PlayTime = "Play Time";
	const std::string FrameTime = "Frame Time";
	const std::string PlayScale = "Play Scale";
};

class CAnimationEditor :
    public CIMGUIWindow
{
	friend class CEditorManager;
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
	// class CIMGUIButton* m_FrameDeleteBtn;
	// class CIMGUITextInput* m_DeleteFrameInput;
	// Animation Seq �߰�
	class CIMGUIButton* m_AnimSequenceAddBtn;
	class CIMGUIButton* m_CreateSample3DBtn;
	// Animation Seq �����
	class CIMGUIButton* m_DeleteAnimSequenceBtn;
	// Animation Play ����
	class CIMGUIButton* m_DeltaTimePlayBtn;
	class CIMGUIButton* m_AnimationPlayBtn;
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
private :
	class CAnim3DObject* m_3DTestObject;
	class CAnimationSequenceInstance* m_Animation;
	class std::stack<std::pair<int, class BoneKeyFrame*>> m_StackDeleteFrame;
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private :
	// Refresh 
	void OnRefreshAnimationClipTable(class CAnimationSequence* SequenceData);
	void OnRefreshFrameSliderInfo(class CAnimationSequence* SequenceData);
	void OnRefreshAnimationComboBox();
	void OnClearExistingAnimationSeqInfos();
	void OnAddAnimationSequence();
	void OnClickAnimationSequence(int, const char*);
	void OnAnimationSliderIntCallback(int);
	void OnAnimationFrameInputCallback();
	// Play
	void OnCreateSample3DObject();
	void OnSetPlayEngineDeltaTime();
	void OnPlayAnimation();
	// Save Load
	void OnSaveAnimationInstance();
	void OnLoadAnimationInstance();
	void OnApplyAnimationSlider(CAnimationSequence* Sequence);
	// Edit
	void OnEditAnimPlayTime();
	void OnEditAnimPlayScale();
	void OnEditAnimSequenceKey();
	void OnDeleteAnimationSequenceData(); // Ư�� Animation �����
	// Animation Sequence Delete -> ���� �ڵ�� BoneKeyFrame �� �ϳ� �����ϴ� �����̴�.
	// �׷��� �̷��� �ϸ� Animation �� �̻��ϰ� �����ϰ� �ȴ�. �Ƹ��� 
	// void OnDeleteAnimFrame();
};

