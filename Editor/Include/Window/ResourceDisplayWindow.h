#pragma once
#include "IMGUIWindow.h"

class CResourceDisplayWindow :
    public CIMGUIWindow
{
	friend class CEditorManager;
	// friend class CAnimationInstanceConvertThread;
public:
	CResourceDisplayWindow();
	~CResourceDisplayWindow();
private :
	// ���� Project �� Resource Manager �� Load �� ��ϵ��� ���� Window
	// - ���⿡��, Drag �ؼ�, Material Window ���� ���� Drop �ϸ�, �ش� ��ġ�� ���� ������
	// - ���õǴ� ������ �ڵ带 �ۼ��� ���̴�.
	// Texture
	class CIMGUIWidgetList* m_TextureList;
	// Shader
	class CIMGUIWidgetList* m_ShaderList;
	// Material
	class CIMGUIWidgetList* m_MaterialList;
private :
	std::vector<class CIMGUIWidgetList*> m_vecResourceTapList;
public :
	virtual bool Init() override;
public  :
	void GatherLoadedTextureResources();
	void GatherLoadedMaterialResources();
};

