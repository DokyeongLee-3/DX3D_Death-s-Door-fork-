#pragma once

#include "GameInfo.h"
#include "Timer.h"

class CEngine
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	Resolution	m_RS;
	static bool	m_Loop;
	float		m_ClearColor[4];
	class CTimer* m_Timer;
	Engine_Space	m_Space;
	int				m_ShowCursorCount;
	Mouse_State		m_MouseState;
	CSharedPtr<class CWidgetWindow>	m_MouseWidget[(int)Mouse_State::Max];
	class CGlobalConstantBuffer* m_GlobalCBuffer;
	float			m_GlobalAccTime;
	CSharedPtr<class CTexture>	m_GlobalNoiseTexture;
	class CStructuredBuffer* m_RandomBuffer;
	bool					m_EditMode; // true -> 

	bool		m_Start;
	bool		m_Play;
	bool		m_Pause;

public:
	void SetMouseState(Mouse_State State);

	void SetClearColor(const Vector4& Color)
	{
		m_ClearColor[0] = Color.x;
		m_ClearColor[1] = Color.y;
		m_ClearColor[2] = Color.z;
		m_ClearColor[3] = Color.w;
	}

	void EnableEditMode()
	{
		m_EditMode = true;
	}

	bool GetEditMode() const
	{
		return m_EditMode;
	}

	class CWidgetWindow* GetMouseWidget()	const
	{
		return m_MouseWidget[(int)m_MouseState];
	}

	Engine_Space GetEngineSpace()	const
	{
		return m_Space;
	}

	void SetEngineSpace(Engine_Space Space)
	{
		m_Space = Space;
	}

	bool IsPlay()	const
	{
		return m_Play;
	}

	bool IsPause() const
	{
		return m_Pause;
	}

	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}

	void Exit()
	{
		DestroyWindow(m_hWnd);
	}

	float GetFPS()	const
	{
		return m_Timer->GetFPS();
	}

	float GetDeltaTime()	const
	{
		return m_Timer->GetDeltaTime();
	}

	Vector4 GetClearColor() const
	{
		return Vector4(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	}

public:
	bool Init(HINSTANCE hInst, const TCHAR* Name, unsigned int Width,
		unsigned int Height, int IconID,
		bool WindowMode = true);
	bool Init(HINSTANCE hInst, HWND hWnd, unsigned int Width,
		unsigned int Height, bool WindowMode = true);
	int Run();
	void Logic();

public:
	void Play();
	void Pause();
	void Resume();
	void Stop();

private:
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Render(float DeltaTime);

private:
	ATOM Register(const TCHAR* Name, int IconID);
	BOOL Create(const TCHAR* Name);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	template <typename T>
	T* CreateMouse(Mouse_State State, const std::string& Name)
	{
		T* Window = new T;

		Window->SetName(Name);

		if (!Window->Init())
		{
			SAFE_RELEASE(Window);
			return nullptr;
		}

		m_MouseWidget[(int)State] = Window;

		return Window;
	}

	DECLARE_SINGLE(CEngine)
};

