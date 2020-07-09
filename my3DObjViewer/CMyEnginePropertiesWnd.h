#pragma once
#include <afxdockablepane.h>
#include "mymath.h"
#include "CViewportProperty.h"
#include "CMyMFCPropertyGridCtrl.h"

using mymath::Vectorf4;

class CMyEnginePropertiesWnd :
	public CDockablePane
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	CMyEnginePropertiesWnd() :
		m_fntPropList{},
		m_wndViewportPropList{ &m_viewportPropertyUpdatedFlag },
		idxViewport{},
		idxTranslation{},
		idxRotation{},
		idxXRotation{},
		idxYRotation{},
		idxZRotation{},
		idxTranslationX{},
		idxTranslationY{},
		idxTranslationZ{},
		m_wndPerspectiveTransformPropList{},
		idxPerspectiveTransform{},
		idxFov{},
		idxHorizontalFov{},
		idxVerticalFov{},
		idxFrustum{},
		idxFrustumLeft{},
		idxFrustumRight{},
		idxFrustumTop{},
		idxFrustumBottom{},
		idxFrustumNear{},
		idxFrustumFar{},
		m_viewportPropertyUpdatedFlag{ false } 
	{
	}

protected:
	CFont m_fntPropList;

	// Properties for viewport.
//	CMFCPropertyGridCtrl m_wndViewportPropList;
	CMyMFCPropertyGridCtrl m_wndViewportPropList;

	// Index for viewport properties.
	int idxViewport;

	// Index for subitem.
	int idxTranslation;
	int idxRotation;
	int idxXRotation;
	int idxYRotation;
	int idxZRotation;

	// idxTranslation.
	int idxTranslationX;
	int idxTranslationY;
	int idxTranslationZ;

	// Properties for perspective transformation.
	CMFCPropertyGridCtrl m_wndPerspectiveTransformPropList;

	// idxPerspectiveFov
	int idxPerspectiveTransform;

	// idxFov
	int idxFov;

	// idxFov
	int idxHorizontalFov;
	int idxVerticalFov;

	// idxFrustum
	int idxFrustum;

	// idxFrustumDimension
	int idxFrustumLeft;
	int idxFrustumRight;
	int idxFrustumTop;
	int idxFrustumBottom;
	int idxFrustumNear;
	int idxFrustumFar;

	bool m_viewportPropertyUpdatedFlag;

public:
	void InitViewportPropList();
	void InitPerspectiveTransformPropList();
	void SetPropListFont();

	void AdjustLayout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	CViewportProperty getViewportTransltion();
	void updateViewportProperty(CViewportProperty viewportProperty );

	bool getViewportPropertyUpdateFlag() {
		//		return m_wndActorPropList.getNeedPropertyUpdateFlag();
		return m_viewportPropertyUpdatedFlag;
	}
	void clearViewportPropertyUpdateFlag() {
		//		m_wndActorPropList.clearNeedPropertyUpdateFlag();
		m_viewportPropertyUpdatedFlag = false;
	}

	virtual void DoDataExchange(CDataExchange* pDX);
};

