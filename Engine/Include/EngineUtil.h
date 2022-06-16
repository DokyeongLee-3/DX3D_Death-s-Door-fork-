#pragma once

#include "GameInfo.h"

class CEngineUtil
{
public:
	static std::string LightTypeToString(Light_Type Type);
	static Light_Type StringToLightType(const std::string& TypeString);

	// String Related
	// �ش� Dir ��ο�, �ش� Name ���� �� ������ �����ϴ��� �Ǵ����ִ� �Լ� + ������ �� FullPath ��� ����
	static std::optional<std::string> CheckAndExtractFullPathOfTargetFile(std::string_view Path, std::string_view FileName);

	// (OBJ : Ȥ�ó� ����ϰ� �ִ� ����� �ֳ��Ͽ�, ���� �Լ� ���ܵ�) �ش� Dir ��ο�, �ش� Name ���� �� ������ �����ϴ��� �Ǵ����ִ� �Լ� 
	static bool IsFileExistInDir(const std::string& Path, const std::string& FileName);

	// Ǯ��θ� �־��ָ� ���� �̸��� �ɷ��� ��������
	static std::string FilterFileName(const std::string& FullPath);
	
	// FullPath �� C::\\Windows\\Project\\Bin\\Hello.txt �̰�, 
	// FolderName �� Project ��� => Bin\\Hello.txt ��� Path ������ return ���ش�.
	// std::string_view GetFilePathAfterSpecificFolder(std::string_view FolderName, std::string_view FullPath);
};

