#pragma once

#include "../GameInfo.h"

class CPostFXRenderer
{
	friend class CRenderManager;

private:
	CPostFXRenderer();
	~CPostFXRenderer();

public:
	bool Init();
	void ComputeAdaptation(float DeltaTime);

public:
	void SetMiddleGray(float Gray);
	void SetLumWhite(float White);
	void SetBloomThreshold(float Threshold);
	void SetBloomScale(float Scale);
	void SetDOFMin(float Min);
	void SetDOFMax(float Max);
	void SetFogColor(const Vector3& Color);
	void SetFogType(Fog_Type Type);
	void SetFogStart(float Start);
	void SetFogEnd(float End);
	void SetFogDensity(float Density);

	float GetMiddleGray() const;
	float GetLumWhite() const;
	float GetBloomThreshold() const;
	float GetBloomScale() const;
	float GetDOFMin() const;
	float GetDOFMax() const;
	const Vector3& GetFogColor() const;
	Fog_Type GetFogType() const;
	float GetFogStart() const;
	float GetFogEnd() const;
	float GetFogDensity() const;

	void SetAdaptationTime(float Time)
	{
		m_AdaptationTime = Time;
	}

	float GetAdaptationTime() const
	{
		return m_AdaptationTime;
	}

public:
	void Render(float DeltaTime, class CRenderTarget* LDRTarget);

private:
	void ComputeHDR(class CRenderTarget* LDRTarget);
	void ComputeBloom();
	void Blur();
	void RenderFinal(class CRenderTarget* LDRTarget);

private:
	// Depth Buffer
	class CRenderTarget* m_GBufferDepth;

	// Shader
	class CComputeShader* m_DownScaleFirstPassShader;
	class CComputeShader* m_DownScaleSecondPassShader;
	class CComputeShader* m_BloomShader;
	class CComputeShader* m_BlurVerticalShader;
	class CComputeShader* m_BlurHorizontalShader;
	class CShader* m_HDRRenderShader;
	
	// ConstantBuffer
	class CDownScaleCBuffer* m_DownScaleCBuffer;
	class CHDRRenderCBuffer* m_HDRRenderCBuffer;
	class CFogCBuffer*		 m_FogCBuffer;

	// RenderState
	class CRenderState* m_DepthDisable;
	class CRenderState* m_AlphaBlend;

	// HDR Resources
	ID3D11Buffer* m_DownScaleBuffer;
	ID3D11UnorderedAccessView* m_DownScaleUAV;
	ID3D11ShaderResourceView* m_DownScaleSRV;

	ID3D11Buffer* m_LuminanceAverageBuffer;
	ID3D11UnorderedAccessView* m_LuminanceAverageBufferUAV;
	ID3D11ShaderResourceView* m_LuminanceAverageBufferSRV;

	ID3D11Buffer* m_PrevFrameLumAverageBuffer;
	ID3D11UnorderedAccessView* m_PrevFrameLumAverageBufferUAV;
	ID3D11ShaderResourceView* m_PrevFrameLumAverageBufferSRV;

	/// Bloom ����
	// 1. DownScaled Scene Texture
	ID3D11Texture2D* m_DownScaleRT;
	ID3D11UnorderedAccessView* m_DownScaleRTUAV;
	ID3D11ShaderResourceView* m_DownScaleRTSRV;

	// 2. Bloom Texture
	ID3D11Texture2D* m_BloomRT;
	ID3D11UnorderedAccessView* m_BloomUAV;
	ID3D11ShaderResourceView* m_BloomSRV;

	// 3. Temp Texture
	ID3D11Texture2D* m_Temp1RT;
	ID3D11UnorderedAccessView* m_Temp1UAV;
	ID3D11ShaderResourceView* m_Temp1SRV;

	ID3D11Texture2D* m_Temp2RT;
	ID3D11UnorderedAccessView* m_Temp2UAV;
	ID3D11ShaderResourceView* m_Temp2SRV;

	// Adaptation ���� 
	bool  m_FirstFrame;
	float m_AdaptationTimer;
	float m_AdaptationTime;

	// DEBUG SECTION
 //	ID3D11Buffer* m_Temp;
 //	ID3D11Buffer* m_Temp2;
 //	ID3D11UnorderedAccessView* m_Temp2UAV;
 //	float m_TempBuf[57];
 //	ID3D11Query* m_Query;
	// DEBUG SECTION END
};

