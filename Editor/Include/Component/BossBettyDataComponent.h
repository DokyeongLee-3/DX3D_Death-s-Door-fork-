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
    std::vector<Vector3> m_vecCloseAttackPoses;

    // Boss Betty Throw Ball ����ü Object
public :
    const std::vector<Vector3>& GetVecCloseAttackPoses() const
    {
        return m_vecCloseAttackPoses;
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
public  :
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
};

