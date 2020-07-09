#pragma once

#ifndef _CEYESPACE_H_
#define _CEYESPACE_H_

#include "CGeometryBase.h"
#include "CViewportProperty.h"
#include "CMyEnginePropertiesWnd.h"
#include "CMy3DObjViewerDoc.h"
#include "CDeviceContext.h"

class CEyeSpace : public CGeometryBase
{
public:
	CEyeSpace(
		float wPosX, float wPosY, float wPosZ, 
		float xAxis, float yAxis, float zAxis) 
		: CGeometryBase{ 
		wPosX , wPosY , wPosZ,
		1.0f,
		xAxis, yAxis, zAxis }{
	}

	void moveHorizontally(int moveX );
	void moveVertically( int moveY );

	CViewportProperty getViewportProperty();

	CMyEnginePropertiesWnd* m_pMyEnginePropertiesWnd;

	void setMyEnginePropertiesWnd(CMyEnginePropertiesWnd* pMyEnginePropertiesWnd) {
		m_pMyEnginePropertiesWnd = pMyEnginePropertiesWnd;
	}

	Matrixf4x4 getProjectionMatrix(CMy3DObjViewerDoc* pDoc, CDeviceContext* pDevCon);
	Matrixf4x4 getOnlyTranslationRotationScalingMatrix(CMy3DObjViewerDoc* pDoc, CDeviceContext* pDevCon);
	Matrixf4x4 getOnlyTranslationRotationScalingMatrixRev(CMy3DObjViewerDoc* pDoc, CDeviceContext* pDevCon);
	Matrixf4x4 getOnlyProjectionMatrix(CMy3DObjViewerDoc* pDoc, CDeviceContext* pDevCon);

	Vectorf3 GetTransformInW(float moveX, float moveY, float moveZ);
	Matrixf3x3 GetRotationInW(float rotX, float rotY, float rotZ);

};

#endif
