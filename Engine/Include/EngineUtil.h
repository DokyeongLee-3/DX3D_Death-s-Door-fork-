#pragma once

#include "GameInfo.h"

class CEngineUtil
{
public:
	// Enum To String
	static std::string LightTypeToString(Light_Type Type);
	static Light_Type StringToLightType(const std::string& TypeString);

	static std::string FogTypeToString(Fog_Type Type);
	static Fog_Type StringToFogType(const std::string& TypeString);

	// String Related
	// �ش� Dir ��ο�, �ش� Name ���� �� ������ �����ϴ��� �Ǵ����ִ� �Լ� + ������ �� FullPath ��� ����
	static std::optional<std::string> CheckAndExtractFullPathOfTargetFile(std::string_view Path, std::string_view FileName);

	// (OBJ : Ȥ�ó� ����ϰ� �ִ� ����� �ֳ��Ͽ�, ���� �Լ� ���ܵ�) �ش� Dir ��ο�, �ش� Name ���� �� ������ �����ϴ��� �Ǵ����ִ� �Լ� 
	static bool IsFileExistInDir(const std::string& Path, const std::string& FileName);

	// Ǯ��θ� �־��ָ� ���� �̸��� �ɷ��� ��������
	static std::string FilterFileName(const std::string& FullPath);

	// Collision Profile
	static std::string CollisionChannelToString(Collision_Channel eChannnel);
	static Collision_Channel StringToCollisionChannel(const std::string& String);
	static Collision_Interaction StringToCollisionInteraction(const std::string& String);

	// String
	static std::string BoolToString(bool Bool);
};

