#pragma once

#include "IMGUIWindow.h"
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
	std::string					m_InitialPath;


	void SetUpdatePath(bool Update)
	{
		m_UpdatePath = Update;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void SetInitialPath(const std::string& Path);
	void ClearWidget();
	// ����(�Ǵ� ���丮) �̹����� Ŭ���ϸ� ȣ��Ǵ� �ݹ�
	void FileClickCallback(class CIMGUIImage* Image);
};
