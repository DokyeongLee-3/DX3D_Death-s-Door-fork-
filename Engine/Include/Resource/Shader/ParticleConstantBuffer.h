#pragma once

#include "ConstantBufferBase.h"

class CParticleConstantBuffer :
    public CConstantBufferBase
{
public:
	CParticleConstantBuffer();
	CParticleConstantBuffer(const CParticleConstantBuffer& Buffer);
	virtual ~CParticleConstantBuffer();

protected:
	ParticleCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CParticleConstantBuffer* Clone();
public  :
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
public:
	const Vector3& GetStartMin()	const
	{
		return m_BufferData.StartMin;
	}

	const Vector3& GetStartMax()	const
	{
		return m_BufferData.StartMax;
	}
	// Seperate Rot Angle
	const Vector3& GetMinSeperateRotAngle() const
	{
		return m_BufferData.MinSeperateRotAngle;
	}
	const Vector3& GetMaxSeperateRotAngle() const
	{
		return m_BufferData.MaxSeperateRotAngle;
	}

	// UV Move
	int GetUVMoveEnable() const
	{
		return m_BufferData.UVMoveEnable;
	}
	int GetUVRowN() const
	{
		return m_BufferData.UVRowN;
	}
	int GetUVColN() const
	{
		return m_BufferData.UVColN;
	}
	int GetSpawnCount()	const
	{
		return m_BufferData.SpawnCountMax;
	}
	bool GetSpawnEnable()
	{
		return m_BufferData.SpawnEnable;
	}
	const Vector3& GetStartMin()
	{
		return m_BufferData.StartMin;
	}

	const Vector3& GetStartMax()
	{
		return m_BufferData.StartMax;
	}

	unsigned int GetSpawnCountMax()
	{
		return m_BufferData.SpawnCountMax;
	}

	const Vector3& GetScaleMin()
	{
		return m_BufferData.ScaleMin;
	}

	const Vector3& GetRotationAngle()
	{
		return m_BufferData.RotationAngle;
	}

	const Vector3& GetScaleMax()
	{
		return m_BufferData.ScaleMax;
	}

	float GetLifeTimeMin()
	{
		return m_BufferData.LifeTimeMin;
	}

	float GetLifeTimeMax()
	{
		return m_BufferData.LifeTimeMax;
	}

	const Vector4& GetColorMin()
	{
		return m_BufferData.ColorMin;
	}

	const Vector4& GetColorMax()
	{
		return m_BufferData.ColorMax;
	}

	float GetSpeedMin()
	{
		return m_BufferData.SpeedMin;
	}

	float GetSpeedMax()
	{
		return m_BufferData.SpeedMax;
	}

	bool GetMove()
	{
		return m_BufferData.Move == 1 ? true : false;
	}

	bool GetGravity()
	{
		return m_BufferData.Gravity == 1 ? true : false;
	}

	const Vector3& GetMoveDir()
	{
		return m_BufferData.MoveDir;
	}
	// �߽ɿ��� �־��� ���� LifeTime ����
	bool IsLifeTimeLinearFromCenter()
	{
		return m_BufferData.IsLifeTimeLinearFromCenter == 1 ? true : false;
	}
	bool Is2D()
	{
		return m_BufferData.Is2D == 1 ? true : false;
	}
	
	// ���� ���� �̵�
	bool IsMoveDirRandom()
	{
		return m_BufferData.IsMoveDirRandom == 1 ? true : false;
	}

	const Vector3& GetMoveAngle()
	{
		return m_BufferData.MoveAngle;
	}

	// Bounce
	int IsBounceEnable() const
	{
		return m_BufferData.ParticleBounce;
	}
	
	float GetParticleBounceResist() const
	{
		return m_BufferData.ParticleBounceResistance;
	}
	// Generate Radius
	float GetGenerateRadius() const
	{
		return m_BufferData.GenerateRadius;
	}
	// Ring
	int IsGenerateRing() const
	{
		return m_BufferData.IsGenerateRing;
	}
	int IsLoopGenerateRing() const
	{
		return m_BufferData.IsLoopGenerateRing;
	}
	// Circle
	int IsGenerateCircle() const
	{
		return m_BufferData.IsGenerateCircle;
	}

	// Torch
	int IsGenerateTorch() const
	{
		return m_BufferData.IsGenerateTorch;
	}

	// Alpha
	float GetStartAlpha() const
	{
		return m_BufferData.AlphaStart;
	}
	float GetEndAlpha() const
	{
		return m_BufferData.AlphaEnd;
	}
public:
	// Particle Component �� Relative Scale �ݿ��ϱ� 
	void SetCommonRelativeScale(const Vector3& Scale)
	{
		m_BufferData.CommonRelativeScale = Scale;
	}

	// UV Move
	void SetUVMoveEnable(bool Enable) 
	{
		m_BufferData.UVMoveEnable = Enable ? 1: 0;
	}
	void SetUVRowN(int Row) 
	{
		m_BufferData.UVRowN = Row;
	}
	void SetUVColN(int Col)
	{
		m_BufferData.UVColN = Col;
	}
	// Seperate Rot Angle Min, Max
	void SetMinSeperateRotAngle(const Vector3& Angle)
	{
		m_BufferData.MinSeperateRotAngle = Angle;
	}
	void SetMaxSeperateRotAngle(const Vector3& Angle)
	{
		m_BufferData.MaxSeperateRotAngle = Angle;
	}

	// Life Time Linear
	void SetLifeTimeLinearFromCenter(bool Enable)
	{
		m_BufferData.IsLifeTimeLinearFromCenter = Enable ? 1 : 0;
	}
	void SetRotationAngle(const Vector3& Angle)
	{
		m_BufferData.RotationAngle = Angle;
	}
	// Radius
	void SetGenerateRadius(float Radius)
	{
		m_BufferData.GenerateRadius = Radius;
	}
	// Ring
	void SetLoopGenerateRing(bool Enable)
	{
		m_BufferData.IsLoopGenerateRing = Enable ? 1 : 0;
	}
	void SetGenerateRingEnable(bool Enable)
	{
		m_BufferData.IsGenerateRing = Enable ? 1 : 0;
	}
	// Circle
	void SetGenerateCircleEnable(bool Enable)
	{
		m_BufferData.IsGenerateCircle = Enable ? 1 : 0;
	}
	// Torch
	void SetGenerateTorchEnable(bool Enable)
	{
		m_BufferData.IsGenerateTorch = Enable ? 1 : 0;
	}
	// Bounce
	void SetBounceEnable(int Bounce)
	{
		m_BufferData.ParticleBounce = Bounce;
	}
	void SetBounceResist(float Bounce)
	{
		m_BufferData.ParticleBounceResistance = Bounce;
	}
	// Alpha
	void SetStartAlpha(float Alpha)
	{
		m_BufferData.AlphaStart = Alpha;
	}
	void SetEndAlpha(float Alpha)
	{
		m_BufferData.AlphaEnd = Alpha;
	}

	void SetSpawnEnable(unsigned int Count)
	{
		m_BufferData.SpawnEnable = Count;
	}

	void SetStartMin(const Vector3& StartMin)
	{
		m_BufferData.StartMin = StartMin;
	}

	void SetStartMax(const Vector3& StartMax)
	{
		m_BufferData.StartMax = StartMax;
	}

	void SetSpawnCountMax(unsigned int Count)
	{
		m_BufferData.SpawnCountMax = Count;
	}

	void SetScaleMin(const Vector3& ScaleMin)
	{
		m_BufferData.ScaleMin = ScaleMin;
	}

	void SetScaleMax(const Vector3& ScaleMax)
	{
		m_BufferData.ScaleMax = ScaleMax;
	}

	void SetLifeTimeMin(float Min)
	{
		m_BufferData.LifeTimeMin = Min;
	}

	void SetLifeTimeMax(float Max)
	{
		m_BufferData.LifeTimeMax = Max;
	}

	void SetColorMin(const Vector4& ColorMin)
	{
		m_BufferData.ColorMin = ColorMin;
	}

	void SetColorMin(float r, float g, float b, float a)
	{
		m_BufferData.ColorMin = Vector4(r, g, b, a);
	}

	void SetColorMax(const Vector4& ColorMax)
	{
		m_BufferData.ColorMax = ColorMax;
	}

	void SetColorMax(float r, float g, float b, float a)
	{
		m_BufferData.ColorMax = Vector4(r, g, b, a);
	}

	void SetSpeedMin(float SpeedMin)
	{
		m_BufferData.SpeedMin = SpeedMin;
	}

	void SetSpeedMax(float SpeedMax)
	{
		m_BufferData.SpeedMax = SpeedMax;
	}

	void SetMove(bool Move)
	{
		m_BufferData.Move = Move ? 1 : 0;
	}

	void SetIsRandomMoveDir(bool Move)
	{
		m_BufferData.IsMoveDirRandom = Move ? 1 : 0;
	}

	void SetGravity(bool Gravity)
	{
		m_BufferData.Gravity = Gravity ? 1 : 0;
	}

	void SetMoveDir(const Vector3& MoveDir)
	{
		m_BufferData.MoveDir = MoveDir;
	}

	void Set2D(bool Is2D)
	{
		m_BufferData.Is2D = Is2D ? 1 : 0;
	}

	void SetMoveAngle(const Vector3& MoveAngle)
	{
		m_BufferData.MoveAngle = MoveAngle;
	}
};

