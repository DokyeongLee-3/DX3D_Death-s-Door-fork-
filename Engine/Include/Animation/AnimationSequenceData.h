#pragma once

#include "../GameInfo.h"

struct AnimationNotify
{
	std::string	Name;
	int		Frame;
	bool	Call;
	std::function<void()>	Function;
	std::function<void(NotifyParameter)>	FunctionParam;
	NotifyParameter Param;

	AnimationNotify() :
		Frame(0),
		Call(false),
		Function(nullptr),
		FunctionParam(nullptr),
		Param{}
	{
	}
};

class CAnimationSequenceData
{
	friend class CAnimationSequenceInstance;

private:
	CAnimationSequenceData();
	~CAnimationSequenceData();

private:
	std::string	m_Name;
	std::string	m_SequenceName;
	CSharedPtr<class CAnimationSequence>	m_Sequence;
	float	m_Time;			// �ִϸ��̼� ���� �ð�
	float	m_FrameTime;	// 1�����Ӵ� �ð�
	float	m_PlayTime;
	float	m_PlayScale;		// ��� ����
	bool	m_Loop;
	std::function<void()>	m_EndFunction;
	std::vector<AnimationNotify*>	m_vecNotify;
public:
	void Save(FILE* File);
	void Load(FILE* File);
public:
	void SetPlayTime(float Time);
	void SetPlayScale(float Scale)
	{
		m_PlayScale = Scale;
	}
	void SetOriginalFramePlayTime();
public:
	bool IsLoop() const
	{
		return m_Loop;
	}

	const std::string& GetName()	const
	{
		return m_Name;
	}

	float GetAnimationTime()	const
	{
		return m_Time;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	class CAnimationSequence* GetAnimationSequence()	const
	{
		return m_Sequence;
	}

	float GetAnimationPlayScale()   const
	{
		return m_PlayScale;
	}

	float GetAnimationPlayTime()   const
	{
		return m_PlayTime;
	}

public:
	template <typename T>
	void SetEndFunction(T* Obj, void (T::* Func)())
	{
		m_EndFunction = std::bind(Func, Obj);
	}

	template <typename T>
	void AddNotify(const std::string& Name, int Frame, T* Obj, void (T::* Func)())
	{
		AnimationNotify* Notify = new AnimationNotify;

		Notify->Name = Name;
		Notify->Frame = Frame;
		Notify->Function = std::bind(Func, Obj);

		m_vecNotify.push_back(Notify);
	}


	template <typename T>
	void AddNotifyParam(const std::string& Name, int Frame, T* Obj, void (T::* Func)(const NotifyParameter&))
	{
		AnimationNotify* Notify = new AnimationNotify;

		Notify->Name = Name;
		Notify->Frame = Frame;
		Notify->FunctionParam = std::bind(Func, Obj, std::placeholders::_1);

		m_vecNotify.push_back(Notify);
	}
};

