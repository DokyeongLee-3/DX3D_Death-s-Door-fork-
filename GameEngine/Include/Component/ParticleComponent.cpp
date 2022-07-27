
#include "ParticleComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneManager.h"
#include "../Scene/CameraManager.h"
#include "../PathManager.h"
#include "../Component/CameraComponent.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../EngineUtil.h"

CParticleComponent::CParticleComponent()	:
	m_SpawnTime(0.f),
	m_SpawnTimeMax(0.01f),
	m_Info{},
	m_BillBoardEffect(false),
	m_BazierMoveEffect(false),
	m_ParticleMoveSpeed(20.f),
	m_ParticleMoveSpeedBottom(3.5f),
	m_InfoShared{},
	m_SpeedChangeMethod(ParticleSpeedChangeMethod::Linear), // Particle Component �� ���, Linear ������, �׳� ���� Speed ���� 
	m_CBuffer(nullptr)
{
	SetTypeID<CParticleComponent>();
	m_Render = true;

	m_LayerName = "Particle";

	// ó������ Particle Component �� Dir �� (0, 0, -1) �� ����
	// �ֳ��ϸ�, Particle Component ��ü��, z �� ���� -1 �������� ���ϸ鼭
	// ����� ���� �ٶ󺸰� �ֱ� �����̴�.
	m_ParticleMoveDir = Vector3(0, 0, -1);
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
	// SAFE_DELETE(m_NormalDistributionBuffer);

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

	// SAFE_DELETE(m_NormalDistributionBuffer);

	m_vecStructuredBuffer.clear();

	m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);

	// ���� ���� ������ ���, �� �ѹ��� Update ���ش�. (�켱 ��� X)
	// m_Particle->CloneNormalDistStructuredBuffer(m_NormalDistributionBuffer);
	// std::vector<float> VecNormalDistVal = m_Particle->GetVecNormalDistVal();
	// m_NormalDistributionBuffer->UpdateBuffer(&VecNormalDistVal[0], (int)VecNormalDistVal.size());

	m_UpdateShader = m_Particle->CloneUpdateShader();

	SAFE_DELETE(m_CBuffer);

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

void CParticleComponent::ApplyBazierMove(float DeltaTime)
{
	if (!m_BazierMoveEffect)
		return;

	m_ParticleMoveAccTime += DeltaTime;

	// �����ϴ� ȿ�� �ֱ� 
	switch (m_SpeedChangeMethod)
	{
		case ParticleSpeedChangeMethod::Exponential :
		{
			m_ParticleMoveSpeed = CEngineUtil::CalculateRealTimeSpeedUsingExponentialWithBottom(m_ParticleMoveSpeedBottom, m_ParticleMoveAccTime, m_ParticleMoveInitSpeed);
		}
		break;
	}

	// ��ġ �̵�
	float BazierMoveDist = (m_ParticleMoveDir * m_ParticleMoveSpeed * DeltaTime).Length();

	m_Object->GetRootComponent()->AddWorldPos(m_ParticleMoveDir * m_ParticleMoveSpeed * DeltaTime);

	m_BazierMoveCurDist += BazierMoveDist;

	// ��ǥ ��ġ�� ���� �̵��ߴٸ�, ���� ��ġ�� �̾Ƽ� �ش� ��ġ�� �̵��Ѵ�.
	const Vector3& CurrentWorldPos = GetWorldPos();

	if (m_BazierMoveTargetDist <= m_BazierMoveCurDist)
	{
		if (!m_queueBazierMovePos.empty())
		{
			Vector3 NextPos = m_queueBazierMovePos.front();
			m_queueBazierMovePos.pop();
			m_ParticleNextMovePos = NextPos;

			Vector3 PrevMoveDir = m_ParticleMoveDir;

			m_ParticleMoveDir = m_ParticleNextMovePos - GetWorldPos();
			m_ParticleMoveDir.Normalize();

			m_BazierMoveTargetDist = m_ParticleNextMovePos.Distance(CurrentWorldPos);

			m_BazierMoveCurDist = 0.f;

			// >> �̵� ���⿡ ����, Rotation �������ش�.
			// 1. ���� Particle Component �� �ٶ󺸴� ������, (0, 0, -1) �̴�.
			// 2. ������, ���� ó�� Particle Component ���� ���ϴ� ������, y �� 1 ���� (0, 1, 0)
			// - ����, �⺻������ m_ParticleMoveDir �� ���ؼ�, X �� ����, 90�� ȸ���� �⺻������ ����� �Ѵ�.
			// 3. �Ӹ� �ƴ϶�, Particle ������ ���� Rot Angle �� �ִ�. �̰��� ��ġ Offset ���� ó�� ������ ���̴�.
			// 4. ���� Dir, ���� Dir ���� Angle �� ���ϰ�, �̰͸�ŭ Particle Dir �� ȸ����ų ���̴�.
			// - ���� ���, X �� ���� ���������� ���ٰ�, Y �� ���� �������� ���ٴ� ����, ���� Angle �� Z �� ���� 90 �� ȸ��
			// float RotAngle = m_ParticleMoveDir.Angle(PrevMoveDir);
			// const Vector3& RotAxis = PrevMoveDir.Cross(m_ParticleMoveDir);
			// 
			// if (XMVector3Equal(RotAxis.Convert(), XMVectorZero()) == false)
			// {
			// 	XMVECTOR Qut = XMQuaternionRotationAxis(RotAxis.Convert(), RotAngle);
			// 
			// 	const Vector3& EulerRotAngle = CEngineUtil::QuarternionToEulerAngles(Qut);
			// 
			// 	AddWorldRotation(EulerRotAngle);
			// }
		}
		else
		{
			m_BazierMoveEffect = false;
		}
	}
}

void CParticleComponent::SetBazierTargetPos(const Vector3& D2, const Vector3& D3, const Vector3& D4, int DetailNum)
{
	CEngineUtil::CalculateBazierTargetPoses(GetWorldPos(), D2, D3, D4, m_queueBazierMovePos, 100);

	// ó�� �Ѱ��� �̾Ƶд�.
	if (!m_queueBazierMovePos.empty())
	{
		Vector3 NextPos = m_queueBazierMovePos.front();
		m_queueBazierMovePos.pop();

		m_ParticleNextMovePos = NextPos;

		m_ParticleMoveDir = m_ParticleNextMovePos - GetWorldPos();
		m_ParticleMoveDir.Normalize();

		const Vector3& CurrentWorldPos = GetWorldPos();

		m_BazierMoveTargetDist = m_ParticleNextMovePos.Distance(CurrentWorldPos);

		m_BazierMoveCurDist = 0.f;

		m_ParticleMoveAccTime = 0.f;

		m_ParticleMoveInitSpeed = m_ParticleMoveSpeed;
	}
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

void CParticleComponent::Reset()
{
	CSceneComponent::Reset();

	ResetParticleStructuredBufferInfo();
}

void CParticleComponent::Update(float DeltaTime)
{
	if (!m_CBuffer)
		return;

	if (m_InitActiveDelayTime > 0.f)
	{
		m_InitActiveDelayTime -= DeltaTime;
		return;
	}

	CSceneComponent::Update(DeltaTime);

	m_SpawnTime += DeltaTime;

	// CBuffer �� DisableNewAlive �� true ���, �ѹ��� �� �����ع����� �ϴ� ��
	// �� ��� �� �ϳ��� m_SpawnTimeMax �� 0���� ���������� ��
	if (m_CBuffer->IsDisableNewAlive() == 1)
	{
		m_SpawnTimeMax = 0.f;
	}

	// Spawn Time ����
	if (m_SpawnTime >= m_SpawnTimeMax)
	{
		m_SpawnTime -= m_SpawnTimeMax;
		m_CBuffer->SetSpawnEnable(1);

		// SpawnTime �� 0���� �����.
		if (m_CBuffer->IsDisableNewAlive() == 1)
		{
			m_SpawnTime = 0.f;
		}
	}
	else
	{
		m_CBuffer->SetSpawnEnable(0);
	}

	// �߰� : Particle �� BillBoard �� �����ϱ����� OBJ �� �߰�
	if (m_BillBoardEffect)
	{
		ApplyBillBoardEffect();
	}

	// Bazier ������� �����̰� �� ���δ�
	if (m_BazierMoveEffect)
	{
		// Bazier �� ����� ��ġ ������ �̵��� ���δ�.
		ApplyBazierMove(DeltaTime);
	}
}

void CParticleComponent::PostUpdate(float DeltaTime)
{
	if (!m_CBuffer)
		return;

	if (m_InitActiveDelayTime > 0.f)
		return;

	CSceneComponent::PostUpdate(DeltaTime);

	CParticleConstantBuffer* CBuffer = m_Particle->GetCBuffer();

	// Update Shader�� ���۽�Ų��.
	// Start Min, Max �� ���, 
	// 1) Particle Component �� World Rot �� �ݿ��Ͽ� ȸ�� ��Ų ����
	// 2) Particle Component �� Scalilng ���� �����Ѵ�. (�켱 RelativeScale �� ����)
	// 3) Translation�� ó������ �ʴ´�. StartMin, Max ��, Local Space �󿡼��� Min, Max �� �ǹ��ϰ� �� ���̴�.
	// ( Rot  -> Translation )
	Vector3	StartMin = CBuffer->GetStartMin() * GetWorldScale();
	StartMin.TransformCoord(GetRotationMatrix());

	Vector3	StartMax = CBuffer->GetStartMax() * GetWorldScale();
	StartMax.TransformCoord(GetRotationMatrix());
	
	// StartMin, Max ��, World Pos �� �������� �ʰ� �Ѱ��� ���̴�.
	// ���� Particle Component Local Space �󿡼��� Min, Max �� ���� �������� ���̴�.
	// StartMin += GetWorldPos();
	// StartMax += GetWorldPos();

	// (���� �ڵ�)
	// StartMin = GetWorldPos() + CBuffer->GetStartMin();
	// StartMax = GetWorldPos() + CBuffer->GetStartMax();

	m_CBuffer->SetStartMin(StartMin);
	m_CBuffer->SetStartMax(StartMax);

	// BillBoard ȿ���� �ְ� �Ǹ�, ParticleComponent �� Y�ุ ȸ����Ų��. (ī�޶� ��������)
	Vector3 BillBoardAngle;

	if (m_BillBoardEffect)
	{
		// ī�޶� ��� �ٶ󺸰� �����.
		// ī�޶��� ��ġ�� ���´�.
		Vector3 CameraPos = m_Scene->GetCameraManager()->GetCurrentCamera()->GetWorldPos();

		// x ��
		{
		}

		// y��
		{
			Vector3	View = CameraPos - GetWorldPos();
			View.y = 0.f;

			Vector3 ZLookDir = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
			Vector3 XLookDir = m_Object->GetWorldAxis(AXIS::AXIS_X) * -1.f;

			float AngleBetween = ZLookDir.Angle(View);

			// Object ���� ������ �ִٸ�, 180 ���� �Ѿ�� ��
			// ������ acos �� 0 ���� 180 �� ������ ������ �����Ѵ�.
			if (View.Dot(XLookDir) > 0)
			{
				AngleBetween = 180.f + (180.f - AngleBetween);
			}

			BillBoardAngle.y = AngleBetween;
		}

		// z ��
		{
		}
		
	}

	// Rotation Angle ������ �����Ѵ�. Transform �� ������ ���� ���̴�.
	// m_CBuffer->SetRotationAngle(GetWorldRot());
	m_CBuffer->SetRotationAngle(GetWorldRot() + BillBoardAngle);
	
	// Relative Scale ������ �����Ѵ�.
	m_CBuffer->SetCommonWorldScale(GetWorldScale());
	m_CBuffer->SetCommonParticleComponentWorldPos(GetWorldPos());

	m_CBuffer->UpdateCBuffer();

	// Normal Dist ����ȭ ���� ������ �Ѱ��ش�.
	// m_NormalDistributionBuffer->SetShader();

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

	// m_NormalDistributionBuffer->ResetShader();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader();
	}
	// ���⿡�� Data�� CopyResource�� �����ؼ� �׽�Ʈ �غ���.
}

void CParticleComponent::PrevRender()
{
	if (m_InitActiveDelayTime > 0.f)
		return;

	CSceneComponent::PrevRender();
}

void CParticleComponent::RenderParticleEffectEditor()
{
	if (!m_CBuffer)
		return;

	// ��� ���۸� �ٽ� �ѹ� Setting ���ش�.
	// ��� ���̴� �ܿ���, Render ���������� ��� ���� ������ ����� �� �ְ� �ϴ� ���̴�.
	m_CBuffer->UpdateCBuffer();

	CSceneComponent::RenderParticleEffectEditor();

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
	{
		m_Material->Render();
		m_Material->EnableDecal(m_ReceiveDecal);
	}

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
	if (!m_CBuffer)
		return;

	if (m_InitActiveDelayTime > 0.f)
		return;

	// ��� ���̴� �ܿ���, Render ���������� ��� ���� ������ ����� �� �ְ� �ϴ� ���̴�.
	m_CBuffer->UpdateCBuffer();

	CSceneComponent::Render();

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
	{
		m_Material->Render();
	}

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

	// Restart ��ư�� ���� �Ѵ�.
	// �ϴ� �ش� ��� ���� ������ ���� GPU ������ �ѹ� �Ѿ�� �ϴϱ�
	// ������ Render Ȥ�� PostUpdate ���� �Ѱ��ְ� ���⼭ ����
	if (m_CBuffer)
	{
		m_CBuffer->SetResetParticleSharedInfoSumSpawnCnt(0);
	}
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

// Shader ���� �Ѱ��� ����ȭ ���� ������ �ʱ�ȭ ���ִ� �ڵ��̴�.
void CParticleComponent::ResetParticleStructuredBufferInfo()
{
	if (!m_Particle)
		return;

	m_CBuffer->SetResetParticleSharedInfoSumSpawnCnt(1);
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
	// m_Mesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh(MeshName);
	m_Mesh = (CSpriteMesh*)CSceneManager::GetInst()->GetScene()->GetResource()->FindMesh(MeshName);

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
		// ���� ���ٸ�, Particle Path ���θ� ������ ã�´�.
		// �ش� Dir ��ο�, �ش� Name ���� �� ������ �����ϴ��� �Ǵ����ִ� �Լ� + ������ �� FullPath ��� ����
		std::string StrLoadParticleFileName = LoadedParticleName;
		if (StrLoadParticleFileName.find(".prtc") == std::string::npos)
			StrLoadParticleFileName.append(".prtc");

		auto ExtraLoadResult = CEngineUtil::CheckAndExtractFullPathOfTargetFile(PARTICLE_PATH, StrLoadParticleFileName);

		// �׷��� Load �� �ȵȴٸ�, �ƿ� Load �� prtc ���� ��ü�� �������� �ʴ´ٴ� �ǹ�
		if (!ExtraLoadResult.has_value())
			assert(false);
		
		Result = m_Particle->LoadFile(ExtraLoadResult.value().c_str());

		if (!Result)
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
