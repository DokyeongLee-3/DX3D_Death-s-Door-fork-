#pragma once

#include "../GameInfo.h"

class CCollisionManager
{
private:
	std::unordered_map<std::string, CollisionProfile*>	m_mapProfile;

public:
	bool Init();
	bool CreateProfile(const std::string& Name, Collision_Channel Channel, bool Enable,
		Collision_Interaction State = Collision_Interaction::Collision);
	bool SetCollisionState(const std::string& Name, Collision_Channel Channel,
		Collision_Interaction State);
	CollisionProfile* FindProfile(const std::string& Name);
	bool DeleteProfile(const std::string& Name);
	void GetProfileNames(std::vector<std::string>& vecOutName);
	bool SaveAsCSV(const char* FileName, const std::string& Path = EXCEL_PATH);
	bool SaveAsCSVFullPath(const char* FullPath);
	bool LoadProfileFromCSV(const char* FileName, const std::string& Path = EXCEL_PATH);
	bool LoadProfileFromCSVFullPath(const char* FullPath);

	DECLARE_SINGLE(CCollisionManager)
};

