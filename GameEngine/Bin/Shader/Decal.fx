
#include "ShaderInfo.fx"

struct VS_OUTPUT_DECAL
{
	float4 Pos : SV_POSITION;
	float4 ProjPos : POSITION;
};

struct VS_OUTPUT_DEBUGDECAL
{
	float4 Pos : SV_POSITION;
	float4 ProjPos : POSITION;
	float3 Normal : NORMAL;
};

struct PS_OUTPUT_DECAL
{
	float4 Diffuse : SV_Target;
	float4 Normal : SV_Target1;
	float4 Material : SV_Target2;
};

Texture2D g_GBufferDepth : register(t10);

VS_OUTPUT_DECAL DecalVS(float3 Pos : POSITION)
{
	VS_OUTPUT_DECAL output = (VS_OUTPUT_DECAL) 0;
    
	output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;
    
	return output;
}

PS_OUTPUT_DECAL DecalPS(VS_OUTPUT_DECAL input)
{
	PS_OUTPUT_DECAL output = (PS_OUTPUT_DECAL) 0;
    
	float2 ScreenUV = input.ProjPos.xy / input.ProjPos.w;
	ScreenUV = ScreenUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
    
	float4 Depth = g_GBufferDepth.Sample(g_BaseSmp, ScreenUV);
    
	if (Depth.a == 0.f)
		clip(-1);
    
	float4 Pos;
	Pos.x = (ScreenUV.x * 2.f - 1.f) * Depth.g;
	Pos.y = (ScreenUV.y * -2.f + 1.f) * Depth.g;
	Pos.z = Depth.r * Depth.g;
	Pos.w = Depth.g;
    
    // ���� ������ �����.
	float3 LocalPos = mul(Pos, g_matInvWVP).xyz;
    
    // DecalMesh�� ���� ũ��� -0.5 ~ 0.5 �����̹Ƿ� �� ������ �����
    // Decal ������ �ƴѰɷ� �Ǵ��Ѵ�.
	clip(0.5f - abs(LocalPos));
    
	return output;
}
