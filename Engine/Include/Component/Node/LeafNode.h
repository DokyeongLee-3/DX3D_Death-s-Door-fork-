#pragma once

#include "Node.h"

class CLeafNode :
    public CNode
{
public:
    CLeafNode();
    CLeafNode(const CLeafNode& Node);
    virtual ~CLeafNode() = 0;


protected:
    virtual NodeResult OnStart(float DeltaTime) = 0;
    virtual NodeResult OnUpdate(float DeltaTime) = 0;
    virtual NodeResult OnEnd(float DeltaTime) = 0;

public:
    virtual bool Invoke(float DeltaTime) override;

protected:
    // Start�� ���ʷ� �ѹ� �̹� ȣ�� �ߴ���
    bool m_CallStart;
};

