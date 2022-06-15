#pragma once

#include "Component/ObjectComponent.h"

class CMinionAIComponent :
    public CObjectComponent
{
    friend class CGameObject;

public:
    CMinionAIComponent();
    virtual ~CMinionAIComponent();
    CMinionAIComponent(const CMinionAIComponent& Com);

private:
    Minion_Type     m_MinionType;
    CGameObject*    m_Target;
    bool            m_OnFight; // ���ο��� ���� �̴Ͼ��̳� è�Ǿ�� �������̴� -> �̶��� ���� ��ǥ ������ ���� �ؼ������� ���� ������ ��� ���߰� �����Ѵ�

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CMinionAIComponent* Clone();


public:
    void SetMinionType(const Minion_Type Type)
    {
        m_MinionType = Type;
    }

    const Minion_Type& GetMinionType()  const
    {
        return m_MinionType;
    }

    void SetOnFight(bool Fight)
    {
        m_OnFight = Fight;
    }



public:
    virtual bool Save(FILE* File);
    virtual bool Load(FILE* File);
    virtual bool SaveOnly(FILE* File) override;
    virtual bool LoadOnly(FILE* File) override;

public:
    void OnAttack(const CollisionResult& Result);
    void OnTrack(const CollisionResult& Result);
};

