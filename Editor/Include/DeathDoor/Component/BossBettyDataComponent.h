#pragma once
#include "MonsterDataComponent.h"

enum class BossBettyFarAttackType
{
    Spin,
    JumpSmash
};

enum class BossBettyHPState
{
    None,
    Below60,
    Below30
};

class CBossBettyDataComponent :
    public CMonsterDataComponent
{
    friend class CGameObject;

private:
    CBossBettyDataComponent();
    CBossBettyDataComponent(const CBossBettyDataComponent& com);
    virtual ~CBossBettyDataComponent();

private :
    // �ٰŸ� ���� ���� Ƚ��
    int m_CloseSequentialAttackNum;

    // ���Ÿ� ���� Ƚ�� -> 3�� ����, Spin , Jump �� �����ư��鼭 ������ ���̴�.
    int m_FarAttackAttackNum;

    // 6�� ���Ÿ� ���� Gauge �� �� ����, Throw �� �� ���̴�.
    bool m_ThrowFarAttackEnable;

    // ���Ÿ� ���� Type
    BossBettyFarAttackType m_FarAttackType;

    // 60%, 30% �� �� ������, Angry Attack �߿��� ù��° Attack �����ϱ� 
    BossBettyHPState m_BettyHPState;

    // �ٰŸ� ���� ���� ���� (�簢��)
    Vector3 m_PunchLeftSquarePos[4];
    Vector3 m_PunchRightSquarePos[4];
    Vector3 m_SlashLeftSquarePos[4];
    Vector3 m_SlashRightSquarePos[4];
    Vector3 m_TwoSideFrontSquarePos[4];

    // ���� Point ���� 
    Vector3 m_RelativePunchRightPos;
    Vector3 m_RelativePunchLeftPos;
    Vector3 m_RelativeSlashRightPos;
    Vector3 m_RelativeSlashLeftPos;

    // Spin �ϴ� �߿� ���� �浹�ߴ��� ���θ� �߰������� ��� �����ϱ� ���ؼ�
    // ColliderBox3D �� �ϳ� �� �� ���̴�.
    class CColliderComponent* m_BossBettySpinCollider;

    // Boss Betty Throw Ball ����ü Object
    class CGameObject* m_BossBettyThrowObject;
    // Boss Betty Punch Down Particle
    // Boss Betty Slash Particle
    // Boss Betty Roar Particle

public:
    virtual void Start() override;
public :
    // (�Ʒ� �ݹ� �Լ�����, ���� Action Node �鿡�� �������� ����ϴ� ȿ��)
    // ���� ����ĥ��, ���ʿ� Attack ȿ���� ����
    void OnBossBettyGenerateTwoSideCloseAttackEffect();
    // �� ����ĥ�� �����ʿ� Attack ȿ���� ���� 
    void OnBossBettyGenerateRightCloseAttackEffect();
    // �� ����ĥ �� ���ʿ� Attack ȿ���� ���� 
    void OnBossBettyGenerateLeftCloseAttackEffect();
    // ���¢�� ȿ��
    void OnBossBettyRoarEffect();
    // ������ ����ü �浹�� �߻���ų Callback �Լ�
    void OnExplodeBettyThrowBallCallback();
    // Spin �߰��� ���� �ε�����, Spin Collide Animation ���� �ٲ��ִ� �Լ� 
    void OnChangeFromSpinToSpinCollideWhenCollide(const CollisionResult&);
    // Spin Collider ��Ȱ��ȭ
    void OnBossBettyDisableSpinCollider();
    // Spin Collider Ȱ��ȭ
    void OnBossBettyEnableSpinCollider();

    // Setter �Լ� ---
public:

    void SetBossBettyThrowObject(class CGameObject* Object)
    {
        m_BossBettyThrowObject = Object;
    }
    void ResetBettyHPState()
    {
        m_BettyHPState = BossBettyHPState::None;
    }
    void SetCloseAngryAttackEnable(bool Enable)
    {
        m_ThrowFarAttackEnable = Enable;
    }

    void IncCloseSequentialAttackNum()
    {
        ++m_CloseSequentialAttackNum;
    }
    void IncFarAttackNum()
    {
        ++m_FarAttackAttackNum;

        // 3�� ����, Far Attack Type �� �ٸ��� ���� ���̴�.
        if (m_FarAttackAttackNum % 3 == 0)
        {
            if (m_FarAttackType == BossBettyFarAttackType::Spin)
                m_FarAttackType = BossBettyFarAttackType::JumpSmash;
            else
                m_FarAttackType = BossBettyFarAttackType::Spin;
        }

        if (m_FarAttackAttackNum == 6)
        {
            m_ThrowFarAttackEnable = true;
            m_FarAttackAttackNum = 0;
        }
    }


    // Getter �Լ� ---
public:
    class CColliderComponent* GetBossBettySpinCollider() const
    {
        return m_BossBettySpinCollider;
    }
    class CGameObject* GetBossBettyThrowObject() const
    {
        return m_BossBettyThrowObject;
    }
    // �ٰŸ� ���� ��ġ 
    const Vector3& GetRelativePunchRightPos()
    {
        return m_RelativePunchRightPos;
    }
    const Vector3& GetRelativePunchLeftPos()
    {
        return m_RelativePunchLeftPos;
    }
    const Vector3& GetRelativeSlashRightPos()
    {
        return m_RelativeSlashRightPos;
    }
    const Vector3& GetRelativeSlashLeftPos()
    {
        return m_RelativeSlashLeftPos;
    }
    // �ٰŸ� ���� ���� �簢��
    const Vector3* GetPunchLeftSquarePoses()
    {
        return m_PunchLeftSquarePos;
    }
    const Vector3* GetPunchRightSquarePoses()
    {
        return m_PunchRightSquarePos;
    }
    const Vector3* GetSlashLeftSquarePoses()
    {
        return m_SlashLeftSquarePos;
    }
    const Vector3* GetSlashRightSquarePoses()
    {
        return m_SlashRightSquarePos;
    }
    const Vector3* GetTwoSideFrontSquarePoses()
    {
        return m_TwoSideFrontSquarePos;
    }
    bool IsThrowFarAttackEnable() const
    {
        return m_ThrowFarAttackEnable;
    }
    int GetCloseSequentialAttackNum() const
    {
        return m_CloseSequentialAttackNum;
    }
    BossBettyHPState GetBettyHPState() const
    {
        return m_BettyHPState;
    }
    BossBettyFarAttackType GetBettyFarAttackType() const
    {
        return m_FarAttackType;
    }
};

