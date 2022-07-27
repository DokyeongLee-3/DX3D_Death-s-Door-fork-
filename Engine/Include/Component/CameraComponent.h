#pragma once

#include "SceneComponent.h"

// ī�޶� ���� �ݹ��� ���� �Ҹ� ������
enum class CamMoveCallBackCallType
{
	START_MOVE,						// �ش� ����Ʈ�� �̵� �����ϴ� Ÿ�̹�
	REACHED_POINT,					// �ش� ����Ʈ�� �������� Ÿ�̹�
	Max
};

struct CamMoveData
{
	Vector3 DestPoint;
	float	NextPointReachTime;
	float	StayTime;
	std::function<void()> CallBack[(int)CamMoveCallBackCallType::Max];
};

class CCameraComponent :
	public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CCameraComponent();
	CCameraComponent(const CCameraComponent& com);
	virtual ~CCameraComponent();

protected:
	Camera_Type m_CameraType;
	Matrix      m_matView;
	Matrix      m_matProj;
	Matrix		m_matKeepProj;
	Matrix      m_matShadowView;
	Matrix      m_matShadowProj;
	float		m_ViewAngle;
	float		m_Distance;
	Resolution	m_RS;
	Vector2		m_Ratio;
	class CFrustum* m_Frustum;

	Vector3		m_OriginRelavitePos;
	bool		m_Shake;
	float		m_ShakeTimer;
	float		m_ShakeMaxTime;
	float		m_ShakeAmount;
	float		m_ShakeDecreaseTick;

	bool		m_Init;
	Vector3		m_OriginOffset;
	Vector3		m_Offet;

	bool		m_StartMove;
	Vector3		m_CurMoveDir;
	float		m_CurMoveTick;
	bool		m_StayStart;
	float		m_StayTimer;
	CamMoveData* m_CurMoveData;
	std::list<CamMoveData*>	m_MoveDataList;
	std::function<void()>	m_MoveEndCallBack;

#ifdef _DEBUG
	bool m_TestMove;
	Vector3 m_TestOriginPos;
	std::list<CamMoveData*>	m_TestMoveDataList;
#endif // _DEBUG

public:
	void Shake(float Time, float Amount);

public:
	Resolution GetResolution()	const
	{
		return m_RS;
	}

	Camera_Type GetCameraType()	const
	{
		return m_CameraType;
	}

	Matrix GetViewMatrix()	const
	{
		return m_matView;
	}

	Matrix GetProjMatrix()	const
	{
		return m_matProj;
	}

	Matrix GetShadowViewMatrix()	const
	{
		return m_matShadowView;
	}

	Matrix GetShadowProjMatrix()	const
	{
		return m_matShadowProj;
	}

	Vector2 GetLeftBottom()	const
	{
		Vector2	LB;

		LB.x = GetWorldPos().x;// - m_Ratio.x * m_RS.Width;
		LB.y = GetWorldPos().y;// - m_Ratio.y * m_RS.Height;

		return LB;
	}

	const std::list<CamMoveData*>& GetMoveData() const
	{
		return m_MoveDataList;
	}

	CamMoveData* FindMoveData(int Index);

	int GetMoveDataCount() const
	{
		return (int)m_MoveDataList.size();
	}

	float GetDistance() const
	{
		return m_Distance;
	}

	float GetViewAngle() const
	{
		return m_ViewAngle;
	}

public:
	void SetViewAngle(float Angle)
	{
		m_ViewAngle = Angle;

		if (m_CameraType == Camera_Type::Camera3D)
			CreateProjectionMatrix();
	}

	void SetDistance(float Distance)
	{
		m_Distance = Distance;

		CreateProjectionMatrix();
	}

	void SetCameraType(Camera_Type Type)
	{
		m_CameraType = Type;

		CreateProjectionMatrix();
	}

	// 2D ����
	void OnViewportCenter()
	{
		if (m_CameraType == Camera_Type::Camera2D)
		{
			float	z = GetRelativePos().z;
			SetRelativePos(m_RS.Width / -2.f, m_RS.Height / -2.f, z);

			m_Ratio.x = 0.5f;
			m_Ratio.y = 0.5f;
		}
	}

	// 2D ����
	void SetViewportRatio(float x, float y)
	{
		if (m_CameraType == Camera_Type::Camera2D)
		{
			float	z = GetRelativePos().z;
			SetRelativePos(m_RS.Width * -x, m_RS.Height * -y, z);

			m_Ratio.x = x;
			m_Ratio.y = y;
		}
	}

public:
	bool FrustumInPoint(const Vector3& Point);
	bool FrustumInSphere(const SphereInfo& Sphere);

public :
	// ���ϴ� Width, Height�� ���缭 Projection Matrix �� ������ �Լ�
	void CreateCustomResolutionProjMatrix(float Width, float Height);

public:
	void ComputeShadowView();

public:
	void StartMove();
	void Move(float DeltaTime);
	void ClearMoveData();
	CamMoveData* AddMoveData(const Vector3& Point, float NextPointReachTime, float StayTime = 0.f);
	bool DeleteMoveData(int Index);
	bool DeleteMoveData(CamMoveData* Data);
	void ChangeMoveData(int Index, const Vector3& Point, float NextPointReachTime, float StayTime = 0.f);
	void ChangeMoveDestPoint(int Index, const Vector3& Point);
	void ChangeMoveDestReachTime(int Index, float Time);
	void ChangeMoveStayTime(int Index, float Time);

#ifdef _DEBUG
	void StartTestMove();
	void TestMove(float DeltaTime);
	void UpdateCurTestMoveData();
#endif // _DEBUG

private:
	void CreateProjectionMatrix();
	bool IsValidMoveDataIndex(int Index);
	void UpdateCurMoveData();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CCameraComponent* Clone();
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);

public:
	template<typename T>
	void AddMoveEndCallBack(T* Obj, void(T::* Func)())
	{
		m_MoveEndCallBack = std::bind(Func, Obj);
	}

	template <typename T>
	void AddMoveCallBack(CamMoveData* Data, CamMoveCallBackCallType CallBackType, T* Obj, void(T::* Func)())
	{
		if (!Data)
		{
			return;
		}

		std::function<void()> CallBack = std::bind(Func, Obj);
		Data->CallBack[(int)CallBackType] = CallBack;
	}

	template <typename T>
	void AddMoveCallBack(int Index, CamMoveCallBackCallType CallBackType, T* Obj, void(T::* Func)())
	{
		if (!IsValidMoveDataIndex(Index))
		{
			return;
		}

		CamMoveData* FoundData = FindMoveData(Index);

		if (!FoundData)
		{
			return;
		}

		std::function<void()> CallBack = std::bind(Func, Obj);
		FoundData->CallBack[(int)CallBackType] = CallBack;
	}
};

