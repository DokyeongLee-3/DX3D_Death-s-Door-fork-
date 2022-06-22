#include "ShaderInfo.fx"

cbuffer HDRRenderCBuffer : register(b10)
{
	float g_HDRMiddleGray;
	float g_HDRLumWhiteSqr;
	float2 g_HDREmpty;
}

Texture2DMS<float4> g_HDRTex : register(t10);
StructuredBuffer<float> g_AvgLum : register(t11);

float3 ToneMapping(float3 HDRColor)
{
	// ���� �ȼ��� ���� �ֵ� ������ ���
	float LScale = dot(float4(HDRColor, 0.f), LUM_FACTOR);
	LScale *= g_HDRMiddleGray / g_AvgLum[0];
	LScale = (LScale + (LScale * LScale / g_HDRLumWhiteSqr)) / (1.0 + LScale);

	return HDRColor * LScale;
}

struct VS_OUTPUT_HDR
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION;
};

VS_OUTPUT_HDR HDRRenderVS(uint VertexID : SV_VertexID)
{
	VS_OUTPUT_HDR Output = (VS_OUTPUT_HDR) 0;

	Output.ProjPos = float4(g_NullPos[VertexID], 0.f, 1.f);
	Output.Pos = Output.ProjPos;
    
	return Output;
}

PSOutput_Single HDRRenderPS(VS_OUTPUT_HDR Input)
{
	PSOutput_Single Output = (PSOutput_Single)0;

	float2 UV = (float2) 0;
	UV.x = Input.ProjPos.x / Input.ProjPos.w * 0.5f + 0.5f;
	UV.y = Input.ProjPos.y / Input.ProjPos.w * -0.5f + 0.5f;

	int2 TargetPos = (int2) 0;
	TargetPos.x = (int) (UV.x * g_Resolution.x);
	TargetPos.y = (int) (UV.y * g_Resolution.y);

	// ���� ���� ���
	float3 Color = g_HDRTex.Load(TargetPos, 0);

	// �� ����
	Color = ToneMapping(Color);

	Output.Color.rgb = Color;
	Output.Color.a = 1.f;

	return Output;
}
