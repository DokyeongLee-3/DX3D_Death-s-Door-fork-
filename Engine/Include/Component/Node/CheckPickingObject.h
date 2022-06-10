#pragma once

#include "ConditionNode.h"

// �÷��̾ ��ŷ������ ray�� (������ �ƴ϶�)Ư�� ������Ʈ(ex. �� è�Ǿ�, �̴Ͼ�)�� �浹ü�� �浹�ϴ���
class CCheckPickingObject :
    public CConditionNode
{
public:
    CCheckPickingObject();
    CCheckPickingObject(const CCheckPickingObject& Node);
    virtual ~CCheckPickingObject();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime);
};

