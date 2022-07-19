#pragma once
#include "Component\Node\ActionNode.h"

// 2���� ���
// 1. HP�� 30%, 60%�� �Ǵ� ���� --> FirstSlamAnger + Roar / �ֺ� Particle ������ Damage
// 2. �ٰŸ� ���� ���� 2�� ������ -> First Slam + Roar
class CBossBettySpecialAttackNode :
    public CActionNode
{
public:
    CBossBettySpecialAttackNode();
    CBossBettySpecialAttackNode(const CBossBettySpecialAttackNode& Node);
    virtual ~CBossBettySpecialAttackNode();

public:
    virtual void Init() override;

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

private:
    float m_RotatePerSec;
};

