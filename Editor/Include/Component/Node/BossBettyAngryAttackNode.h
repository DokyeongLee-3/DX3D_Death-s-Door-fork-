#pragma once
#include "Component\Node\ActionNode.h"

// 1. HP�� 30%, 60%�� �Ǵ� ���� --> FirstSlamAnger + Roar / �ֺ� Particle ������ Damage
class CBossBettyAngrylAttackNode :
    public CActionNode
{
public:
    CBossBettyAngrylAttackNode();
    CBossBettyAngrylAttackNode(const CBossBettyAngrylAttackNode& Node);
    virtual ~CBossBettyAngrylAttackNode();
public:
    virtual void Init() override;

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);
private :
    // 1) BossBettyAngryType::HPDamageAngry
    // ���߿��� Snow Ball ���� �������� �Ѵ�.
    void OnBossBettyStartFallingSnowBallEffect();
};

