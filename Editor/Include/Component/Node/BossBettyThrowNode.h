#pragma once
#include "Component\Node\ActionNode.h"

// �� ���� ������ Attack Node
class CBossBettyThrowNode :
    public CActionNode
{
public:
    CBossBettyThrowNode();
    CBossBettyThrowNode(const CBossBettyThrowNode& Node);
    virtual ~CBossBettyThrowNode();

public:
    virtual void Init() override;

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

private:
    float m_RotatePerSec;
};

