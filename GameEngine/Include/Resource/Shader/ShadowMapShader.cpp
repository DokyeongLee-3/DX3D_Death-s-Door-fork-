
#include "ShadowMapShader.h"
#include "../ResourceManager.h"
#include "ShadowMapInstancingShader.h"

CShadowMapShader::CShadowMapShader()
{
	SetTypeID<CShadowMapShader>();
}

CShadowMapShader::~CShadowMapShader()
{
}

bool CShadowMapShader::Init()
{
	if (!LoadVertexShader("Standard3DShadowMapVS", TEXT("ShadowMap.fx"), SHADER_PATH))
		return false;

	if (!LoadPixelShader("Standard3DShadowMapPS", TEXT("ShadowMap.fx"), SHADER_PATH))
		return false;

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
		return false;

	// Instancing Shader도 같이 만든다.
	bool Success = CResourceManager::GetInst()->CreateInstancingShader<CShadowMapShader, CShadowMapInstancingShader>(this, "ShadowMapInstancingShader");
	if (!Success)
	{
		return false;
	}

	return true;
}
