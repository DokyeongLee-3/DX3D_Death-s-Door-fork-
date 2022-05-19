#pragma once

#include "../../Ref.h"

struct KeyFrame
{
	double	dTime;
	Vector3	vPos;
	Vector3	vScale;
	Vector4	vRot;
};

struct BoneKeyFrame
{
	int		iBoneIndex;
	std::vector<KeyFrame*>	vecKeyFrame;
	int			iRefCount;

	BoneKeyFrame()	:
		iRefCount(1)
	{
	}

	~BoneKeyFrame()
	{
		size_t	Size = vecKeyFrame.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(vecKeyFrame[i]);
		}

		vecKeyFrame.clear();
	}
};

struct AnimationFrameTrans
{
	Vector4 vTranslate;
	Vector4 vScale;
	Vector4 qRot;
};

class CAnimationSequence :
    public CRef
{
	friend class CAnimation;
	friend class CResourceManager;
	friend class CSceneResource;
	friend class CAnimationManager3D;
	friend class CAnimationSequenceInstance;
	friend class CAnimationSequenceData;

private:
	CAnimationSequence();
	CAnimationSequence(const CAnimationSequence& Anim);
	~CAnimationSequence();

private:
	class CScene*				m_Scene;
	bool						m_Loop;
	float						m_StartTime;
	float						m_EndTime;
	float						m_TimeLength;
	float						m_FrameTime;
	float						m_PlayTime;
	float						m_PlayScale;
	int							m_StartFrame;
	int							m_EndFrame;
	int							m_CurrentFrameIdx;
	int							m_FrameLength;
	int							m_FrameMode;
	int							m_ChangeFrame;
	bool						m_End;
	std::vector<BoneKeyFrame*>	m_vecKeyFrame;
	char						m_FullPath[MAX_PATH];
	class CStructuredBuffer* m_KeyFrameBuffer;

public:
	int GetCurrentFrameIdx() const
	{
		return m_CurrentFrameIdx;
	}
	int GetKeyFrameCount()	const
	{
		return m_FrameLength;
	}
	int GetStartFrame() const
	{
		return m_StartFrame;
	}
	int GetEndFrame() const
	{
		return m_EndFrame;
	}
	int GetFrameLength() const
	{
		return m_FrameLength;
	}
	int GetFrameMode() const
	{
		return m_FrameMode;
	}
	float GetPlayTime() const
	{
		return m_PlayTime;
	}
	float GetPlayScale() const
	{
		return m_PlayScale;
	}
	float GetFrameTime() const
	{
		return m_FrameTime;
	}

public:
	CAnimationSequence* Clone();
	bool IsSequenceEnd()	const;
	void GetKeyFrame(std::vector<BoneKeyFrame*>& vecFrame);
	bool LoadFbxAnimation(const char* pFullPath, bool bLoop = false);
	bool Save(const TCHAR* pFileName, const std::string& strPathName = MESH_PATH);
	bool SaveMultibyte(const char* pFileName, const std::string& strPathName = MESH_PATH);
	bool SaveFullPath(const TCHAR* pFullPath);
	bool SaveFullPathMultibyte(const char* pFullPath);
	bool Load(const TCHAR* pFileName, const std::string& strPathName = MESH_PATH);
	bool LoadMultibyte(const char* pFileName, const std::string& strPathName = MESH_PATH);
	bool LoadFullPath(const TCHAR* pFullPath);
	bool LoadFullPathMultibyte(const char* pFullPath);

	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
public:
	void SetPlayScale(float fScale);
	void SetPlayTime(float fTime);
	BoneKeyFrame* DeleteAnimationFrame(int Index);
public:
	bool CreateSequence(bool bLoop, struct _tagFbxAnimationClip* pClip);
	bool CreateSequence(const std::string& strName, bool bLoop,
		int iStartFrame, int iEndFrame, float fPlayTime,
		const std::vector<BoneKeyFrame*>& vecFrame);
	bool CreateSequence(const TCHAR* pFullPath);
	bool CreateSequenceMultibyte(const char* pFullPath);
	bool CreateSequence(bool bLoop, const std::string& strName, const TCHAR* pFileName,
		const std::string& strPathName = MESH_PATH);
	void SetShader();
	void ResetShader();
	void SetChangeShader();
	void ResetChangeShader();
	void Clear();
	void AddNotify(const std::string& Name, float Time);

	// FBX Convert �뵵�� ���
	// KeyFrameBuffer ������ ����
	bool CreateSequenceConvertFBX(bool bLoop, struct _tagFbxAnimationClip* pClip);
};

