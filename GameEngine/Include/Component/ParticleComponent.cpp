
#include "ParticleComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneManager.h"
#include "../Scene/CameraManager.h"
#include "../PathManager.h"
#include "../Component/CameraComponent.h"
#include "../Resource/Shader/StructuredBuffer.h"

CParticleComponent::CParticleComponent()	:
	m_SpawnTime(0.f),
	m_SpawnTimeMax(0.01f),
	m_Info{},
	m_BillBoardEffect(false),
	m_InfoShared{},
	m_CBuffer(nullptr)
{
	SetTypeID<CParticleComponent>();
	m_Render = true;

	m_LayerName = "Particle";
}

CParticleComponent::CParticleComponent(const CParticleComponent& com) :
	CSceneComponent(com)
{
	m_SpawnTime = 0.f;
	m_SpawnTimeMax = com.m_SpawnTimeMax;

	m_Particle = com.m_Particle;

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	m_vecStructuredBuffer.clear();

	if (m_Particle)
	{
		m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);

		m_UpdateShader = m_Particle->GetUpdateShader();

		SAFE_DELETE(m_CBuffer);

		m_CBuffer = m_Particle->CloneConstantBuffer();
	}

}

CParticleComponent::~CParticleComponent()
{
	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	SAFE_DELETE(m_CBuffer);
}

void CParticleComponent::SetParticle(const std::string& Name)
{
	SetParticle(m_Scene->GetResource()->FindParticle(Name));
}

void CParticleComponent::SetParticle(CParticle* Particle)
{
	m_Particle = Particle;

	m_Material = m_Particle->CloneMaterial();
	
	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	SAFE_DELETE(m_CBuffer);

	m_vecStructuredBuffer.clear();

	m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);

	m_UpdateShader = m_Particle->CloneUpdateShader();

	m_CBuffer = m_Particle->CloneConstantBuffer();

	m_SpawnTimeMax = m_Particle->GetSpawnTimeMax();

	m_ParticleName = m_Particle->GetName();
}

void CParticleComponent::SetSpawnTime(float Time)
{
	m_SpawnTimeMax = Time;

	m_Particle->SetSpawnTimeMax(m_SpawnTimeMax);
}

void CParticleComponent::ApplyBillBoardEffect()
{
	Vector3 CameraPos = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetWorldPos();

	Vector3 View = CameraPos - GetWorldPos();

	View.Normalize();

	// float 
	Vector3 OriginDir = Vector3(0.f, 0.f, -1.f);

	m_Transform->SetRotationAxis(OriginDir, View);
}

void CParticleComponent::Start()
{
	CSceneComponent::Start();
}

bool CParticleComponent::Init()
{
	m_Mesh = m_Scene->GetResource()->FindMesh("ParticlePointMesh");

	return true;
}

void CParticleComponent::Update(float DeltaTime)
{
	if (!m_CBuffer)
		return;

	CSceneComponent::Update(DeltaTime);

	m_SpawnTime += DeltaTime;

	if (m_SpawnTime >= m_SpawnTimeMax)
	{
		m_SpawnTime -= m_SpawnTimeMax;
		m_CBuffer->SetSpawnEnable(1);
	}

	else
		m_CBuffer->SetSpawnEnable(0);

	// �߰� : Particle �� BillBoard �� �����ϱ����� OBJ �� �߰�
	if (m_BillBoardEffect)
	{
		ApplyBillBoardEffect();
	}
}

void CParticleComponent::PostUpdate(float DeltaTime)
{
	if (!m_CBuffer)
		return;

	CSceneComponent::PostUpdate(DeltaTime);

	CParticleConstantBuffer* CBuffer = m_Particle->GetCBuffer();

	// Update Shader�� ���۽�Ų��.
	Vector3	StartMin, StartMax;

	StartMin = GetWorldPos() + CBuffer->GetStartMin();
	StartMax = GetWorldPos() + CBuffer->GetStartMax();

	m_CBuffer->SetStartMin(StartMin);
	m_CBuffer->SetStartMax(StartMax);

	// Rotation Angle ������ �����Ѵ�. Transform �� ������ ���� ���̴�.
	m_CBuffer->SetRotationAngle(GetWorldRot());

	m_CBuffer->UpdateCBuffer();

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader();
	}

	// UpdateShader�� Thread�� 64, 1, 1�� ����ϰ� �ִ�.
	// �����Ǿ��� ��ƼŬ�� ��ü ������ 64���� ������. ���� 64���� �ִ���ƼŬ ���� �������־��ٸ�
	// �ʿ��� �׷��� ���� 1���̴�. ������ 64�� �̸��̶�� 64�� ���� ��� 0�� �����Ƿ� ���⿡ 1�� �����־�� �Ѵ�.
	// 100���� ��� �׷��� 2���� �����ȴ�. �̶� ������� 128���� �ǹǷ� 100���� ������ ������ 28���� ó���� �ȵǰ�
	// �����ָ� �Ǵ°��̴�.
	int	GroupCount = m_Particle->GetSpawnCountMax() / 64 + 1;
	m_UpdateShader->Excute(GroupCount, 1, 1);


	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader();
	}


	// ���⿡�� Data�� CopyResource�� �����ؼ� �׽�Ʈ �غ���.
}

void CParticleComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CParticleComponent::RenderParticleEffectEditor()
{
	if (!m_CBuffer)
		return;

	CSceneComponent::RenderParticleEffectEditor();

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
		m_Material->Render();

	// �ν��Ͻ��� �̿��ؼ� �׷��ش�.
	m_Mesh->RenderInstancing(m_CBuffer->GetSpawnCount());

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
		m_Material->Reset();
}

void CParticleComponent::Render()
{
	if (!m_Material)
		return;

	CSceneComponent::Render();

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
		m_Material->Render();


	// �ν��Ͻ��� �̿��ؼ� �׷��ش�.
	m_Mesh->RenderInstancing(m_CBuffer->GetSpawnCount());


	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}


	if (m_Material)
		m_Material->Reset();
}

void CParticleComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CParticleComponent* CParticleComponent::Clone()
{
	return new CParticleComponent(*this);
}

bool CParticleComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	bool Result = SaveOnly(File);

	return Result;
}

bool CParticleComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);

	LoadOnly(File);

	return true;
}

bool CParticleComponent::SaveOnly(FILE* File)
{
	// Mesh
	std::string	MeshName = m_Mesh->GetName();
	int	Length = (int)MeshName.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(MeshName.c_str(), sizeof(char), Length, File);

	if (m_Particle->GetName() == "")
	{
		// Particle �� ���, Particle Editor �� ���ؼ� ������ ��, �ݵ�� Particle �̸��� �����ϰ� �Ǿ� �ִ�.
		// Particle �� �̸���, Particle File�� ����� ���� �̸��� �����ϴ�.
		assert(false); 
	}

	// Ȥ�� �𸣴� �ѹ� �� �������ش�.
	m_ParticleName = m_Particle->GetName();
	int	 ParticleNameLength = (int)m_ParticleName.length();
	fwrite(&ParticleNameLength, sizeof(int), 1, File);
	fwrite(m_ParticleName.c_str(), sizeof(char), ParticleNameLength, File);

	// Particle
	// m_Particle->Save(File);
	std::string SaveParticleFileFullPath;
	SaveParticleFileFullPath.reserve(100);

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PARTICLE_PATH);

	if (Info)
		SaveParticleFileFullPath = Info->PathMultibyte;
	SaveParticleFileFullPath.append(m_Particle->GetName().c_str());

	// .ptrc �� �ش� Name�� �ִ��� Ȯ�� ��, ������ Add
	if (SaveParticleFileFullPath.find(".prtc") == std::string::npos)
		SaveParticleFileFullPath.append(".prtc");

	m_Particle->SaveFile(SaveParticleFileFullPath.c_str());

	fwrite(&m_BillBoardEffect, sizeof(bool), 1, File);
	fwrite(&m_SpawnTimeMax, sizeof(float), 1, File);

	return true;
}

bool CParticleComponent::LoadOnly(FILE* File)
{
	// Mesh
	char	MeshName[256] = {};
	int	Length = 0;
	fread(&Length, sizeof(int), 1, File);
	fread(MeshName, sizeof(char), Length, File);
	m_Mesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh(MeshName);

	// Particle ����
	m_Particle = CSceneManager::GetInst()->GetScene()->GetResource()->CreateParticleEmpty<CParticle>();
	// m_Particle->Load(File);

	char LoadedParticleName[MAX_PATH] = {};
	int	 ParticleNameLength = (int)m_ParticleName.length();
	fread(&ParticleNameLength, sizeof(int), 1, File);
	fread(LoadedParticleName, sizeof(char), ParticleNameLength, File);

	// Particle
	// m_Particle->Save(File);
	std::string LoadParticleFileFullPath;
	LoadParticleFileFullPath.reserve(100);

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PARTICLE_PATH);

	if (Info)
		LoadParticleFileFullPath = Info->PathMultibyte;
	LoadParticleFileFullPath.append(LoadedParticleName);

	// .ptrc �� �ش� Name�� �ִ��� Ȯ�� ��, ������ Add
	if (LoadParticleFileFullPath.find(".prtc") == std::string::npos)
		LoadParticleFileFullPath.append(".prtc");

	bool Result = m_Particle->LoadFile(LoadParticleFileFullPath.c_str());

	if (!Result)
	{
		assert(false);
	}

	m_ParticleName = m_Particle->GetName();

	SetParticle(m_Particle);

	// Load �� Particle �� Particle Manager �� �߰����ش�.
	CResourceManager::GetInst()->GetParticleManager()->AddParticle(m_Particle);

	fread(&m_BillBoardEffect, sizeof(bool), 1, File);
	fread(&m_SpawnTimeMax, sizeof(float), 1, File);

	return true;
}
