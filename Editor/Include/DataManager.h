#pragma once

#include "GameInfo.h"

// csv���� �������� �о���� Manager
class CDataManager
{
	friend class CEditorManager;

private:
	CDataManager();
	~CDataManager();

private:
	std::unordered_map <std::string, class CExcelData*> m_mapData;

public:
	virtual void Init();

private:
	void ReadObjectData();
	void ReadNotifyData();

public:
	void SetObjectData(class CGameObject* Object);
	void SetNotify(class CGameObject* Object, const std::string& ObjectName);

public:
	class CExcelData* FindData(const std::string& Name);
};

