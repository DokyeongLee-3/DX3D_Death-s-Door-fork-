#pragma once

#include "IMGUIWindow.h"
#include "IMGUIMenuItem.h"
#include "IMGUIChild.h"

class CFileBrowser :
	public CIMGUIWindow
{
public:
	CFileBrowser();
	virtual ~CFileBrowser();

private:
	std::vector<std::string>	m_vecFileName;
	std::vector<std::string>	m_vecDirName;
	std::vector<CIMGUIChild*>	m_vecChild;

	bool						m_UpdatePath;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	std::string m_InitialPath;

public:
	void SetInitialPath(const std::string& Name);
	void ClearWidget();
	// ����(�Ǵ� ���丮) �̹����� Ŭ���ϸ� ȣ��Ǵ� �ݹ�
	void FileClickCallback(class CIMGUIImage* Image);

};
