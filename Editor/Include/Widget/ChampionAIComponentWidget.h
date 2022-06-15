#pragma once
#include "ObjectComponentWidget.h"

class CChampionAIComponentWidget :
    public CObjectComponentWidget
{
public:
    CChampionAIComponentWidget();
    virtual ~CChampionAIComponentWidget();

public:
    virtual bool Init() override;

public:
    // �ݵ�� Init ���� �� �Լ� ȣ���ؾ� ��
    void SetObjectComponent(class CObjectComponent* Com);

private:
    class CIMGUITree* m_RootTree;
    class CIMGUIComboBox* m_LineComboBox;
    class CIMGUIInputInt* m_DetectRangeInput;
    class CIMGUIText* m_InHome;

private:
    LoLLine m_Line;
    int m_DetectRange;
    bool m_InHome;

public:
    void OnChangeChampionLine(int Index, const char* Label);
    void OnChangeDetectRange(int Index, const char* Label);
    void OnChangeInHome(int Index, const char* Label);
};

