#pragma once

#include "IMGUIWindow.h"
#include "IMGUICollapsingHeader.h"
#include "IMGUIGizmo.h"
#include "IMGUIGrid.h"
#include "IMGUIRadioButton.h"
#include "IMGUISliderFloat.h"
#include "IMGUIColor3.h"

class CToolWindow :
    public CIMGUIWindow
{
public:
	CToolWindow();
	~CToolWindow();

public:
	virtual bool Init();

public:
	void SetGizmoObject(class CGameObject* Object);
	void SetGizmoComponent(class CSceneComponent* SceneComp);

public:
	// ����׿� �ӽ� Ű
	void OnQDown(float DetlaTime);
	void OnWDown(float DetlaTime);
	void OnEDown(float DetlaTime);

private:
	void OnSelectGizmoOperationMode(const char* Label, bool Check);
	void OnSelectGizmoTransformMode(const char* Label, bool Check);
	void OnChangeCameraSpeed(float Speed);
	void OnChangeOutlineThickness(float Val);
	void OnChangeOutlineDepthMultiply(float Val);
	void OnChangeOutlineDepthBias(float Val);
	void OnChangeOutlineNormalMultiply(float Val);
	void OnChangeOutlineNormalBias(float Val);
	void OnChangeOutlineColor(const Vector3& Color);

private:
	// Gizmo
	CIMGUICollapsingHeader* m_GizmoBlock;
	CIMGUIGizmo* m_Gizmo;
	CIMGUIGrid* m_Grid;
	CIMGUIRadioButton* m_GizmoTransformMode;
	CIMGUIRadioButton* m_GizmoOperationMode;

	// Camera
	CIMGUICollapsingHeader* m_EditorCameraBlock;
	CIMGUISliderFloat* m_CameraSpeed;

	// Render
	CIMGUICollapsingHeader* m_RenderBlock;
	CIMGUISliderFloat* m_OutlineThickness;
	CIMGUISliderFloat* m_OutlineDepthMultiply;
	CIMGUISliderFloat* m_OutlineDepthBias;
	CIMGUISliderFloat* m_OutlineNormalMutliply;
	CIMGUISliderFloat* m_OutlineNormalBias;
	CIMGUIColor3* m_OutlineColor;
};

