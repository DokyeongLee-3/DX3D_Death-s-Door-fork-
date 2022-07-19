#pragma once

#include "Component/Node/ConditionNode.h"

// GameObject�� AttackTarget�� nullptr�� �ƴϰ� ��Ÿ�� ������ŭ �������
class CCheckAttackTarget :
    public CConditionNode
{
    friend class CGameBehaviorTree;

public:
    CCheckAttackTarget();
    CCheckAttackTarget(const CCheckAttackTarget& Node);
    virtual ~CCheckAttackTarget();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime);
};

