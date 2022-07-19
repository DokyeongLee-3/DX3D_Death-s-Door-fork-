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
    // ���� 2�� �ٰŸ� ���� �ÿ�, Close Attack ����, AngryAttack Node �߿��� 2��° (FirstSlam + Roar)
    bool m_CloseAngryAttackEnable;
    // ���Ÿ� ���� Ƚ�� -> 3�� ����, Spin , Jump �� �����ư��鼭 ������ ���̴�.
    int m_FarAttackAttackNum;
    // 6�� ���Ÿ� ���� Gauge �� �� ����, Throw �� �� ���̴�.
    bool m_ThrowFarAttackEnable;
    // ���Ÿ� ���� Type
    BossBettyFarAttackType m_FarAttackType;
    // 60%, 30% �� �� ������, Angry Attack �߿��� ù��° Attack �����ϱ� 
    BossBettyHPState m_BettyHPState;
public :
    bool IsCloseAngryAttackEnable() const
    {
        return m_CloseAngryAttackEnable;
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
public  :
    void SetCloseAngryAttackEnable(bool Enable)
    {
        m_ThrowFarAttackEnable = Enable;
    }
    void SetFarThrowAttackEnable(bool Enable)
    {
        m_CloseAngryAttackEnable = Enable;
    }
    void IncCloseSequentialAttackNum() 
    {
        ++m_CloseSequentialAttackNum;

        if (m_CloseSequentialAttackNum == 2)
        {
            m_CloseSequentialAttackNum = 0;
            m_CloseAngryAttackEnable = true;
        }
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
public:
    virtual void Start() override;
};

