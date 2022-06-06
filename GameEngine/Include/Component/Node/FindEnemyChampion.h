#pragma once

#include "ConditionNode.h"

class CFindEnemyChampion :
    public CConditionNode
{
public:
    CFindEnemyChampion();
    CFindEnemyChampion(const CFindEnemyChampion& Node);
    virtual ~CFindEnemyChampion();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime);
};

