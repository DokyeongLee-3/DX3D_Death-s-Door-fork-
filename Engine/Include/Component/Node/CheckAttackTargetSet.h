#pragma once
#include "ConditionNode.h"

// GameObject�� AttackTarget�� nullptr�� �ƴϰ�, AttackTarget���� �Ÿ��� ��Ÿ�� ������ŭ �������
class CCheckAttackTargetSet :
    public CConditionNode
{
public:
    CCheckAttackTargetSet();
    CCheckAttackTargetSet(const CCheckAttackTargetSet& Node);
    virtual ~CCheckAttackTargetSet();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime);
};

