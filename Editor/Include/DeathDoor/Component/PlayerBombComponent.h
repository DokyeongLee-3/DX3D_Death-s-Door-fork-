#pragma once

#include "ProjectileComponent.h"

class CPlayerBombComponent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CPlayerBombComponent();
	CPlayerBombComponent(const CPlayerBombComponent& com);
	virtual ~CPlayerBombComponent();

private:
	class CPlayerDataComponent* m_PlayerData;
	class CLightComponent* m_Light;
	std::queue<Vector3> m_LiftBombPathQueue;
	class CGameObject* m_Bomb;
	Vector3 m_LiftDir;
	float m_LiftSpeed;
	float m_ShootSpeed;
	float m_CollisionLifeTime;// �浹�ϰ� ũ�Ⱑ Ŀ���ٰ� �� �Ŀ� ����������
	float m_AccCollisionLifeTime;
	bool m_Collision;

public:
	void SetLiftSpeed(float Speed)
	{
		m_LiftSpeed = Speed;
	}

	void SetShootSpeed(float Speed)
	{
		m_ShootSpeed = Speed;
	}

	const Vector3& GetBombPos()		const;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual void Reset();
	virtual CPlayerBombComponent* Clone();

public:
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;

public:
	void LiftBomb();
	void ShootBomb(const Vector3& ShootDir);
	void HideBomb();
	
public:
	void OnCollision(const CollisionResult& Result);
};

