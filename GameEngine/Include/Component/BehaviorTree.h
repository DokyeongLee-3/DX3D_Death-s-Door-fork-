#pragma once

#include "../GameInfo.h"


class CBehaviorTree
{
    friend class CStateComponent;

public:
    CBehaviorTree();
    CBehaviorTree(const CBehaviorTree& Tree);
    ~CBehaviorTree();

private:
    class CNode* m_Root;
    class CStateComponent* m_Owner;
    std::list<class CNode*> m_NodeList;
    class CNode* m_CurrentNode; // �̹� �����ӿ��� Root�� �ƴ� CurrentNode���� ��ȸ�ؾ��Ҷ�


    class CAnimationMeshComponent* m_AnimationMeshComp;
    
public:
    class CNode* GetCurrentNode()   const;
    void SetCurrentNode(class CNode* CurrentNode);


public:
    void SetAnimationMeshComponent(class CAnimationMeshComponent* Mesh);
    class CAnimationMeshComponent* GetAnimationMeshComp()   const;

    class CNode* GetRootNode()    const;
    void SetRoot(class CNode* Node);
    CNode* FindNode(const std::string& Name);
    void AddNode(class CNode* Node);


public:
    bool Init();
    void Start();
    void Update(float DeltaTime);
    void PostUpdate(float DeltaTime);
    void PrevRender();
    void Render();
    void PostRender();
    bool Save(FILE* File);
    bool Load(FILE* File);
    CBehaviorTree* Clone();

private:
    std::function<CNode* (CNode*, size_t)>   m_NodeCreateCallback;

public:
    CNode* LoadNode(CNode* Parent, size_t TypeID);

    template <typename T>
    void SetCreateNodeCallback(T* Obj, CNode*(T::* Func)(CNode* Parent, size_t TypeID))
    {
        m_NodeCreateCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }



public:
    

   /* template <typename T>
    T* CreateNode(const std::string& Name)
    {
        T* Node = new T;

        Node->SetName(Name);

        if (m_Owner)
            Node->SetObject(m_Owner->GetGameObject());

        Node->SetOwner(this);

        if (!m_Root)
            m_Root = Node;

        m_NodeList.push_back(Node);

        return Node;
    }*/
};

