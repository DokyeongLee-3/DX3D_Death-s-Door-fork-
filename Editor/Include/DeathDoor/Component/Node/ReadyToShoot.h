#pragma once

#include "Component/Node/ActionNode.h"

class CReadyToShoot :
    public CActionNode
{
    friend class CGameBehaviorTree;

public:
    CReadyToShoot();
    CReadyToShoot(const CReadyToShoot& Node);
    virtual ~CReadyToShoot();

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

private:
    Vector3 m_CameraDestPos;
    Vector3 m_CameraMoveDir;
    Vector3 m_OriginCameraPos;
    bool m_CameraMoveEnd;
    class CPlayerDataComponent* m_PlayerDataComp;

public:
    const Vector3& GetOriginCameraPos()  const
    {
        return m_OriginCameraPos;
    }

public:
    virtual NodeResult Invoke(float DeltaTime) override;

};

