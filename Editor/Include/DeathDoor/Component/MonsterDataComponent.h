#pragma once

#include "ObjectDataComponent.h"

#define HIT_EFFECT_FIRST	0x01
#define HIT_EFFECT_SECOND	0x02
#define HIT_EFFECT_THIRD	0x04
#define HIT_EFFECT_FOURTH	0x08
#define HIT_EFFECT_FIFTH	0x10

class CMonsterDataComponent :
    public CObjectDataComponent
{
	friend class CGameObject;

protected:
	CMonsterDataComponent();
	CMonsterDataComponent(const CMonsterDataComponent& com);
	virtual ~CMonsterDataComponent();

public:
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual CMonsterDataComponent* Clone();

public:
	void LookPlayer(float DeltaTime);
	void RightLookPlayer(float DeltaTime);
	void LeftLookPlayer(float DeltaTime);
	void MoveZ(float DeltaTime);
	void ChangeColorBossDeath(float DeltaTime);	// ���� �� ���� ��ȭ�� �ʿ��� ������ ������ ��� �� �Լ��� Update���� ȣ��

public:
    virtual void OnActiveMeleeAttackCollider();
    virtual void OnInActiveMeleeAttackCollider();
    virtual void OnHitMeleeAttack(const CollisionResult& Result);
    void OnEndAnimPostAttackDelayOn();
	void OnEndAnimPostAttackDelayOff();
    void OnEnableLookPlayer();
	void OnDisableLookPlayer();
	void OnEnableRightLookPlayer();
	void OnDisableRightLookPlayer();
	void OnEnableLeftLookPlayer();
	void OnDisableLeftLookPlayer();
	void OnEnableMoveZ();
	void OnDisableMoveZ();
	void OnSetMoveSpeedZero(float DeltaTime)
	{
		m_CurMoveSpeed = 0.f;
	}
    virtual void OnDeadPaperBurnEnd();			// PaperBurn ����� ȣ��
    virtual void OnDeadAnimStart();				// Death Animation ���۽� ȣ��
    virtual void OnDeadAnimEnd();				// Death Animation ����� ȣ��
    virtual void OnPlayerEnterZone(const CollisionResult& Result);		// Boss Cutscene Trigger
	virtual void OnStartCutScene();
    virtual void OnEndCutScene();
	// Blood Particle
	virtual void OnActivateBloodParticle();
public:
	virtual void SetIsHit(bool Hit) override;

	void SetPostAttckDelaying(bool Enable)
	{
		m_PostAttackDelaying = Enable;
	}
	void OnCombatStart()
	{
		m_IsCombat = true;
	}
	void SetMoveZEnableMaxTime(float Time)
	{
		m_MoveZEnableMaxTime = Time;
	}
	void SetCurMoveSpeed(float Speed)
	{
		m_CurMoveSpeed = Speed;
	}	
	void SetCurRotSpeed(float Speed)
	{
		m_CurRotSpeed = Speed;
	}
	void SetAttackCoolDelayTimeMax(float Time)
	{
		m_AttackCoolDelayTimeMax = Time;
	}
	void SetAttackCoolDelayTimeEnable(bool Enable)
	{
		m_AttackCoolTimeEnable = Enable;

		if (Enable)
		{
			m_AttackCoolDelayTime = m_AttackCoolDelayTimeMax;
		}
	}
	void SetCurrentNodeNull();

	void SetPlayerEnterZone(bool Enter)
	{
		m_PlayerEnterZone = Enter;
	}

	void SetPlayerEnterZoneFalse()
	{
		m_PlayerEnterZone = false;
	}

public:
	bool IsAttackCoolTimeEnable() const
	{
		return m_AttackCoolTimeEnable;
	}
	float GetRotateSpeed() const
	{
		return m_Data.RotateSpeedPerSec;
	}
	float GetMeleeAttackRange() const
	{
		return m_Data.MeleeAttackRange;
	}
	float GetCurMoveSpeed() const
	{
		return m_CurMoveSpeed;
	}
	float GetJumpAttackRange() const
	{
		return m_Data.JumpAttackRange;
	}

	class CParticleComponent* GetBloodParticle() const
	{
		return m_BloodParticle;
	}

    class CColliderBox3D* GetMeleeAttackCollider() const
    {
        return m_MeleeAttackCollider;
    }

	CColliderBox3D* GetPlayerEnterZoneTrigger()	const
	{
		return m_PlayerEnterZoneTrigger;
	}

	class CCameraComponent* GetCutSceneCam() const
	{
		return m_CutSceneCam;
	}

	bool IsPostAttackDelaying() const
	{
		return m_PostAttackDelaying;
	}

	bool IsCombat() const
	{
		return m_IsCombat;
	}

	bool IsCutScenePlaying() const
	{
		return m_IsCutScenePlaying;
	}

	bool IsPlayerEnterZone() const
	{
		return m_PlayerEnterZone;
	}

	float GetAnglePlayer();
	bool IsPlayerLeftBasedInLookDir(); // �÷��̾ ���Ͱ� ���� ���� ���� ���ʿ� �ִ��� �Ǻ�
	bool IsPlayerInMeleeAttackRange();
	Vector3	ToPlayer();
	float DistToPlayer();
		
public:
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;

protected:
	void ActiveHitEffect(float DeltaTime);
	void ChangeHitColor(int EffectNum);

protected:
	class CMonsterNavAgent* m_MonsterNavAgent;
	class CAnimationMeshComponent* m_AnimMesh;
	class CColliderBox3D* m_HitBox;
	class CColliderBox3D* m_MeleeAttackCollider;
	class CGameStateComponent* m_State;
	class CParticleComponent* m_BloodParticle;

	bool m_PostAttackDelaying;  // ���� �ĵ����� ������ ����
	bool m_IsCombat;			// ���� ���� ����

	// Hit Effect ���� ����
	bool m_HitEffectStart;
	float m_HitEffectTimer;
	float m_HitEffectMax;
	int m_HitEffectFlag;

	size_t m_MeshMatSize;
	std::vector<Vector4> m_vecOriginDiffuse;
	std::vector<Vector4> m_vecOriginSpecluar;
	std::vector<Vector4> m_vecOriginAmbient;
	std::vector<Vector4> m_vecOriginEmissive;

	// Attack Cool Time
	bool m_AttackCoolTimeEnable;  // true ���, ���� X
	float m_AttackCoolDelayTime;
	float m_AttackCoolDelayTimeMax;

	// Rot 
	float m_OriginRotSpeed;
	float m_CurRotSpeed;

	// Move ���� ����
	float m_CurMoveSpeed;
	bool m_LookPlayer;
	bool m_MoveZ;
	// �ش� �ð� ���ȸ� m_MoveZ �� true �� ������ٰ�, false �� �ٽ� ����
	float m_MoveZEnableMaxTime;
	// �������θ� ������ ���ƾ� �ϴ� ����
	bool m_LeftLookPlayer;
	// ���������θ� ������ ���ƾ� �ϴ� ����
	bool m_RightLookPlayer;

	// Death Effect ���� ����
    class CPaperBurnComponent* m_PaperBurn;
    bool m_DeathColorChangeStart;
    float m_DeathTimer;
    float m_DeathColorChangeTimeMax;
	Vector4 m_DeathColorStart;
	Vector4	m_DeathColorEnd;

	// CutScene���� ����
    bool m_PlayerEnterZone;
    class CColliderBox3D* m_PlayerEnterZoneTrigger;

    bool m_IsCutScenePlaying;
    class CCameraComponent* m_CutSceneCam;
    Vector3 m_CutSceneBeforePlayerPos;

public:
	class CMonsterNavAgent* GetMonsterNavAgent()	const;

public:
	class CColliderBox3D* GetHitBox()	const;
};

