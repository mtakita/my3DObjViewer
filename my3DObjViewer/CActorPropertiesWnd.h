#pragma once

#ifndef _CACTORPROPERTIESWND_H_
#define _CACTORPROPERTIESWND_H_

#include <afxdockablepane.h>
#include "CActorProperty.h"
#include "CMyMFCPropertyGridCtrl.h"
#include "CStringPropertyHelper.h"

#define TESSELLATION_DOAMIN_QUAD     _T("Quad")
#define TESSELLATION_DOAMIN_ISOLINE  _T("Isoline")
#define TESSELLATION_DOAMIN_TRIANGLE _T("Triangle")

static WCHAR* tessellation_domain[]{
	TESSELLATION_DOAMIN_QUAD ,
	TESSELLATION_DOAMIN_ISOLINE,
	TESSELLATION_DOAMIN_TRIANGLE
};

class CActorPropertiesWnd :
	public CDockablePane
{
public:
	CActorPropertiesWnd() : 
		m_fntPropList{},
		m_wndActorPropList{ &m_propertyUpdatedFlag },
		idxActor{},
		idxTranslation{},
		idxRotation{},
		idxScale{},
		idxTranslationX{},
		idxTranslationY{},
		idxTranslationZ{},
		idxRotationX{},
		idxRotationY{},
		idxRotationZ{},
		idxScale_k{},
		CDockablePane {} 
	{
		m_propertyUpdatedFlag = false;
	}

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
	void AdjustLayout();
	void InitActorPropList();
	void SetPropListFont();
	void updateActorProperty(CActorProperty actorProperty);
	void getActorProperty(CActorProperty& actorProperty);

	CStringPropertyHelper m_tessellationDomainHelper = { tessellation_domain, sizeof(tessellation_domain) / sizeof(WCHAR*) };

	CFont m_fntPropList;

	// Properties for actor.
//	CMFCPropertyGridCtrl m_wndActorPropList;
	CMyMFCPropertyGridCtrl  m_wndActorPropList;

	// Index for actor properties.
	int idxActor;

	// Index for subitem.
	int idxTranslation;
	int idxRotation;
	int idxScale;
	int idxTessellation;
	int idxGeometry;

	// idxTranslation.
	int idxTranslationX;
	int idxTranslationY;
	int idxTranslationZ;

	// idxRotation
	int idxRotationX;
	int idxRotationY;
	int idxRotationZ;

	// idxScale
	int idxScale_k;

	// idxTessellation
	int idxTessellationFunc;
	int idxTessellationDomain;
	int idxTessellationLevelFactor;

	// idxTessellationFunc
	int idxTessellationFuncOnOff;

	// idxTessellationDomain
	int idxTessellationDomainQuadTriaLine;

	// idxTessellationLevelFactor
	int idxTessellationLevelFactorOuter0;
	int idxTessellationLevelFactorOuter1;
	int idxTessellationLevelFactorOuter2;
	int idxTessellationLevelFactorOuter3;
	int idxTessellationLevelFactorInner0;
	int idxTessellationLevelFactorInner1;

	// idxGeometry
	int idxGeometryFunc;
	int idxGeometryLevelFactor;

	// idxGeometryFunc
	int idxGeometryFuncOnOff;

	// idxGeometryLevelFactor
	int idxGeometryLevelFactorFurLayer;
	int idxGeometryLevelFactorFurDepth;

	bool getPropertyUpdateFlag() {
//		return m_wndActorPropList.getNeedPropertyUpdateFlag();
		return m_propertyUpdatedFlag;
	}
	void clearPropertyUpdateFlag() {
//		m_wndActorPropList.clearNeedPropertyUpdateFlag();
		m_propertyUpdatedFlag = false;
	}

	bool m_propertyUpdatedFlag;

	virtual void DoDataExchange(CDataExchange* pDX);

};

#endif
