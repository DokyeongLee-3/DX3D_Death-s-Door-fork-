#pragma once

#include "IMGUIBeginMenu.h"

class CSaveLoadBeginMenu :
    public CIMGUIBeginMenu
{
public:
	CSaveLoadBeginMenu();
	virtual ~CSaveLoadBeginMenu();

private :
	// Scene
	class CIMGUIMenuItem* m_SaveSceneMenu;
	class CIMGUIMenuItem* m_LoadSceneMenu;
	// Object
	class CIMGUIMenuItem* m_SaveObjectMenu;
	class CIMGUIMenuItem* m_LoadObjectMenu;
	// Component
	class CIMGUIMenuItem* m_SaveSceneComponentMenu;
	class CIMGUIMenuItem* m_LoadSceneComponentMenu;
	// Component
	class CIMGUIMenuItem* m_SaveObjectComponentMenu;
	class CIMGUIMenuItem* m_LoadObjectComponentMenu;
public:
	virtual bool Init() override;
	virtual void Render() override;
private :
	void OnSaveSceneMenuCallback();
	void OnLoadSceneMenuCallback();

	void OnSaveObjectMenuCallback();
	void OnLoadObjectMenuCallback();

	void OnSaveSceneComponentMenuCallback();
	void OnLoadSceneComponentMenuCallback();

	void OnSaveObjectComponentMenuCallback();
	void OnLoadObjectComponentMenuCallback();

private:
	void RefreshWindow(class CGameObject* Object);
};

