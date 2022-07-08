
#include "Transparent3D.fx"

Vertex3DOutput LaserVS(Vertex3D input)
{
    Vertex3DOutput output = (Vertex3DOutput) 0;
    
    output.Pos = mul(float4(input.Pos, 1.f), g_matWVP);
    output.ProjPos = output.Pos;
    output.UV = input.UV;
    
    output.ViewPos = mul(float4(input.Pos, 1.f), g_matWV).xyz;
    
    return output;
}

PS_OUTPUT_TRANSPARENT LaserPS(Vertex3DOutput Input)
{
    PS_OUTPUT_TRANSPARENT output = (PS_OUTPUT_TRANSPARENT) 0;
    
    float4 BaseTextureColor = g_BaseTexture.Sample(g_BaseSmp, Input.UV);
    
    if (BaseTextureColor.a == 0.f || g_MtrlOpacity == 0.f)
        clip(-1);
    
    if(Input.UV.y < 0.2f || Input.UV.y > 0.8f)
        clip(-1);
    
    if (BaseTextureColor.r < 0.25f && BaseTextureColor.g < 0.25f && BaseTextureColor.b < 0.25f)
        clip(-1);
    
    output.ScreenColor.rgb = BaseTextureColor.rgb;
    
    output.ScreenColor.a = BaseTextureColor.a * g_MtrlOpacity;
    
    return output;
}