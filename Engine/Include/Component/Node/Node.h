#pragma once

#include "../../GameInfo.h"
#include "../../GameObject/GameObject.h"


class CNode
{
	friend class CBehaviorTree;

protected:
	CNode();
	CNode(const CNode& Node);
	virtual ~CNode() = 0;

protected:
	CNode* m_Parent;
	CGameObject* m_Object;
	std::string m_Name;
	size_t		m_TypeID;
	bool		m_IsEnd;
	class CBehaviorTree* m_Owner;
	// Start�� ���ʷ� �ѹ� �̹� ȣ�� �ߴ���
	bool m_CallStart;

public:
	bool IsCallStart()
	{
		return m_CallStart;
	}

	void SetCallStart(bool CallStart)
	{
		m_CallStart = CallStart;
	}

	void SetOwner(class CBehaviorTree* Owner);

	class CBehaviorTree* GetOwner();

	void SetEnd(bool End)
	{
		m_IsEnd = End;
	}

	bool IsEnd()	const
	{
		return m_IsEnd;
	}

	void SetTypeID(size_t ID)
	{
		m_TypeID = ID;
	}

	size_t GetTypeID()	const
	{
		return m_TypeID;
	}

	void SetObject(CGameObject* Object)
	{
		m_Object = Object;
	}

	CGameObject* GetGameObject()	const
	{
		return m_Object;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	std::string GetName()	const
	{
		return m_Name;
	}

	CNode* GetParent()	const
	{
		return m_Parent;
	}

	void SetParent(CNode* Parent)
	{
		m_Parent = Parent;
	}

public:
	virtual bool Invoke(float DeltaTime) = 0;

public:
	virtual NodeResult OnStart(float DeltaTime) = 0;
	virtual NodeResult OnUpdate(float DeltaTime) = 0;
	virtual NodeResult OnEnd(float DeltaTime) = 0;

};
