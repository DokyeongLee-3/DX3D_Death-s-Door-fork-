
#include "ShaderInfo.fx"

cbuffer	ParticleCBuffer : register(b11)
{
	uint	g_ParticleSpawnEnable;	// ��ƼŬ ��������
	float3	g_ParticleStartMin;		// ��ƼŬ�� ������ ������ Min
	float3	g_ParticleStartMax;		// ��ƼŬ�� ������ ������ Max
	uint	g_ParticleSpawnCountMax;	// ������ ��ƼŬ�� �ִ�
	
	float3	g_ParticleScaleMin;		// ������ ��ƼŬ ũ���� Min
	float	g_ParticleLifeTimeMin;	// ������ ��ƼŬ�� ������� �ּҽð�
	
	float3	g_ParticleScaleMax;		// ������ ��ƼŬ ũ���� Max
	float	g_ParticleLifeTimeMax;	// ������ ��ƼŬ�� ������� �ִ�ð�
	
	float4	g_ParticleColorMin;		// ������ ��ƼŬ�� ���� Min
	float4	g_ParticleColorMax;		// ������ ��ƼŬ�� ���� Max
	
	float	g_ParticleSpeedMin;		// ��ƼŬ�� �ּ� �̵��ӵ�
	float	g_ParticleSpeedMax;		// ��ƼŬ�� �ִ� �̵��ӵ�
	int		g_ParticleMove;			// �̵��� �ϴ��� ���ϴ���
	int		g_ParticleGravity;		// �߷� ������ �޴��� �ȹ޴���
	
	float3	g_ParticleMoveDir;		// �̵��� �Ѵٸ� ������ �� �̵� ����
	int		g_Particle2D;			// 2D�� ��ƼŬ����
	
	float3	g_ParticleMoveAngle;	// �̵��� �Ѵٸ� ������ �� �������κ��� x, y, z �� ����� ������ŭ Ʋ���� ������ ������ ���Ѵ�.
	int      g_ParticleBounce;

	float3 g_ParticleRotationAngle;
	float   g_ParticleBounceResist;

	int g_ParticleGenerateCircle;
	float g_ParcticleGenerateRadius;

	int g_LoopGenerateCircle;
	float g_ParticleEmpty11;
};

/*
if (g_ParticleBounce == 1)
			{
				if (g_ParticleArray[ThreadID.x].InitWorldPosY >= g_ParticleArray[ThreadID.x].WorldPos.y)
				{
					g_ParticleArray[ThreadID.x].FallTime = 0.f;
					// g_ParticleArray[ThreadID.x].Speed *= 0.98f;
					g_ParticleArray[ThreadID.x].Speed *= g_ParticleBounceResist;
				}
			}
*/

#define	GRAVITY	9.8f

struct ParticleInfo
{
	float3	WorldPos;
	float3	Dir;
	float	Speed;
	float	LifeTime;
	float	LifeTimeMax;
	int		Alive;
	float	FallTime;
	float	FallStartY;
	float  InitWorldPosY;
};

struct ParticleInfoShared
{
	uint	SpawnEnable;

	float3	ScaleMin;
	float3	ScaleMax;

	float4	ColorMin;
	float4	ColorMax;

	int		GravityEnable;

	float3 RotationAngle;
	float PrevCircleAngle;
};

RWStructuredBuffer<ParticleInfo>		g_ParticleArray	: register(u0);
RWStructuredBuffer<ParticleInfoShared> g_ParticleShare : register(u1);

Texture2DMS<float4> g_GBufferDepth : register(t10);


float3x3 ComputeRotationMatrix(float3 Angle)
{
	float3	ConvertAngle;
	ConvertAngle.x = DegreeToRadian(Angle.x);
	ConvertAngle.y = DegreeToRadian(Angle.y);
	ConvertAngle.z = DegreeToRadian(Angle.z);

	float3x3	matRotX, matRotY, matRotZ, matRot;

	// 11, 12, 13
	// 21, 22, 23
	// 31, 32, 33
	matRotX._11 = 1.f;
	matRotX._21 = 0.f;
	matRotX._31 = 0.f;

	matRotX._12 = 0.f;
	matRotX._22 = cos(ConvertAngle.x);
	matRotX._32 = -sin(ConvertAngle.x);

	matRotX._13 = 0.f;
	matRotX._23 = sin(ConvertAngle.x);
	matRotX._33 = cos(ConvertAngle.x);


	matRotY._11 = cos(ConvertAngle.y);
	matRotY._21 = 0.f;
	matRotY._31 = -sin(ConvertAngle.y);

	matRotY._12 = 0.f;
	matRotY._22 = 1.f;
	matRotY._32 = 0.f;

	matRotY._13 = sin(ConvertAngle.y);
	matRotY._23 = 0.f;
	matRotY._33 = cos(ConvertAngle.y);


	matRotZ._11 = cos(ConvertAngle.z);
	matRotZ._21 = -sin(ConvertAngle.z);
	matRotZ._31 = 0.f;

	matRotZ._12 = sin(ConvertAngle.z);
	matRotZ._22 = cos(ConvertAngle.z);
	matRotZ._32 = 0.f;

	matRotZ._13 = 0.f;
	matRotZ._23 = 0.f;
	matRotZ._33 = 1.f;

	matRot = mul(matRotX, matRotY);
	matRot = mul(matRot, matRotZ);


	return matRot;
}

[numthreads(64, 1, 1)]	// ������ �׷� ������ ���� �����Ѵ�.
void ParticleUpdate(uint3 ThreadID : SV_DispatchThreadID)
{
	g_ParticleShare[0].SpawnEnable = g_ParticleSpawnEnable;
	g_ParticleShare[0].ScaleMin = g_ParticleScaleMin;
	g_ParticleShare[0].ScaleMax = g_ParticleScaleMax;
	g_ParticleShare[0].ColorMin = g_ParticleColorMin;
	g_ParticleShare[0].ColorMax = g_ParticleColorMax;
	g_ParticleShare[0].GravityEnable = g_ParticleGravity;
	g_ParticleShare[0].RotationAngle = g_ParticleRotationAngle;


	// ���۵Ǵ� �������� ���� �����Ǵ� ��ƼŬ�� �ִ� �� ���� ũ�ų� ���ٸ�
	// �߸��� �ε����� �����ϱ� ������ ó���� �����ش�.
	if (g_ParticleSpawnCountMax <= ThreadID.x)
		return;

	// ��ƼŬ�� ����ִ� ��ƼŬ���� �Ǵ��Ѵ�.
	if (g_ParticleArray[ThreadID.x].Alive == 0)
	{
		int	SpawnEnable = g_ParticleShare[0].SpawnEnable;
		int	Exchange = 0;

		if (g_ParticleShare[0].SpawnEnable == 1)
		{
			int	InputValue = SpawnEnable - 1;

			// �Լ��� ���ڴ� in, in, out���� �����Ǿ� �ִ�.
			// in�� �Ϲ������� ������ ���� �Ѱ��ٶ� ����Ѵ�.
			// out�� ����¥�� ���۷���ó�� ������� ������ ������ �޾ƿö� ����Ѵ�.
			InterlockedExchange(g_ParticleShare[0].SpawnEnable, InputValue, Exchange);

			if (Exchange == SpawnEnable)
				g_ParticleArray[ThreadID.x].Alive = 1;
		}

		if (g_ParticleArray[ThreadID.x].Alive == 0)
			return;

		// ����� �ϴ� ��ƼŬ�̶�� ��ƼŬ ���������� ������.
		float	key = ThreadID.x / g_ParticleSpawnCountMax;
		float3	RandomPos = float3(Rand(key), Rand(2.142f), Rand(key * 3.f));
		float	Rand = (RandomPos.x + RandomPos.y + RandomPos.z) / 3.f;

		// Radius �ֺ����� �׷������� �ϱ� ���ؼ��� �Ʒ� ������� StartRange �� �ƴ϶�, ���� �߽����� �׷����� �ؾ� �Ѵ�.
		// �׸��� Loop �� �������ָ�, ���� �߽����� ������ �������� ���ʴ�� Spawn �ǵ��� ó���ؾ� �Ѵ�.
		float3	StartRange = g_ParticleStartMax - g_ParticleStartMin;
		g_ParticleArray[ThreadID.x].WorldPos = Rand * (g_ParticleStartMax - g_ParticleStartMin) + g_ParticleStartMin;
		g_ParticleArray[ThreadID.x].InitWorldPosY = g_ParticleArray[ThreadID.x].WorldPos.y;

		// �� ��� ���� => Generate Circle �� �ϰ� �Ǹ�, Start Min, Max �� ���ǹ������� �Ǵ� ���̴�.
		if (g_ParticleGenerateCircle == 1)
		{
			// 0.f, 0.f, 0.f �߽����� ���� �����ϱ�
			float RandomAngle = 360.f * Rand;
			float3 CirclePos = float3(0.f, 0.f, 0.f) + float3(cos(RandomAngle) * g_ParcticleGenerateRadius,
				0.f, 
				sin(RandomAngle) * g_ParcticleGenerateRadius);

			g_ParticleArray[ThreadID.x].WorldPos = CirclePos;

			// Loop �� �����ϰ� �Ǹ�, ���ʴ�� ��������� �Ѵ�.
			if (g_LoopGenerateCircle == 1)
			{
				float NextCircleAngle = g_ParticleShare[0].PrevCircleAngle + (360.f / g_ParticleSpawnCountMax);

				CirclePos = float3(0.f, 0.f, 0.f) + float3(cos(NextCircleAngle) * g_ParcticleGenerateRadius,  0.f, sin(NextCircleAngle) * g_ParcticleGenerateRadius);
				g_ParticleArray[ThreadID.x].WorldPos = CirclePos;

				g_ParticleShare[0].PrevCircleAngle = NextCircleAngle;
					// PrevCircleAngle
				// g_ParticleShare[0].RotationAngle = g_ParticleRotationAngle;
			}
		}

		g_ParticleArray[ThreadID.x].FallTime = 0.f;
		g_ParticleArray[ThreadID.x].FallStartY = g_ParticleArray[ThreadID.x].WorldPos.y;

		g_ParticleArray[ThreadID.x].LifeTime = 0.f;
		g_ParticleArray[ThreadID.x].LifeTimeMax = (g_ParticleLifeTimeMax - g_ParticleLifeTimeMin) + g_ParticleLifeTimeMin;

		if (g_ParticleMove == 1)
		{
			float3	ConvertAngle = (RandomPos.xyz * 2.f - 1.f) * g_ParticleMoveAngle;

			float3x3 matRot = ComputeRotationMatrix(ConvertAngle);

			// ���� Rot �����δ�, Particle Rotation ��ɸ� �������̴�.
			float3	Dir = normalize(mul(g_ParticleMoveDir, matRot));

			g_ParticleArray[ThreadID.x].Speed = Rand * (g_ParticleSpeedMax - g_ParticleSpeedMin) + g_ParticleSpeedMin;
			g_ParticleArray[ThreadID.x].Dir = Dir;
		}
	}
	// ���� ������ �Ǿ� �ִ� ��ƼŬ�� ���
	else
	{
		g_ParticleArray[ThreadID.x].LifeTime += g_DeltaTime;

		float3	MovePos = (float3)0.f;

		if (g_ParticleMove == 1)
		{
			MovePos = g_ParticleArray[ThreadID.x].Dir *
				g_ParticleArray[ThreadID.x].Speed * g_DeltaTime;

			/*g_ParticleArray[ThreadID.x].WorldPos += g_ParticleArray[ThreadID.x].Dir *
				g_ParticleArray[ThreadID.x].Speed * g_DeltaTime;*/
		}

		// �߷� ����
		if (g_ParticleShare[0].GravityEnable)
		{
			g_ParticleArray[ThreadID.x].FallTime += g_DeltaTime;

			float	Velocity = 0.f;
			
			if (g_ParticleArray[ThreadID.x].Dir.y > 0.f)
				Velocity = g_ParticleArray[ThreadID.x].Speed * g_ParticleArray[ThreadID.x].FallTime;

			g_ParticleArray[ThreadID.x].WorldPos.y = g_ParticleArray[ThreadID.x].FallStartY +
				(Velocity - 0.5f * GRAVITY * g_ParticleArray[ThreadID.x].FallTime * g_ParticleArray[ThreadID.x].FallTime * 10.f);

			// Bounce ȿ���� ���ٸ� 
			if (g_ParticleBounce == 1)
			{
				if (g_ParticleArray[ThreadID.x].InitWorldPosY >= g_ParticleArray[ThreadID.x].WorldPos.y)
				{
					g_ParticleArray[ThreadID.x].FallTime = 0.f;
					// g_ParticleArray[ThreadID.x].Speed *= 0.98f;
					g_ParticleArray[ThreadID.x].Speed *= g_ParticleBounceResist;
				}
			}

			g_ParticleArray[ThreadID.x].WorldPos.x += MovePos.x;
			g_ParticleArray[ThreadID.x].WorldPos.z += MovePos.z;
		}
		else
			g_ParticleArray[ThreadID.x].WorldPos += MovePos;

		if (g_ParticleArray[ThreadID.x].LifeTime >= g_ParticleArray[ThreadID.x].LifeTimeMax)
		{
			g_ParticleArray[ThreadID.x].Alive = 0;
		}
	}
}

struct VertexParticle
{
	float3	Pos	: POSITION;
	uint InstanceID : SV_InstanceID;	// �������ۿ� ���� �־��ִ°��� �ƴϴ�.
};

struct VertexParticleOutput
{
	uint	InstanceID : TEXCOORD;
};

StructuredBuffer<ParticleInfo>		g_ParticleArraySRV	: register(t30);
StructuredBuffer<ParticleInfoShared>	g_ParticleShareSRV	: register(t31);


VertexParticleOutput ParticleVS(VertexParticle input)
{
	VertexParticleOutput	output = (VertexParticleOutput)0;

	output.InstanceID = input.InstanceID;

	return output;
}

struct GeometryParticleOutput
{
	float4	Pos : SV_POSITION;
	float4	Color : COLOR;
	float2	UV	: TEXCOORD;
	float4	ProjPos : POSITION;
};

static float3	g_ParticleLocalPos[4] =
{
	{-0.5f, 0.5f, 0.f},
	{0.5f, 0.5f, 0.f},
	{-0.5f, -0.5f, 0.f},
	{0.5f, -0.5f, 0.f}
};

// in : ���� �Լ� ������ �Ѱ��ٶ�
// out : �Լ��ȿ��� ����� �Ѱܹ�����. �� �Լ� �ȿ��� �� ���� ����� ������ �� �� ����.
// inout : �׳� ���۷���.
[maxvertexcount(6)]
void ParticleGS(point VertexParticleOutput input[1],
	inout TriangleStream<GeometryParticleOutput> output)
{
	// ���� �簢������ Ȯ���Ѵ�.
	uint	InstanceID = input[0].InstanceID;

	if (g_ParticleArraySRV[InstanceID].Alive == 0)
		return;

	GeometryParticleOutput	OutputArray[4] =
	{
		(GeometryParticleOutput)0.f,
		(GeometryParticleOutput)0.f,
		(GeometryParticleOutput)0.f,
		(GeometryParticleOutput)0.f
	};

	OutputArray[0].UV = float2(0.f, 0.f);
	OutputArray[1].UV = float2(1.f, 0.f);
	OutputArray[2].UV = float2(0.f, 1.f);
	OutputArray[3].UV = float2(1.f, 1.f);

	float	Ratio = g_ParticleArraySRV[InstanceID].LifeTime / g_ParticleArraySRV[InstanceID].LifeTimeMax;

	float3	Scale = lerp(g_ParticleShareSRV[0].ScaleMin, g_ParticleShareSRV[0].ScaleMax,
		float3(Ratio, Ratio, Ratio));

	float4	Color = lerp(g_ParticleShareSRV[0].ColorMin, g_ParticleShareSRV[0].ColorMax,
		float4(Ratio, Ratio, Ratio, Ratio));

	float3x3 matRot = ComputeRotationMatrix(g_ParticleShareSRV[0].RotationAngle);

	// 4���� ���� ���������� ������ش�.
	for (int i = 0; i < 4; ++i)
	{
		float3	WorldPos = g_ParticleArraySRV[InstanceID].WorldPos + mul(g_ParticleLocalPos[i] * Scale ,matRot);

		OutputArray[i].ProjPos = mul(float4(WorldPos, 1.f), g_matVP);
		// OutputArray[i].ProjPos.xyz = mul(OutputArray[i].ProjPos.xyz, matRot);
		OutputArray[i].Pos = OutputArray[i].ProjPos;
		OutputArray[i].Color = Color;
	}

	output.Append(OutputArray[0]);
	output.Append(OutputArray[1]);
	output.Append(OutputArray[3]);
	output.RestartStrip();

	output.Append(OutputArray[0]);
	output.Append(OutputArray[3]);
	output.Append(OutputArray[2]);
	output.RestartStrip();
}

PSOutput_Single ParticlePS(GeometryParticleOutput input)
{
	PSOutput_Single output = (PSOutput_Single) 0;

	float4 Color = g_BaseTexture.Sample(g_BaseSmp, input.UV);

	if (Color.a == 0.f || input.Color.a == 0.f)
		clip(-1);
    
	float2 ScreenUV = input.ProjPos.xy / input.ProjPos.w;
	ScreenUV = ScreenUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
    
	int2 TargetPos = (int2) 0;
	
	TargetPos.x = (int) (ScreenUV.x * g_Resolution.x);
	TargetPos.y = (int) (ScreenUV.y * g_Resolution.y);
    
	// Soft Particle
	// float4 Depth = g_GBufferDepth.Load(TargetPos, 0);
    // 
	float Alpha = 1.f;
    // 
	// if (Depth.a > 0.f)
	// 	Alpha = (Depth.g - input.ProjPos.w) / 0.5f;
    // 
	// Alpha = clamp(Alpha, 0.f, 1.f);

	Color = PaperBurn2D(Color * input.Color, input.UV);
	
	output.Color = Color;
	output.Color.a = Color.a * g_MtrlOpacity * Alpha;

	return output;
}
