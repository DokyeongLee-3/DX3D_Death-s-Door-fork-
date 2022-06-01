#pragma once

#include "SceneComponentWidget.h"

class CAnimationMeshWidget :
    public CSceneComponentWidget
{
public:
    CAnimationMeshWidget();
    virtual ~CAnimationMeshWidget();

public:
    virtual bool Init() override;
    
public:
    virtual void SetSceneComponent(class CSceneComponent* Com) override;

private:
    void OnClickLoadMesh();
    void OnSelectMaterialSlotCombo(int Idx, const char* Label);
    void OnEditBaseColor(const Vector3& Color);
    void OnEditAmbientColor(const Vector3& Color);
    void OnEditSpecluarColor(const Vector3& Color);
    void OnEditSpecluarPower(float Power);
    void OnEditEmissiveColor(const Vector3& Color);
    void OnCheckTransparency(int Idx, bool Check);
    void OnEditOpacity(float Opacity);

private:
    void RefreshMeshWidget(class CMesh* Mesh);

private:
    class CIMGUITextInput* m_MeshName;
    class CIMGUIButton* m_LoadMeshButton;

    class CIMGUIComboBox* m_MaterialSlotCombo;
    class CIMGUIColor3* m_BaseColorEdit;
    class CIMGUIColor3* m_AmbientColorEdit;
    class CIMGUIColor3* m_SpecularColorEdit;
    class CIMGUIInputFloat* m_SpecluarPowerEdit;
    class CIMGUIColor3* m_EmissiveColorEdit;
    class CIMGUICheckBox* m_TransparencyEdit;
    class CIMGUISliderFloat* m_OpacityEdit;
};
