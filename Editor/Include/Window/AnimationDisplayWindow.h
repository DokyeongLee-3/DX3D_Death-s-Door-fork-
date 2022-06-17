#pragma once
#include "IMGUIWindow.h"

class CAnimationDisplayWindow :
    public CIMGUIWindow
{
    friend class CEditorManager;
public:
    CAnimationDisplayWindow();
    ~CAnimationDisplayWindow();
public:
    virtual bool Init() override;
	// Animation Render Target
private:
    class CIMGUIImage* m_AnimationRenderTarget;
	// Anim Instance Make
private:
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
private:
	// FullPath ��� ��Ƶ� Vector 
	std::vector<std::string> m_vecAnimationSeqFilesFullPath;

	// FileName ��� ��Ƶ� Vector
	// std::vector<std::string> m_vecAnimationSeqFileNames;

	char m_SelectedSeqSrcsDirPath[MAX_PATH];

	class CAnimationSequenceInstance* m_DummyAnimation;

private :
	void OnAnimInstanceConvertLoading(const LoadingMessage& msg);
	void OnClickSetAnimSeqSrcDirButton();
	void OnConvertSequencesIntoAnimationInstance();
	void AddSequenceToDummyAnimationInstance(const char* FileFullPath);
};

