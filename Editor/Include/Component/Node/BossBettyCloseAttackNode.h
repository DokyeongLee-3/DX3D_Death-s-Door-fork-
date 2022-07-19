#pragma once
#include "Component\Node\ActionNode.h"

// �ٰŸ� ����
// 4���� ����
// Slash Left, Slash Right, Left, Right
// Slash �� ���, �ش� �������� 45�� ȸ�� (������)
class CBossBettyCloseAttackNode :
    public CActionNode
{
    public:
        CBossBettyCloseAttackNode();
        CBossBettyCloseAttackNode(const CBossBettyCloseAttackNode& Node);
        virtual ~CBossBettyCloseAttackNode();

    public:
        virtual void Init() override;

    protected:
        virtual NodeResult OnStart(float DeltaTime);
        virtual NodeResult OnUpdate(float DeltaTime);
        virtual NodeResult OnEnd(float DeltaTime);

    private:
        float m_RotatePerSec;
};

