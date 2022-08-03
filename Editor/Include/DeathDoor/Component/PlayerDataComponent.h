#pragma once

#include "ObjectDataComponent.h"

#include "GameObject/GameObject.h"

class CPlayerDataComponent :
    public CObjectDataComponent
{
	friend class CGameObject;

protected:
	CPlayerDataComponent();
	CPlayerDataComponent(const CPlayerDataComponent& com);
	virtual ~CPlayerDataComponent();

private:
	PlayerData m_PlayerData;
	bool m_OnSlash;
	class CAnimationMeshComponent* m_AnimComp;
	class CPlayerNormalAttackCheckCollider* m_AttackCheckCollider;
	class CColliderComponent* m_Body;
	Vector3 m_AttackDir;
	std::queue<char> m_KeyStateQueue;
	int m_FrameCount;
	bool m_MouseLButtonDown;
	bool m_Unbeatable; // ��������¿� ���������� ���� ��������
	float m_UnbeatableAccTime; // �������ϰ� �����ð� ������ ���� �ð� 
	// --> OnRollEnd�� m_Unbeatable = false�� ���ָ� m_Unbeatable = false�� ���� �ǰ� HitCheck�� �Ǿ
	// �������ε��� �¾� ���� ������ ���� �� �ִ�
	float m_UnbeatableTime; // �������ϰ� �����ð� 

	bool m_LadderUpEnable;
	bool m_LadderDownEnable;
	bool m_IsClimbingLadder;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CPlayerDataComponent* Clone();


public:
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;

public:
	void SetLadderUpEnable(bool Enable)
	{
		m_LadderUpEnable = Enable;
	}

	bool IsLadderUpEnable()	const
	{
		return m_LadderUpEnable;
	}

	void SetLadderDownEnable(bool Enable)
	{
		m_LadderDownEnable = Enable;
	}

	bool IsLadderDownEnable()	const
	{
		return m_LadderDownEnable;
	}

	void SetClimbingLadder(bool Enable)
	{
		m_IsClimbingLadder = Enable;
	}

	bool IsClimbingLadder()	const
	{
		return m_IsClimbingLadder;
	}

	void SetTrueOnSlash();

	void SetFalseOnSlash();

	void SetOnSlash(bool Slash)
	{
		m_OnSlash = Slash;
	}

	bool GetOnSlash()	const
	{
		return m_OnSlash;
	}

	void SetPlayerAbilityArrow(float DeltaTime)
	{
		m_PlayerData.Abilty_Type = Player_Ability::Arrow;

		//m_PlayerHook->Enable(false);

	}

	void SetPlayerAbilityFire(float DeltaTime)
	{
		m_PlayerData.Abilty_Type = Player_Ability::Fire;

		//m_PlayerHook->Enable(false);
	}

	void SetPlayerAbilityChain(float DeltaTime)
	{
		m_PlayerData.Abilty_Type = Player_Ability::Hook;
	}

	void SetPlayerAbilityBomb(float DeltaTime)
	{
		m_PlayerData.Abilty_Type = Player_Ability::Bomb;
	}

	void SetSkillNone(float DeltaTime)
	{
		m_PlayerData.Abilty_Type = Player_Ability::None;
	}

	Player_Ability GetPlayerAbility()	const
	{
		return m_PlayerData.Abilty_Type;
	}

	void SetAttackDir(const Vector3& Dir)
	{
		m_AttackDir = Dir;
	}

	bool IsUnbeatable()	const
	{
		return m_Unbeatable;
	}

	const Vector3& GetAttackDir()	const
	{
		return m_AttackDir;
	}

	void PushKeyState(const char State)
	{
		m_KeyStateQueue.push(State);
	}

	void PopKeyState()
	{
		m_KeyStateQueue.pop();
	}

	const char GetFrontKeyState()	const
	{
		return m_KeyStateQueue.front();
	}

	bool IsKeyStateQueueEmpty()	const
	{
		return m_KeyStateQueue.empty();
	}

	size_t GetInputQueueSize()	const
	{
		return m_KeyStateQueue.size();
	}

	class CAnimationMeshComponent* GetAnimationMeshComponent() const;

	void OnHitBack();
	void OnHitBackEnd();
	void OnHitRecoverEnd();
	void OnRoll();
	void OnRollEnd();
	void OnBombLift();

	void ForceUpdateAttackDirection();
	void OnEnableAttackCollider();
	void OnDisableAttackCollider();
};

