#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture/RenderTarget.h"

struct RenderLayer
{
	std::string		Name;
	int				LayerPriority;
	std::vector<class CSceneComponent*>	RenderList;
	std::unordered_map<size_t, class CRenderInstancing*> mapInstancing;
	int				RenderCount;

	RenderLayer()
	{
		LayerPriority = 0;
		RenderCount = 0;
		RenderList.resize(500);
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

	// 인스턴싱용 구조화 버퍼를 만들어준다.
	std::vector<class CStructuredBuffer*>	m_vecInstancingBuffer;

	// GBuffer
	std::vector<CSharedPtr<CRenderTarget>>	m_vecGBuffer;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecDecal;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecLightBuffer;

	// Animation Editor Buffer
	CSharedPtr<class CShader> m_NoLightRenderShader; // m_AnimRenderTargetPrevProcess 에 그려내기 위한 Shader 
	CSharedPtr<class CShader> m_AnimRenderShader;    // 
	CSharedPtr<CRenderTarget>	m_AnimRenderTargetPrevProcess; // Skinning 처리 이후, 해당 출력을, 별도의 RenderTarget 에 그려낸다.
	CSharedPtr<CRenderTarget>	m_AnimationRenderTarget; // Null Buffer 를 활용, m_AnimRenderTargetPrevProcess 에 기록된 사항 출력

public:
	class CStandard2DConstantBuffer* GetStandard2DCBuffer()	const
	{
		return m_Standard2DCBuffer;
	}
	CRenderTarget* GetAnimationRenderTarget() const
	{
		return m_AnimationRenderTarget;
	}

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
	void RenderAnimationEditorPrevProcess();
	void RenderAnimationEditor();
	// Render State
public:
	void SetBlendFactor(const std::string& Name, float r, float g, float b, float a);
	void AddBlendInfo(const std::string& Name, bool BlendEnable = true, D3D11_BLEND SrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND DestBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND SrcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& Name, bool AlphaToCoverageEnable, bool IndependentBlendEnable);
private :
	int GetRenderLayerIndex(const std::string& Name);
public:
	class CRenderState* FindRenderState(const std::string& Name);

private:
	static bool Sortlayer(RenderLayer* Src, RenderLayer* Dest);

	DECLARE_SINGLE(CRenderManager)
};

