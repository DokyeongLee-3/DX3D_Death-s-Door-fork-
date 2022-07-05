
#include "Standard3DInfo.fx"

Vertex3DOutput Standard3DVS(Vertex3D input)
{
    Vertex3DOutput output = (Vertex3DOutput) 0;

    SkinningInfo Info = Skinning(input.Pos, input.Normal, input.Tangent,
                input.Binormal, input.BlendWeight, input.BlendIndex);
    
    float3 Pos = Info.Pos;

    output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
    output.Pos = output.ProjPos;
    
    // ������� ��ġ�� ������ش�.
    output.ViewPos = mul(float4(Pos, 1.f), g_matWV).xyz;

    // ��������� ��ġ
	output.WorldPos = Pos;
    
    // �� ������ Normal�� ������ش�.
    output.Normal = normalize(mul(float4(Info.Normal, 0.f), g_matWV).xyz);

    // �� ������ Tangent�� ������ش�.
    output.Tangent = normalize(mul(float4(Info.Tangent, 0.f), g_matWV).xyz);

    // �� ������ Binormal�� ������ش�.
    output.Binormal = normalize(mul(float4(Info.Binormal, 0.f), g_matWV).xyz);

    output.UV = input.UV;

    return output;
}

PSOutput_GBuffer Standard3DPS(Vertex3DOutput input)
{
    PSOutput_GBuffer output = (PSOutput_GBuffer) 0;
    
    //LightResult LightInfo = ComputeLight(input.ViewPos, input.Normal,
    //    input.Tangent, input.Binormal, input.UV);

    float4 BaseTextureColor = g_BaseTexture.Sample(g_BaseSmp, input.UV);

    if (BaseTextureColor.a == 0.f || g_MtrlOpacity == 0.f)
        clip(-1);
    
    output.Diffuse.rgb = BaseTextureColor.rgb;// * (LightInfo.Dif + LightInfo.Amb) + LightInfo.Spc + LightInfo.Emv;

    output.Diffuse.a = BaseTextureColor.a * g_MtrlOpacity;
    
    output.GBuffer1.rgb = ComputeBumpNormal(input.Normal, input.Tangent, input.Binormal, input.UV);
    output.GBuffer1.a = 1.f;
    
    output.GBuffer2.r = input.ProjPos.z / input.ProjPos.w;
    output.GBuffer2.g = input.ProjPos.w;
    output.GBuffer2.b = g_MtrlSpecularColor.w;
    output.GBuffer2.a = input.WorldPos.y;    // y fog�� ���� ���� y�� ����

    output.GBuffer3.r = ConvertColor(g_MtrlBaseColor);
    output.GBuffer3.g = ConvertColor(g_MtrlAmbientColor);

    output.GBuffer4.rgb = input.Tangent.xyz;
    output.GBuffer4.a = 1.f;
    
    output.GBuffer5.rgb = input.Binormal.xyz;
    output.GBuffer5.a = 1.f;
    
    float4 SpecularColor = g_MtrlSpecularColor.xyzw;
	
    if (g_MtrlSpecularTex)
        SpecularColor = g_SpecularTexture.Sample(g_BaseSmp, input.UV).xxxx;
    
    output.GBuffer3.b = ConvertColor(SpecularColor);
    
    float4 EmissiveColor = g_MtrlEmissiveColor.xyzw;
	
    if (g_MtrlEmissiveTex)
        EmissiveColor = g_EmissiveTexture.Sample(g_BaseSmp, input.UV).xxxx;

    output.GBuffer3.a = ConvertColor(EmissiveColor);

    return output;
}

SkinningInfo SkinningInstancing(float3 Pos, float3 Normal, float3 Tangent, float3 Binormal,
    float4 BlendWeight, float4 BlendIndex, uint InstanceID)
{
	SkinningInfo Info = (SkinningInfo) 0;
    
	if (g_InstancingInfoArray[InstanceID].g_MtrlAnimation3DEnable == 0)
	{
		Info.Pos = Pos;
		Info.Normal = Normal;
		Info.Tangent = Tangent;
		Info.Binormal = Binormal;
        
		return Info;
	}
    
	for (int i = 0; i < 4; ++i)
	{
		if (BlendWeight[i] == 0.f)
			continue;
        
		matrix matBone = g_SkinningBoneMatrixArray[(InstanceID * g_InstancingBoneCount) + (int) BlendIndex[i]];
        
		Info.Pos += (mul(float4(Pos, 1.f), matBone) * BlendWeight[i]).xyz;
		Info.Normal += (mul(float4(Normal, 0.f), matBone) * BlendWeight[i]).xyz;
		Info.Tangent += (mul(float4(Tangent, 0.f), matBone) * BlendWeight[i]).xyz;
		Info.Binormal += (mul(float4(Binormal, 0.f), matBone) * BlendWeight[i]).xyz;
	}
    
	Info.Normal = normalize(Info.Normal);
	Info.Tangent = normalize(Info.Tangent);
	Info.Binormal = normalize(Info.Binormal);
    
    
	return Info;

}

// Instancing
Vertex3DOutputInstancing Standard3DInstancingVS(Vertex3DInstancing input)
{
	Vertex3DOutputInstancing output = (Vertex3DOutputInstancing) 0;
    
	SkinningInfo Info = SkinningInstancing(input.Pos, input.Normal, input.Tangent,
                input.Binormal, input.BlendWeight, input.BlendIndex, input.InstanceID);
    
	output.InstanceID = input.InstanceID;
    
	float3 Pos = Info.Pos;

	output.ProjPos = mul(float4(Pos, 1.f), g_InstancingInfoArray[input.InstanceID].matWVP);
	output.Pos = output.ProjPos;
    
    // ������� ��ġ�� ������ش�.
	output.ViewPos = mul(float4(Pos, 1.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz;
    
    // �� ������ Normal�� ������ش�.
	output.Normal = normalize(mul(float4(Info.Normal, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);
    // �� ������ Tangent�� ������ش�.
	output.Tangent = normalize(mul(float4(Info.Tangent, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);
    // �� ������ Binormal�� ������ش�.
	output.Binormal = normalize(mul(float4(Info.Binormal, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);
	output.UV = input.UV;

	return output;
}

float3 ComputeBumpNormalInstancing(float3 Normal, float3 Tangent, float3 Binormal,
	float2 UV, uint InstanceID)
{
	float3 result = Normal;
	
	if (g_InstancingInfoArray[InstanceID].g_MtrlBumpEnable == 1)
	{
		float4 NormalColor = g_NormalTexture.Sample(g_BaseSmp, UV);
		
		// ������ 0 ~ 1 �����̹Ƿ� -1 ~ 1 ������ �������ͷ� ������ش�.
		float3 ConvertNormal = NormalColor.xyz * 2.f - 1.f;
		// z�� ������ + �������� ������ش�.
		ConvertNormal.z = 1.f;
		ConvertNormal = normalize(ConvertNormal);
		
		// Tangent, Binormal, Normal�� �̿��ؼ� ������ ������� ����
		// ���͸� ��������� ��ȯ���ش�. ������� �Ǵ� ������ Tangent,
		// Binormal, Normal 3�� ��� ��������� ��ȯ�� ���Ͱ� ������
		// �����̴�.
		float3x3 mat =
		{
			Tangent,
			Binormal,
			Normal
		};

		result = normalize(mul(ConvertNormal, mat));
	}
	
	return result;
}


PSOutput_GBuffer Standard3DInstancingPS(Vertex3DOutputInstancing input)
{
	PSOutput_GBuffer output = (PSOutput_GBuffer) 0;
    
    //LightResult LightInfo = ComputeLight(input.ViewPos, input.Normal,
    //    input.Tangent, input.Binormal, input.UV);

	float4 BaseTextureColor = g_BaseTexture.Sample(g_BaseSmp, input.UV);

	if (BaseTextureColor.a == 0.f || g_InstancingInfoArray[input.InstanceID].g_MtrlOpacity == 0.f)
		clip(-1);
    
	output.Diffuse.rgb = BaseTextureColor.rgb; // * (LightInfo.Dif + LightInfo.Amb) + LightInfo.Spc + LightInfo.Emv;

	output.Diffuse.a = BaseTextureColor.a * g_InstancingInfoArray[input.InstanceID].g_MtrlOpacity;
    
	output.GBuffer1.rgb = ComputeBumpNormalInstancing(input.Normal, input.Tangent, input.Binormal, input.UV, input.InstanceID);
	output.GBuffer1.a = 1.f;
    
	output.GBuffer2.r = input.ProjPos.z / input.ProjPos.w;
	output.GBuffer2.g = input.ProjPos.w;
	output.GBuffer2.b = g_InstancingInfoArray[input.InstanceID].g_MtrlSpecularColor.w;
	output.GBuffer2.a = 1.f;
    
	output.GBuffer3.r = ConvertColor(g_InstancingInfoArray[input.InstanceID].g_MtrlBaseColor);
	output.GBuffer3.g = ConvertColor(g_InstancingInfoArray[input.InstanceID].g_MtrlAmbientColor);
    
	output.GBuffer4.rgb = input.Tangent.xyz;
	output.GBuffer4.a = g_InstancingInfoArray[input.InstanceID].g_MtrlReceiveDecal;
    
	output.GBuffer5.rgb = input.Binormal.xyz;
	output.GBuffer5.a = 1.f;
    
	float4 SpecularColor = g_InstancingInfoArray[input.InstanceID].g_MtrlSpecularColor.xyzw;
	
	if (g_InstancingInfoArray[input.InstanceID].g_MtrlSpecularTex)
		SpecularColor = g_SpecularTexture.Sample(g_BaseSmp, input.UV).xxxx;
	
	SpecularColor = float4(0.5f, 0.5f, 0.5f, 1.f);
    
	output.GBuffer3.b = ConvertColor(SpecularColor);
    
	float4 EmissiveColor = g_InstancingInfoArray[input.InstanceID].g_MtrlEmissiveColor.xyzw;
	
	if (g_InstancingInfoArray[input.InstanceID].g_MtrlEmissiveTex)
		EmissiveColor = g_EmissiveTexture.Sample(g_BaseSmp, input.UV).xxxx;
    
	output.GBuffer3.a = ConvertColor(EmissiveColor);

	return output;
}
// Instancing

/*
VertexUVOutput RenderTargetVS(VertexUV input)
{
    VertexUVOutput output = (VertexUVOutput)0;

    output.Pos = mul(float4(input.Pos, 1.f), g_matWidgetWP);
    output.UV = input.UV;

    return output;
}

PSOutput_Single RenderTargetPS(VertexUVOutput input)
{
    PSOutput_Single output = (PSOutput_Single)0;

    int2 Pos = (int2)0;

    Pos.x = (int)(input.UV.x * g_Resolution.x);
    Pos.y = (int)(input.UV.y * g_Resolution.y);

    float4 BaseTextureColor = g_TargetTex.Load(Pos, 0);

    if (BaseTextureColor.a == 0.f)
        clip(-1);

    output.Color = BaseTextureColor;

    return output;
}
*/

PSOutput_Single StandardNoLight3DPS(Vertex3DOutput input)
{
    PSOutput_Single output = (PSOutput_Single)0;

    //LightResult LightInfo = ComputeLight(input.ViewPos, input.Normal,
    //    input.Tangent, input.Binormal, input.UV);

    // float4 BaseTextureColor = g_BaseTexture.Sample(g_BaseSmp, input.UV);
    float4 BaseTextureColor = g_BaseTexture.Sample(g_BaseSmp, input.UV);

    if (BaseTextureColor.a == 0.f || g_MtrlOpacity == 0.f)
        clip(-1);

    output.Color.rgb = BaseTextureColor.rgb;// * (LightInfo.Dif + LightInfo.Amb) + LightInfo.Spc + LightInfo.Emv;

    output.Color.a = BaseTextureColor.a * g_MtrlOpacity;

    return output;
}

Vertex3DOutput Standard3DWireFrameVS(Vertex3D input)
{
    Vertex3DOutput output = (Vertex3DOutput) 0;

    float3 Pos = input.Pos;

    output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
    output.Pos = output.ProjPos;
    
    // ������� ��ġ�� ������ش�.
    output.ViewPos = mul(float4(Pos, 1.f), g_matWV).xyz;
    
    // �� ������ Normal�� ������ش�.
	output.Normal = normalize(mul(float4(input.Normal, 0.f), g_matWV).xyz);
    // �� ������ Tangent�� ������ش�.
    output.Tangent = normalize(mul(float4(input.Tangent, 0.f), g_matWV).xyz);
    // �� ������ Binormal�� ������ش�.
    output.Binormal = normalize(mul(float4(input.Binormal, 0.f), g_matWV).xyz);
    output.UV = input.UV;

    return output;
}

PSOutput_GBuffer Standard3DWireFramePS(Vertex3DOutput input)
{
    PSOutput_GBuffer output = (PSOutput_GBuffer) 0;
    
	float4 BaseTextureColor = float4(0.f, 1.f, 0.f, 1.f);
    
    output.Diffuse = BaseTextureColor; // * (LightInfo.Dif + LightInfo.Amb) + LightInfo.Spc + LightInfo.Emv;
    
    output.GBuffer1.rgb = input.Normal;
    output.GBuffer1.a = 1.f;
    
    output.GBuffer2.r = input.ProjPos.z / input.ProjPos.w;
    output.GBuffer2.g = input.ProjPos.w;
    output.GBuffer2.b = 1.f;
    output.GBuffer2.a = 1.f;
    
	output.GBuffer3.r = float4(0.f, 1.f, 0.f, 1.f);
	output.GBuffer3.r = float4(0.f, 1.f, 0.f, 1.f);
    
    output.GBuffer4.rgb = input.Tangent.xyz;
    output.GBuffer4.a = 1.f;
    
    output.GBuffer5.rgb = input.Binormal.xyz;
    output.GBuffer5.a = 1.f;
    
	float4 SpecularColor = float4(0.f, 1.f, 0.f, 1.f);
	
    output.GBuffer3.b = ConvertColor(SpecularColor);
    
	float4 EmissiveColor = float4(0.f, 1.f, 0.f, 1.f);
    
    output.GBuffer3.a = ConvertColor(EmissiveColor);

    return output;
}
