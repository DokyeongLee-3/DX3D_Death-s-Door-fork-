#pragma once

#include "GameInfo.h"

class CEngineUtil
{
public:
	static std::string LightTypeToString(Light_Type Type);
	static Light_Type StringToLightType(const std::string& TypeString);

	// String Related
	// �ش� Dir ��ο�, �ش� Name ���� �� ������ �����ϴ��� �Ǵ����ִ� �Լ�
	static bool IsFileExistInDir(const std::string& Path, const std::string& FileName);
	// Ǯ��θ� �־��ָ� ���� �̸��� �ɷ��� ��������
	static std::string FilterFileName(const std::string& FullPath);
};

