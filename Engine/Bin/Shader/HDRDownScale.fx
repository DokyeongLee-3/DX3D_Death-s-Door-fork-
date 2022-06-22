#include "ShaderInfo.fx"

cbuffer DownScaleCBuffer : register(b10)
{
	uint2 g_DownScaleRS;
	uint g_DownScaleDomain;
	uint g_DownScaleGroupSize;
}

groupshared float SharedPositions[1024];

Texture2DMS<float4> g_HDRTex : register(t120);
StructuredBuffer<float> AverageValues1D : register(t21);
RWStructuredBuffer<float> AverageLum : register(u0);

float DownScale4x4(uint2 CurPixel, uint GroupThreadID)
{
	float AvgLum = 0.f;

	if (CurPixel.y < g_DownScaleRS.y)
	{
		uint2 FullResPos = uint2(CurPixel * 4);
		float4 DownScaled = float4(0.f, 0.f, 0.f, 0.f);

		[unroll]
		for (int i = 0; i < 4; ++i)
		{
			[unroll]
			for (int j = 0; j < 4; ++j)
			{
				DownScaled += g_HDRTex.Load(int2(FullResPos.x + j, FullResPos.y + i), 0);
			}
		}
		DownScaled /= 16.f;
		AvgLum = dot(DownScaled, LUM_FACTOR);
		SharedPositions[GroupThreadID] = AvgLum;
	}

	GroupMemoryBarrierWithGroupSync();

	return AvgLum;
}

float DownScale1024to4(uint DispatchThreadID, uint GroupThreadID, float AvgLum)
{
	[unroll]
	for (uint GroupSize = 4, Step1 = 1, Step2 = 2, Step3 = 3; GroupSize < 1024; GroupSize *= 4, Step1 *= 4, Step2 *= 4, Step3 *= 4)
	{
		if (GroupThreadID % GroupSize == 0)
		{
			// �ֵ� �� �ջ�
			float StepAvgLum = AvgLum;
			StepAvgLum += DispatchThreadID + Step1 < g_DownScaleDomain ? SharedPositions[GroupThreadID + Step1] : AvgLum;
			StepAvgLum += DispatchThreadID + Step2 < g_DownScaleDomain ? SharedPositions[GroupThreadID + Step2] : AvgLum;
			StepAvgLum += DispatchThreadID + Step3 < g_DownScaleDomain ? SharedPositions[GroupThreadID + Step3] : AvgLum;

			// ��� �� ����
			AvgLum = StepAvgLum;
			SharedPositions[GroupThreadID] = StepAvgLum;
		}

		GroupMemoryBarrierWithGroupSync();
	}

	return AvgLum;
}

void DownScale4To1(uint DispatchThreadID, uint GroupThreadID, uint GroupID, float AvgLum)
{
	if (GroupThreadID == 0)
	{
		// ������ �׷쿡 ���� ��� ���
		float FinalAvgLum = AvgLum;
		FinalAvgLum += DispatchThreadID + 256 < g_DownScaleDomain ? SharedPositions[GroupThreadID + 256] : AvgLum;
		FinalAvgLum += DispatchThreadID + 512 < g_DownScaleDomain ? SharedPositions[GroupThreadID + 512] : AvgLum;
		FinalAvgLum += DispatchThreadID + 768 < g_DownScaleDomain ? SharedPositions[GroupThreadID + 768] : AvgLum;
		FinalAvgLum /= 1024.0;

		AverageLum[GroupID] = FinalAvgLum;
	}
}

[numthreads(1024, 1, 1)]
void DownScaleFirstPass(uint3 GroupID : SV_GroupID, uint3 DispatchThreadID : SV_DispatchThreadID,
	uint GroupThreadID : SV_GroupThreadID)
{
	uint2 CurPixel = uint2(DispatchThreadID.x % g_DownScaleRS.x, DispatchThreadID.x / g_DownScaleRS.x);

	// 16�ȼ� �׷��� �ϳ��� �ȼ��� �ٿ� ���� �޸𸮿� ����
	float AvgLum = DownScale4x4(CurPixel, GroupThreadID.x);

	// 1024���� 4�� �ٿ����
	AvgLum = DownScale1024to4(DispatchThreadID.x, GroupThreadID.x, AvgLum);

	// 4���� 1�� �ٿ����
	DownScale4To1(DispatchThreadID.x, GroupThreadID.x, GroupID.x, AvgLum);
}

#define MAX_GROUPS 64

// ���� �޸� �׷쿡 �߰� �� ����
groupshared float SharedAvgFinal[MAX_GROUPS];

[numthreads(MAX_GROUPS, 1, 1)]
void DownScaleSecondPass(uint3 GroupID : SV_GroupID, uint3 GroupThreadID : SV_GroupThreadID, uint DispatchThreadID : SV_DispatchThreadID)
{
	// ���� �޸𸮿� 1D �� ����
	float AvgLum = 0.f;
	if (DispatchThreadID < g_DownScaleGroupSize)
	{
		AvgLum = AverageValues1D[DispatchThreadID.x];
	}
	SharedAvgFinal[DispatchThreadID.x] = AvgLum;

	GroupMemoryBarrierWithGroupSync();

	// 64���� 16���� �ٿ����
	if (DispatchThreadID.x % 4 == 0)
	{
		float StepAvgLum = AvgLum;
		StepAvgLum += DispatchThreadID.x + 1 < g_DownScaleGroupSize ? SharedAvgFinal[DispatchThreadID.x + 1] : AvgLum;
		StepAvgLum += DispatchThreadID.x + 2 < g_DownScaleGroupSize ? SharedAvgFinal[DispatchThreadID.x + 2] : AvgLum;
		StepAvgLum += DispatchThreadID.x + 3 < g_DownScaleGroupSize ? SharedAvgFinal[DispatchThreadID.x + 3] : AvgLum;

		AvgLum = StepAvgLum;
		SharedAvgFinal[DispatchThreadID.x] = StepAvgLum;
	}

	GroupMemoryBarrierWithGroupSync();

	// 16���� 4�� �ٿ����
	if (DispatchThreadID.x % 16 == 0)
	{
		float StepAvgLum = AvgLum;
		StepAvgLum += DispatchThreadID.x + 4 < g_DownScaleGroupSize ? SharedAvgFinal[DispatchThreadID.x + 4] : AvgLum;
		StepAvgLum += DispatchThreadID.x + 8 < g_DownScaleGroupSize ? SharedAvgFinal[DispatchThreadID.x + 8] : AvgLum;
		StepAvgLum += DispatchThreadID.x + 12 < g_DownScaleGroupSize ? SharedAvgFinal[DispatchThreadID.x + 12] : AvgLum;

		AvgLum = StepAvgLum;
		SharedAvgFinal[DispatchThreadID.x] = StepAvgLum;
	}

	GroupMemoryBarrierWithGroupSync();

	// 4���� 1�� �ٿ����
	if (DispatchThreadID.x == 0)
	{
		float FinalLumValue = AvgLum;
		FinalLumValue += DispatchThreadID.x + 16 < g_DownScaleGroupSize ? SharedAvgFinal[DispatchThreadID.x + 16] : AvgLum;
		FinalLumValue += DispatchThreadID.x + 32 < g_DownScaleGroupSize ? SharedAvgFinal[DispatchThreadID.x + 32] : AvgLum;
		FinalLumValue += DispatchThreadID.x + 48 < g_DownScaleGroupSize ? SharedAvgFinal[DispatchThreadID.x + 48] : AvgLum;
		FinalLumValue /= 64.f;

		AverageLum[0] = FinalLumValue;
	}
}
