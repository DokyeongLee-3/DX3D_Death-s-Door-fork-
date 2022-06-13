#pragma once
#include "ConditionNode.h"

class CCheckTurretAttackTarget :
    public CConditionNode
{
public:
    CCheckTurretAttackTarget();
    CCheckTurretAttackTarget(const CCheckTurretAttackTarget& Node);
    virtual ~CCheckTurretAttackTarget();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime);

private:
    std::list<CGameObject*> m_TargetChampionList; // ��ž ��Ÿ� ���� �����ϴ� ����(è�Ǿ�, �̴Ͼ� ����)
    std::list<CGameObject*> m_FriendChampionList; // ��ž ��Ÿ� ���� �����ϴ� �Ʊ� è�Ǿ�

private:
    // �켱���� 1 : �Ʊ� è�Ǿ��� �����ϴ� ���� è�Ǿ�
    // �켱���� 2 : ���� �̴Ͼ�
    // �켱���� 3 : ���� è�Ǿ�
    CGameObject* FindPriorAttackTarget();
    CGameObject* CheckPriorAttackCandidate();
};

