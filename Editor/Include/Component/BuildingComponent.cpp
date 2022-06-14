
#include "BuildingComponent.h"

CBuildingComponent::CBuildingComponent() :
	m_TowerOrder(1)
{
	m_BuildingLine = LoLLine::Top;
	SetTypeID<CBuildingComponent>();
}

CBuildingComponent::CBuildingComponent(const CBuildingComponent& com) :
	CSceneComponent(com)
{
}

CBuildingComponent::~CBuildingComponent()
{
}

bool CBuildingComponent::Save(FILE* File)
{
	fwrite(&m_BuildingType, sizeof(Building_Type), 1, File);
	fwrite(&m_BuildingLine, sizeof(LoLLine), 1, File);
	fwrite(&m_TowerOrder, sizeof(int), 1, File);

	CSceneComponent::Save(File);

	return true;
}

bool CBuildingComponent::Load(FILE* File)
{
	fread(&m_BuildingType, sizeof(Building_Type), 1, File);
	fread(&m_BuildingLine, sizeof(LoLLine), 1, File);
	fread(&m_TowerOrder, sizeof(int), 1, File);

	CSceneComponent::Load(File);

	return true;
}

bool CBuildingComponent::SaveOnly(FILE* File)
{
	return true;
}

bool CBuildingComponent::LoadOnly(FILE* File)
{
	return true;
}
