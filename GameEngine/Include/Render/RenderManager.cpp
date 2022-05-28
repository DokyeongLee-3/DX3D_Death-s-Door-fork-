
#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Component/SceneComponent.h"
#include "RenderStateManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh/AnimationMesh.h"
#include "RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Viewport.h"
#include "../Engine.h"
#include "../Device.h"
#include "../Resource/Shader/Shader.h"
#include "../Component/PaperBurnComponent.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	m_RenderStateManager(nullptr),
	m_Standard2DCBuffer(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	m_RenderLayerList.clear();

	SAFE_DELETE(m_Standard2DCBuffer);
	SAFE_DELETE(m_RenderStateManager);
}

void CRenderManager::AddRenderList(CSceneComponent* Component)
{
	RenderLayer* Layer = nullptr;

	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Component->GetLayerName())
		{
			Layer = *iter;
			break;
		}
	}

	if (!Layer)
		return;

	Layer->RenderList.push_back(Component);
}

void CRenderManager::CreateLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	m_RenderLayerList.push_back(Layer);

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::Sortlayer);
}

void CRenderManager::SetLayerPriority(const std::string& Name, int Priority)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->LayerPriority = Priority;
			break;
		}
	}

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::Sortlayer);
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;

	m_RenderStateManager->Init();

	m_Standard2DCBuffer = new CStandard2DConstantBuffer;

	m_Standard2DCBuffer->Init();

	// �⺻ ���̾� ����
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = "Back";
	Layer->LayerPriority = 0;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Default";
	Layer->LayerPriority = 1;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Decal";
	Layer->LayerPriority = 2;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Particle";
	Layer->LayerPriority = 3;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "ScreenWidgetComponent";
	Layer->LayerPriority = 4;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "AnimationEditorLayer";
	Layer->LayerPriority = 5;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "ParticleEditorLayer";
	Layer->LayerPriority = 6;

	m_RenderLayerList.push_back(Layer);

	m_DepthDisable = m_RenderStateManager->FindRenderState("DepthDisable");
	m_AlphaBlend = m_RenderStateManager->FindRenderState("AlphaBlend");
	m_LightAccBlend = m_RenderStateManager->FindRenderState("LightAcc");

	// ���۵� �������� Ÿ�� ����
	Resolution RS = CDevice::GetInst()->GetResolution();

	if (!CResourceManager::GetInst()->CreateTarget("Diffuse",
		RS.Width, RS.Height, DXGI_FORMAT_R8G8B8A8_UNORM))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer1",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer2",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer3",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer4",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer5",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	CRenderTarget* GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("Diffuse");
	GBufferTarget->SetPos(Vector3(0.f, 0.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer1");
	GBufferTarget->SetPos(Vector3(0.f, 100.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer2");
	GBufferTarget->SetPos(Vector3(0.f, 200.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer3");
	GBufferTarget->SetPos(Vector3(0.f, 300.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer4");
	GBufferTarget->SetPos(Vector3(0.f, 400.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer5");
	GBufferTarget->SetPos(Vector3(0.f, 500.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	// Decal�� �̹� ������ ����Ÿ�ٿ� �����Ѵ�.
	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("Diffuse");
	m_vecDecal.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer1");
	m_vecDecal.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer3");
	m_vecDecal.push_back(GBufferTarget);

	if (!CResourceManager::GetInst()->CreateTarget("LightDif",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("LightSpc",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("LightEmv",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	CRenderTarget* LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightDif");
	LightTarget->SetPos(Vector3(100.f, 0.f, 0.f));
	LightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);

	LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightSpc");
	LightTarget->SetPos(Vector3(100.f, 100.f, 0.f));
	LightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);

	LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightEmv");
	LightTarget->SetPos(Vector3(100.f, 200.f, 0.f));
	LightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);


	if (!CResourceManager::GetInst()->CreateTarget("FinalScreen",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	CRenderTarget* FinalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");
	FinalScreenTarget->SetPos(Vector3(200.f, 0.f, 0.f));
	FinalScreenTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	FinalScreenTarget->SetDebugRender(true);

	m_LightBlendShader = CResourceManager::GetInst()->FindShader("LightBlendShader");
	m_LightBlendRenderShader = CResourceManager::GetInst()->FindShader("LightBlendRenderShader");
	m_Mesh3DNoLightRenderShader = CResourceManager::GetInst()->FindShader("Mesh3DNoLightShader");
	m_Standard3DInstancingShader = CResourceManager::GetInst()->FindShader("Standard3DInstancingShader");

	// Animation Editor �� Render Target 
	if (!CResourceManager::GetInst()->CreateTarget("AnimationEditorRenderTarget",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	m_AnimEditorRenderTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("AnimationEditorRenderTarget");
	m_AnimEditorRenderTarget->SetPos(Vector3(450.f, 100.f, 0.f));
	m_AnimEditorRenderTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	m_AnimEditorRenderTarget->SetDebugRender(false);

	// Animation Editor �� Render Target 
	if (!CResourceManager::GetInst()->CreateTarget("ParticleEffectRenderTarget",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	m_ParticleEffectEditorRenderTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("ParticleEffectRenderTarget");
	m_ParticleEffectEditorRenderTarget->SetPos(Vector3(600.f, 100.f, 0.f));
	m_ParticleEffectEditorRenderTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	m_ParticleEffectEditorRenderTarget->SetDebugRender(false);


	return true;
}

void CRenderManager::Render()
{
	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderList.clear();
			(*iter)->InstancingIndex = 0;

			auto iter1 = (*iter)->m_vecInstancing.begin();
			auto iter1End = (*iter)->m_vecInstancing.end();

			for (; iter1 != iter1End; ++iter1)
			{
				(*iter1)->RenderList.clear();
			}
		}
	}

	{
		auto	iter = m_ObjectList->begin();
		auto	iterEnd = m_ObjectList->end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->PrevRender();
		}	
	}

	{
		const std::list<InstancingCheckCount*>* InstancingList = CSceneComponent::GetInstancingCheckList();

		auto	iter = InstancingList->begin();
		auto	iterEnd = InstancingList->end();

		for (; iter != iterEnd; ++iter)
		{
			// (*iter) �� �ϳ��� Mesh �� �������� ����ϴ� SceneComponent ����
			// 10�� �̻��̶��, �ش� SceneComponent ���� ���� Instancing ���� �׷���� �Ѵ�.
			if ((*iter)->InstancingList.size() >= 10)
			{
				RenderLayer* Layer = nullptr;

				size_t	Size = m_RenderLayerList.size();

				// ��� Render Layer ���� ���鼭, ���� Scene Component ���ܵ��� ���� Layer �� ã�Ƴ���
				for (size_t i = 0; i < Size; ++i)
				{
					if (m_RenderLayerList[i]->Name == (*iter)->LayerName)
					{
						Layer = m_RenderLayerList[i];
						break;
					}
				}

				if (Layer)
				{
					// �� Layer ���� m_vecInstancing �� �ִ�.
					// �ϳ��� Layer �ȿ�����, ���� Set ��ü���� Instancing ���� �׷��� �� �ʿ䰡 �ִ�.
					// ex) ��� 30�� ����, ���� 30�� ����
					// InstancingIndex �� ���� �ش� Layer �� Instancing ���� �׸� ���� ���� - 1. �� ���� ���ϰ� �ִ�
					// �׷��� ���� ���Ӱ� Instancing ���� �׸� ��ü ������ �߰��ؾ� �ϴµ�
					// �ش� ����. ���� vector �� Size �� ���ٸ�, �ٽ� Size �� ���Ҵ������ �Ѵ�.
					if (Layer->m_vecInstancing.size() == Layer->InstancingIndex)
					{
						Layer->m_vecInstancing.resize(Layer->InstancingIndex * 2);

						for (int i = 0; i < Layer->InstancingIndex; ++i)
						{
							Layer->m_vecInstancing[Layer->InstancingIndex + i] = new RenderInstancingList;
						}
					}

					// (*iter)->InstancingList ��, �ش� ��ü ����.�� ��ü ���. ���� ����ִ�.
					// ���� Instancing ���� �׷��ֱ� ���ؼ���, �ش� ��ü���� �ϳ��� ����ȭ ���ۿ� ������ ��Ƽ�
					// ����ؾ� �Ѵ�.
					// ��, �翬�� �ش� ��ü ������ ��� ����ȭ ������ ũ���, �翬�� ��ü ���� ��ü ũ�⺸�� ũ�ų� ���ƾ� �Ѵ�.
					// ����, ����ȭ ������ ũ�Ⱑ �۴ٸ�, ���� ũ�� * 1.5 �� �� ���Ҵ� ����� �Ѵ�.
					if ((*iter)->InstancingList.size() > Layer->m_vecInstancing[Layer->InstancingIndex]->BufferCount)
					{
						int	Count = Layer->m_vecInstancing[Layer->InstancingIndex]->BufferCount * 1.5f;

						// �Ҵ� ���� ����
						if ((*iter)->InstancingList.size() > Count)
							Count = (int)(*iter)->InstancingList.size();

						// ���� ����ȭ ���۸� �����Ѵ�.
						SAFE_DELETE(Layer->m_vecInstancing[Layer->InstancingIndex]->Buffer);

						// ����ȭ ���۸� ���Ҵ����ش�.
						Layer->m_vecInstancing[Layer->InstancingIndex]->Buffer = new CStructuredBuffer;

						Layer->m_vecInstancing[Layer->InstancingIndex]->Buffer->Init("InstancingBuffer", sizeof(Instancing3DInfo),
							Count, 40, true,
							(int)Buffer_Shader_Type::Vertex || (int)Buffer_Shader_Type::Pixel);
					}

					// ���� �ش� ��ü ����. �� �ȿ� �ִ� ��ü �ϳ��ϳ��� ��ȸ�� ���̴�.
					auto	iter1 = (*iter)->InstancingList.begin();
					auto	iter1End = (*iter)->InstancingList.end();

					// ���� RenderLayer �� , Ư�� ���� ������
					// �� ��ü �ϳ��ϳ��� Render List �� �־��� ���̴�.
					for (; iter1 != iter1End; ++iter1)
					{
						// ����ȭ ���ۿ� ������ ä�� �� �ִ� �Լ��� ���� ������ ä���ش�.
						Layer->m_vecInstancing[Layer->InstancingIndex]->RenderList.push_back(*iter1);
					}

					// Mesh ���� ����
					Layer->m_vecInstancing[Layer->InstancingIndex]->Mesh = (*iter)->Mesh;

					if ((*iter)->Mesh->CheckType<CAnimationMesh>())
					{
						Layer->m_vecInstancing[Layer->InstancingIndex]->Animation = true;
						Layer->m_vecInstancing[Layer->InstancingIndex]->CBuffer->SetBoneCount(((CAnimationMesh*)(*iter)->Mesh)->GetBoneCount());
					}

					++Layer->InstancingIndex;
				}
			}
		}
	}

	// ȯ��� ���
	CSharedPtr<CGameObject> SkyObj = CSceneManager::GetInst()->GetScene()->GetSkyObject();
	SkyObj->Render();

	// GBuffer�� ������.
	RenderGBuffer();

	// Decal�� �׷�����.
	RenderDecal();

	// ���� �������۸� ������.
	RenderLightAcc();

	// ���� �������ۿ� GBuffer�� �̿��Ͽ� ����ȭ���� ������.
	RenderLightBlend();

	// ����ó���� ���� ȭ���� ����ۿ� �׷�����.
	RenderFinalScreen();

	// Animation Editor Animation Instance ���ۿ� Render Target
	RenderAnimationEditor();

	// Particle Effect Editor ���ۿ� Render Target
	RenderParticleEffectEditor();

	m_vecGBuffer[2]->SetShader(10, (int)Buffer_Shader_Type::Pixel, 0);

	m_AlphaBlend->SetState();

	// ��ƼŬ ���̾� ���
	auto	iter = m_RenderLayerList[3]->RenderList.begin();
	auto	iterEnd = m_RenderLayerList[3]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render();
	}

	m_AlphaBlend->ResetState();

	m_vecGBuffer[2]->ResetShader(10, (int)Buffer_Shader_Type::Pixel, 0);

	// Screen Widget ���
	iter = m_RenderLayerList[4]->RenderList.begin();
	iterEnd = m_RenderLayerList[4]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render();
	}

	// ���� ������ Shader�� �Ѱ��ش�.
	//CSceneManager::GetInst()->GetScene()->GetLightManager()->SetShader();

	/*{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; ++i)
			{
				(*iter)->RenderList[i]->Render();
			}
		}
	}*/

	{
		auto	iter1 = m_RenderLayerList.begin();
		auto	iter1End = m_RenderLayerList.end();

		for (; iter1 != iter1End; ++iter1)
		{
			auto	iter2 = (*iter1)->RenderList.begin();
			auto	iter2End = (*iter1)->RenderList.end();

			for (; iter2 != iter2End; ++iter2)
			{
				(*iter2)->PostRender();
			}
		}
	}


	// Widget ���
	m_DepthDisable->SetState();

	m_AlphaBlend->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	// ������ ����Ÿ���� ����Ѵ�.
	// CResourceManager::GetInst()->RenderTarget();

	// ���콺 ���
	CWidgetWindow* MouseWidget = CEngine::GetInst()->GetMouseWidget();

	if (MouseWidget)
		MouseWidget->Render();

	m_AlphaBlend->ResetState();

	m_DepthDisable->ResetState();
}

void CRenderManager::RenderGBuffer()
{
	// ���� ����۷� ����Ÿ���� �����Ǿ� �ִ�.
	// �̸� GBuffer ����Ÿ������ ��ü�ؾ� �Ѵ�.
	std::vector<ID3D11RenderTargetView*>	vecTarget;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t	GBufferSize = m_vecGBuffer.size();

	vecPrevTarget.resize(GBufferSize);

	for (size_t i = 0; i < GBufferSize; ++i)
	{
		m_vecGBuffer[i]->ClearTarget();
		vecTarget.push_back(m_vecGBuffer[i]->GetTargetView());
	}

	// ���� �����Ǿ� �ִ� ����Ÿ�ٰ� ����Ÿ���� ���´�.
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)GBufferSize,
		&vecPrevTarget[0], &PrevDepthTarget);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize,
		&vecTarget[0], PrevDepthTarget);

	for (size_t i = 0; i <= 1; ++i)
	{
		auto	iter = m_RenderLayerList[i]->RenderList.begin();
		auto	iterEnd = m_RenderLayerList[i]->RenderList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Render();
		}
	}

	RenderDefaultInstancing();

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize,
		&vecPrevTarget[0], PrevDepthTarget);

	SAFE_RELEASE(PrevDepthTarget);
	for (size_t i = 0; i < GBufferSize; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[i]);
	}
}

void CRenderManager::RenderDecal()
{
	std::vector<ID3D11RenderTargetView*>	vecTarget;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t	DecalSize = m_vecDecal.size();

	vecPrevTarget.resize(DecalSize);

	for (size_t i = 0; i < DecalSize; ++i)
	{
		vecTarget.push_back(m_vecDecal[i]->GetTargetView());
	}

	// ���� �����Ǿ� �ִ� ����Ÿ�ٰ� ����Ÿ���� ���´�.
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)DecalSize,
		&vecPrevTarget[0], &PrevDepthTarget);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)DecalSize,
		&vecTarget[0], PrevDepthTarget);

	m_vecGBuffer[2]->SetTargetShader(10);
	m_vecGBuffer[4]->SetTargetShader(11);
	m_vecGBuffer[5]->SetTargetShader(12);

	auto	iter = m_RenderLayerList[2]->RenderList.begin();
	auto	iterEnd = m_RenderLayerList[2]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render();
	}

	m_vecGBuffer[2]->ResetTargetShader(10);
	m_vecGBuffer[4]->ResetTargetShader(11);
	m_vecGBuffer[5]->ResetTargetShader(12);


	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)DecalSize,
		&vecPrevTarget[0], PrevDepthTarget);

	SAFE_RELEASE(PrevDepthTarget);
	for (size_t i = 0; i < DecalSize; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[i]);
	}

	// ����� ���
#ifdef _DEBUG

	std::vector<ID3D11RenderTargetView*>	vecTarget1;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget1;
	ID3D11DepthStencilView* PrevDepthTarget1 = nullptr;

	size_t	GBufferSize = m_vecGBuffer.size();

	vecPrevTarget1.resize(GBufferSize);

	for (size_t i = 0; i < GBufferSize; ++i)
	{
		vecTarget1.push_back(m_vecGBuffer[i]->GetTargetView());
	}

	// ���� �����Ǿ� �ִ� ����Ÿ�ٰ� ����Ÿ���� ���´�.
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)GBufferSize,
		&vecPrevTarget1[0], &PrevDepthTarget1);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize,
		&vecTarget1[0], PrevDepthTarget1);

	iter = m_RenderLayerList[2]->RenderList.begin();
	iterEnd = m_RenderLayerList[2]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->RenderDebug();
	}

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize,
		&vecPrevTarget1[0], PrevDepthTarget1);

	SAFE_RELEASE(PrevDepthTarget1);
	for (size_t i = 0; i < GBufferSize; ++i)
	{
		SAFE_RELEASE(vecPrevTarget1[i]);
	}

#endif // _DEBUG
}

void CRenderManager::RenderLightAcc()
{
	std::vector<ID3D11RenderTargetView*>	vecTarget;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t	LightBufferSize = m_vecLightBuffer.size();

	vecPrevTarget.resize(LightBufferSize);

	for (size_t i = 0; i < LightBufferSize; ++i)
	{
		m_vecLightBuffer[i]->ClearTarget();
		vecTarget.push_back(m_vecLightBuffer[i]->GetTargetView());
	}

	// ���� �����Ǿ� �ִ� ����Ÿ�ٰ� ����Ÿ���� ���´�.
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)LightBufferSize,
		&vecPrevTarget[0], &PrevDepthTarget);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)LightBufferSize,
		&vecTarget[0], PrevDepthTarget);


	m_LightAccBlend->SetState();

	m_DepthDisable->SetState();


	m_vecGBuffer[0]->SetTargetShader(14);
	m_vecGBuffer[1]->SetTargetShader(15);
	m_vecGBuffer[2]->SetTargetShader(16);
	m_vecGBuffer[3]->SetTargetShader(17);

	CSceneManager::GetInst()->GetScene()->GetLightManager()->Render();

	m_vecGBuffer[0]->ResetTargetShader(14);
	m_vecGBuffer[1]->ResetTargetShader(15);
	m_vecGBuffer[2]->ResetTargetShader(16);
	m_vecGBuffer[3]->ResetTargetShader(17);

	m_DepthDisable->ResetState();

	m_LightAccBlend->ResetState();


	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)LightBufferSize,
		&vecPrevTarget[0], PrevDepthTarget);

	SAFE_RELEASE(PrevDepthTarget);
	for (size_t i = 0; i < LightBufferSize; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[i]);
	}
}

void CRenderManager::RenderLightBlend()
{
	CRenderTarget* FinalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");

	// Light ȥ���� �� ������, RenderTarget �� �ٲ㼭 ó���Ѵ�.
	FinalScreenTarget->ClearTarget();

	FinalScreenTarget->SetTarget(nullptr);

	m_vecGBuffer[0]->SetTargetShader(14);
	m_vecLightBuffer[0]->SetTargetShader(18);
	m_vecLightBuffer[1]->SetTargetShader(19);
	m_vecLightBuffer[2]->SetTargetShader(20);

	m_LightBlendShader->SetShader();

	m_DepthDisable->SetState();

	// �Ʒ��ڵ带 ����� ������, Null Buffer �� ����ϱ� �����̴�.
	UINT Offset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	CDevice::GetInst()->GetContext()->Draw(4, 0);


	m_DepthDisable->ResetState();

	m_vecGBuffer[0]->ResetTargetShader(14);
	m_vecLightBuffer[0]->ResetTargetShader(18);
	m_vecLightBuffer[1]->ResetTargetShader(19);
	m_vecLightBuffer[2]->ResetTargetShader(20);

	FinalScreenTarget->ResetTarget();
}

void CRenderManager::RenderFinalScreen()
{
	CRenderTarget* FinalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");

	FinalScreenTarget->SetTargetShader(21);

	// RenderLightBlend �� �޸�, ���⼭�� Render Target �� �������� �ʴ´�.
	// ������ Back Buffer �� �׷����� �����̴�.

	m_LightBlendRenderShader->SetShader();

	m_DepthDisable->SetState();

	// ������ �׸��� ���� Null Buffer �� ����Ͽ� �׷�����.
	UINT Offset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_DepthDisable->ResetState();

	FinalScreenTarget->ResetTargetShader(21);
}

void CRenderManager::RenderAnimationEditor()
{
	int AnimationEditorLayerIdx = GetRenderLayerIndex("AnimationEditorLayer");

	// ��~�࿡ �ش� Layer �� Idx �� ������ ���� �ʴٸ�
	if (AnimationEditorLayerIdx == -1)
		return;

	// Animation Edtior �󿡼� Animation Editor ���� ������ �ʴٸ�
	if (m_RenderLayerList[AnimationEditorLayerIdx]->RenderList.size() <= 0)
		return;

	// Render Target ��ü
	m_AnimEditorRenderTarget->ClearTarget();

	m_AnimEditorRenderTarget->SetTarget(nullptr);

	m_Mesh3DNoLightRenderShader->SetShader();

	// m_DepthDisable->SetState();

	// m_AnimationRenderTarget->SetTargetShader(55);

	auto iter = m_RenderLayerList[AnimationEditorLayerIdx]->RenderList.begin();
	auto iterEnd = m_RenderLayerList[AnimationEditorLayerIdx]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->RenderAnimationEditor();
	}

	m_AnimEditorRenderTarget->ResetTarget();
 }

void CRenderManager::RenderParticleEffectEditor()
{
	int ParticleEffectEditorLayerIdx = GetRenderLayerIndex("ParticleEffectRenderTarget");

	// ��~�࿡ �ش� Layer �� Idx �� ������ ���� �ʴٸ�
	if (ParticleEffectEditorLayerIdx == -1)
		return;

	// Animation Edtior �󿡼� Animation Editor ���� ������ �ʴٸ�
	if (m_RenderLayerList[ParticleEffectEditorLayerIdx]->RenderList.size() <= 0)
		return;

	// Render Target ��ü
	m_ParticleEffectEditorRenderTarget->ClearTarget();

	m_ParticleEffectEditorRenderTarget->SetTarget(nullptr);

	auto iter = m_RenderLayerList[ParticleEffectEditorLayerIdx]->RenderList.begin();
	auto iterEnd = m_RenderLayerList[ParticleEffectEditorLayerIdx]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->RenderParticleEffectEditor();
	}


	m_ParticleEffectEditorRenderTarget->ResetTarget();
}

void CRenderManager::SetBlendFactor(const std::string& Name, float r, float g,
	float b, float a)
{
	m_RenderStateManager->SetBlendFactor(Name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& Name, bool BlendEnable,
	D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, 
	D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha, 
	D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	m_RenderStateManager->AddBlendInfo(Name, BlendEnable, SrcBlend, DestBlend,
		BlendOp, SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& Name, 
	bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(Name, AlphaToCoverageEnable, IndependentBlendEnable);
}

int CRenderManager::GetRenderLayerIndex(const std::string& TargetName)
{
	for (size_t j = 0; j < m_RenderLayerList.size(); ++j)
	{
		if (m_RenderLayerList[j]->Name == TargetName)
			return j;
	}

	return -1;
}

CRenderState* CRenderManager::FindRenderState(const std::string& Name)
{
	return m_RenderStateManager->FindRenderState(Name);
}

void CRenderManager::RenderDefaultInstancing()
{
	for (int i = 0; i < m_RenderLayerList[1]->InstancingIndex; ++i)
	{
		// Material Slot ����ŭ �ݺ��Ѵ�.
		int	SlotCount = 0;
		if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Static)
		{
			SlotCount = ((CStaticMeshComponent*)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		else if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Animation)
		{
			SlotCount = ((CAnimationMeshComponent*)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		for (int j = 0; j < SlotCount; ++j)
		{
			auto	iter = m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.begin();
			auto	iterEnd = m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.end();

			std::vector<Instancing3DInfo>	vecInfo;
			vecInfo.reserve(m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.size());

			CMaterial* Material = nullptr;

			for (; iter != iterEnd; ++iter)
			{
				if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Static)
				{
					Material = ((CStaticMeshComponent*)(*iter))->GetMaterial(j);
				}

				else if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Animation)
				{
					Material = ((CAnimationMeshComponent*)(*iter))->GetMaterial(j);
				}

				Instancing3DInfo	Info = {};

				(*iter)->SetInstancingInfo(&Info);
				Material->GetCBuffer()->SetInstancingInfo(&Info);

				// �����۹� ����
				if (Info.MtrlPaperBurnEnable == 1)
				{
					CPaperBurnComponent* PaperBurn = (*iter)->GetGameObject()->FindComponentFromType<CPaperBurnComponent>();
					if (PaperBurn)
					{
						PaperBurn->SetInstancingInfo(&Info);
					}
				}

				vecInfo.push_back(Info);
			}

			if (Material)
				Material->RenderTexture();

			if (m_RenderLayerList[1]->m_vecInstancing[i]->Animation)
			{
				((CAnimationMesh*)m_RenderLayerList[1]->m_vecInstancing[i]->Mesh)->SetBoneShader();
			}

			m_Standard3DInstancingShader->SetShader();

			m_RenderLayerList[1]->m_vecInstancing[i]->CBuffer->UpdateCBuffer();

			m_RenderLayerList[1]->m_vecInstancing[i]->Buffer->UpdateBuffer(&vecInfo[0],
				(int)vecInfo.size());

			m_RenderLayerList[1]->m_vecInstancing[i]->Buffer->SetShader();

			m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->RenderInstancing((int)vecInfo.size(), j);


			m_RenderLayerList[1]->m_vecInstancing[i]->Buffer->ResetShader();



			if (m_RenderLayerList[1]->m_vecInstancing[i]->Animation)
			{
				((CAnimationMesh*)m_RenderLayerList[1]->m_vecInstancing[i]->Mesh)->ResetBoneShader();
			}


			if (Material)
				Material->Reset();

		}
	}
}

bool CRenderManager::Sortlayer(RenderLayer* Src, RenderLayer* Dest)
{
	return Src->LayerPriority < Dest->LayerPriority;
}
