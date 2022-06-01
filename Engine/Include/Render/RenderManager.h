#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture/RenderTarget.h"
#include "../Resource/Shader/InstancingCBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"

struct RenderInstancingList
{
	std::list<class CSceneComponent*> RenderList;
	class CMesh* Mesh;
	CStructuredBuffer* Buffer;
	int BufferCount; // �ν��Ͻ��� Scene Component�� ����
	CInstancingCBuffer* CBuffer;
	bool Animation;

	RenderInstancingList()
	{
		Mesh = nullptr;
		Animation = false;

		Buffer = new CStructuredBuffer;

		Buffer->Init("InstancingBuffer", sizeof(Instancing3DInfo), 100, 40, true, 
			(int)Buffer_Shader_Type::Vertex | (int)Buffer_Shader_Type::Pixel);

		BufferCount = 100;

		CBuffer = new CInstancingCBuffer;
		CBuffer->Init();
	}

	~RenderInstancingList()
	{
		SAFE_DELETE(CBuffer);
		SAFE_DELETE(Buffer);
	}
};

struct RenderLayer
{
	std::string		Name;
	int				LayerPriority;
	std::list<class CSceneComponent*> RenderList;
	std::vector<RenderInstancingList*> m_vecInstancing;
	int				InstancingIndex;
	int				RenderCount;

	RenderLayer()
	{
		LayerPriority = 0;
		RenderCount = 0;
		InstancingIndex = 0;
		m_vecInstancing.resize(30);

		for (int i = 0; i < 30; ++i)
		{
			m_vecInstancing[i] = new RenderInstancingList;
		}
	}

	~RenderLayer()
	{
		size_t Size = m_vecInstancing.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(m_vecInstancing[i]);
		}
	}
};

class CRenderManager
{
private:
	class CRenderStateManager* m_RenderStateManager;

private:
	const std::list<CSharedPtr<class CGameObject>>* m_ObjectList;
	std::vector<RenderLayer*>	m_RenderLayerList;
	class CStandard2DConstantBuffer* m_Standard2DCBuffer;
	class CRenderState* m_DepthDisable;
	class CRenderState* m_AlphaBlend;
	class CRenderState* m_LightAccBlend;
	CSharedPtr<class CShader> m_LightBlendShader;
	CSharedPtr<class CShader> m_LightBlendRenderShader;
	CSharedPtr<class CShader> m_Standard3DInstancingShader;

	// GBuffer
	std::vector<CSharedPtr<CRenderTarget>>	m_vecGBuffer;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecDecal;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecLightBuffer;

	// Animation Editor 
	CSharedPtr<class CShader> m_Mesh3DNoLightRenderShader; // m_AnimEditorRenderTarget �� �׷����� ���� Shader 
	CSharedPtr<CRenderTarget>	m_AnimEditorRenderTarget; // Skinning ó�� ����, �ش� �����, ������ RenderTarget �� �׷�����.

	// Particle Editor
	// CSharedPtr<CRenderTarget>	m_ParticleEffectEditorRenderTarget; // Skinning ó�� ����, �ش� �����, ������ RenderTarget �� �׷�����.
public:
	class CStandard2DConstantBuffer* GetStandard2DCBuffer()	const
	{
		return m_Standard2DCBuffer;
	}
	CRenderTarget* GetAnimationRenderTarget() const
	{
		return m_AnimEditorRenderTarget;
	}
	// CRenderTarget* GetParticleEffectRenderTarget() const
	// {
	// 	return m_ParticleEffectEditorRenderTarget;
	// }
public:
	void SetObjectList(const std::list<CSharedPtr<class CGameObject>>* List)
	{
		m_ObjectList = List;
	}

	void AddRenderList(class CSceneComponent* Component);
	void CreateLayer(const std::string& Name, int Priority);
	void SetLayerPriority(const std::string& Name, int Priority);

public:
	bool Init();
	void Render();
private:
	void RenderGBuffer();
	void RenderDecal();
	void RenderLightAcc();
	void RenderLightBlend();
	void RenderFinalScreen();
	void RenderAnimationEditor();
	// void RenderParticleEffectEditor();
	// 
	// Render State
public:
	void SetBlendFactor(const std::string& Name, float r, float g, float b, float a);
	void AddBlendInfo(const std::string& Name, bool BlendEnable = true, D3D11_BLEND SrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND DestBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND SrcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& Name, bool AlphaToCoverageEnable, bool IndependentBlendEnable);
public:
	class CRenderState* FindRenderState(const std::string& Name);

private:
	void RenderDefaultInstancing();

private :
	int GetRenderLayerIndex(const std::string& Name);

private:
	static bool Sortlayer(RenderLayer* Src, RenderLayer* Dest);

	DECLARE_SINGLE(CRenderManager)
};

