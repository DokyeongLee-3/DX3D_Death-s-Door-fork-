#pragma once

#include "Component/Node/ConditionNode.h"

// ��ų ���������(�׷��� �ٸ� ���·� ��ȯ�Ǹ� �ȵǴ� �������� üũ)
class CInSkillCheck :
    public CConditionNode
{
public:
    CInSkillCheck();
    CInSkillCheck(const CInSkillCheck& Node);
    virtual ~CInSkillCheck();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime);
};

