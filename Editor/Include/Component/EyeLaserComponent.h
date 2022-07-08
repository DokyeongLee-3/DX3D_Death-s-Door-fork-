#pragma once

#include "Component/SceneComponent.h"

class CEyeLaserComponent :
    public CSceneComponent
{
	friend class CGameObject;

protected:
	CEyeLaserComponent();
	CEyeLaserComponent(const CEyeLaserComponent& com);
	virtual ~CEyeLaserComponent();

private:
	// TriggerBox�� �÷��̾ Ÿ���� Ƚ�� -> 1���� �ϸ� EyeLaser���, 4�� ��� �νø� EyeLaser�� �ı�
	int m_TriggerHitCount;
	class CGameObject* m_Player;
	class CAnimationMeshComponent* m_AnimComp;
	bool	m_WakeEnd;
	Vector3 m_CurrentLaserDir;
	Matrix m_LaserRotMatrix;

	//CSharedPtr<CShader>	m_Shader;
	CSharedPtr<CMesh> m_LaserPlaneMesh;
	CSharedPtr<CMaterial>	m_Material;

public:
	CMesh* GetMesh()	const
	{
		return m_LaserPlaneMesh;
	}

	CMaterial* GetMaterial()	const
	{
		return m_Material;
	}

	void SetWakeEnd();

	void AddTriggerCount()
	{
		++m_TriggerHitCount;
	}

	int GetTriggerCount() const
	{
		return m_TriggerHitCount;
	}

	bool IsTransparent()	const
	{
		return m_Material->IsTransparent();
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CEyeLaserComponent* Clone();


public:
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;

private:
	void TrackPlayer(float DeltaTime);
	void ChangeToWakeAnimation();
	void ChangeToDieAnimation();
	void ChangeToIdleAnimation();

public:
	void SetBaseColor(const Vector4& Color, int Index = 0);
	void SetBaseColor(float r, float g, float b, float a, int Index = 0);
	void SetAmbientColor(const Vector4& Color, int Index = 0);
	void SetAmbientColor(float r, float g, float b, float a, int Index = 0);
	void SetSpecularColor(const Vector4& Color, int Index = 0);
	void SetSpecularColor(float r, float g, float b, float a, int Index = 0);
	void SetEmissiveColor(const Vector4& Color, int Index = 0);
	void SetEmissiveColor(float r, float g, float b, float a, int Index = 0);
	void SetSpecularPower(float Power, int Index = 0);
};

