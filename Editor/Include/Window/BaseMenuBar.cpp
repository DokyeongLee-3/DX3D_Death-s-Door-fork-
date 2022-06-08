#include "BaseMenuBar.h"
#include "IMGUIWidget.h"
#include "IMGUIBeginMenu.h"
#include "../Widget/SaveLoadBeginMenu.h"
#include "../Widget/BehaviorTreeMenu.h"

CBaseMenuBar::CBaseMenuBar()
{
}

CBaseMenuBar::~CBaseMenuBar()
{
	size_t Size = m_vecBeginMenuChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecBeginMenuChild[i]);
	}
}

bool CBaseMenuBar::Init()
{
	// "File" -> Name ���� �������� ����, ���� Menu Bar ���� �������� �̸��� �ȴ�.
	m_SaveLoadBeginMenu = AddWidget<CSaveLoadBeginMenu>("File");
	m_BehaviorTreeBeginMenu = AddWidget<CBehaviorTreeMenu>("BehaviorTree");

	return true;
}

void CBaseMenuBar::Update(float DeltaTime)
{
	if (ImGui::BeginMainMenuBar())
	{
		mSize = m_vecBeginMenuChild.size();

		for (size_t i = 0; i < mSize; ++i)
		{
			m_vecBeginMenuChild[i]->Render();
		}

		ImGui::EndMainMenuBar();
	}
}

