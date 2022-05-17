#pragma once
#include "IMGUIWidget.h"
class CIMGUIButton :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;
	friend class CIMGUIPopUpModal;
	friend class CIMGUIWidgetList;

protected:
	CIMGUIButton();
	virtual ~CIMGUIButton();

protected:
	std::function<void()>	m_ClickCallback;
	bool					m_Click;

public:
	virtual bool Init();
	virtual void Render();

public:
	template <typename T>
	void SetClickCallback(T* Obj, void(T::* Func)())
	{
		m_ClickCallback = std::bind(Func, Obj);
	}
};

