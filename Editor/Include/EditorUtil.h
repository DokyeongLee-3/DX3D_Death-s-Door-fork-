#pragma once

#include "GameInfo.h"
#include "Flag.h"
#include "EditorInfo.h"

class CEditorUtil
{
public:
	// Ǯ��θ� �־��ָ� ���� �̸��� �ɷ��� ��������
	static std::string FilterFileName(const std::string& FullPath);

	// �ش� ��ο� �����ϴ� ��� ���� �̸��� ���丮 �̸����� ������
	static void GetAllFilenames(const std::string& PathName, std::vector<std::string>& vecFileNames,
		std::vector<std::string>& vecDirNames);
	static void GetAllFilenamesFullPath(const std::string& FullPath, std::vector<std::string>& vecFileNames,
		std::vector<std::string>& vecDirNames);

	// �ش� ���丮�� �����ϰ�, Ȯ���ڿ� ��ġ�ϴ� ���ϵ��� Ǯ ��θ� ������
	static void GetAllFileFullPathInDir(const char* TargetDir, std::vector<std::string>& OutVecFullPath,
		const char* ExtFilter = nullptr);

	// Imgui Demo Window�� ����ִ� �Լ�
	static void ShowDemo();

	static std::string LoLObjectToString(LoLObject Object);

	static std::string SceneComponent3DTypeToString(SceneComponent3DType Type);
	static std::string ObjectComponent3DTypeToString(ObjectComponent3DType Type);

	static size_t ObjectTypeIndexToTypeid(int TypeIndex);
	static size_t SceneComponentTypeIndexToTypeid(int TypeIndex);
	static size_t ObjectComponentTypeIndexToTypeid(int TypeIndex);

private:
	static bool CompareExt(const char* FullPath, const char ExtFilter[_MAX_EXT]);

};