#include "pch.h"
#include "CEyeSpace.h"
#include "mymath.h"
#include "CViewportProperty.h"
#include "etc.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace mymath;

void CEyeSpace::moveHorizontally(int moveX) {

	float moveXf = moveX / 10.0f;
	AddPos( moveXf, 0.0f, 0.0f );

}

void CEyeSpace::moveVertically(int moveY) {

	float moveYf = moveY / 10.0f;
	AddPos( 0.0f, moveYf, 0.0f);

}

CViewportProperty CEyeSpace::getViewportProperty() {

	CViewportProperty viewportProperty;

	viewportProperty.SetViewportTranslationX(GetPosX());
	viewportProperty.SetViewportTranslationY(GetPosY());
	viewportProperty.SetViewportTranslationZ(GetPosZ());

	return viewportProperty;
}

Matrixf4x4 CEyeSpace::getProjectionMatrix(CMy3DObjViewerDoc* pDoc, CDeviceContext * pDevCon ) {

	CViewportProperty viewportTranslation = pDoc->getViewportProperty();

	if (true == m_pMyEnginePropertiesWnd->getViewportPropertyUpdateFlag()) {

		float translateX = viewportTranslation.GetViewportTranslationX();
		float translateY = viewportTranslation.GetViewportTranslationY();
		float translateZ = viewportTranslation.GetViewportTranslationZ();

		SetPosX(translateX);
		SetPosY(translateY);
		SetPosZ(translateZ);

		float rotationX = viewportTranslation.GetViewportRotationX();
		float rotationY = viewportTranslation.GetViewportRotationY();
		float rotationZ = viewportTranslation.GetViewportRotationZ();
		SetRotateX(rotationX);
		SetRotateY(rotationY);
		SetRotateZ(rotationZ);

		m_pMyEnginePropertiesWnd->clearViewportPropertyUpdateFlag();

	}

	Matrixf4x4 translateMatrix = GetTranslateMatrix4x4Rev();
	Matrixf4x4 rotateMatrix = GetRotateMatrix4x4Rev();

	//
	// Construct perspective matrix.
	//

	float left = viewportTranslation.GetPerspectiveFrustumLeft();
	float right = viewportTranslation.GetPerspectiveFrustumRight();
	float top = viewportTranslation.GetPerspectiveFrustumTop();
	float bottom = viewportTranslation.GetPerspectiveFrustumBottom();
	float nearPlane = viewportTranslation.GetPerspectiveFrustumNear();
	float farPlane = viewportTranslation.GetPerspectiveFrustumFar();

	float horizontalFov = viewportTranslation.GetHorizontalFov();
	float verticalFov = viewportTranslation.GetVerticalFov();

	RECT rect;
	pDevCon->GetDC()->GetWindow()->GetClientRect(&rect);

	float zoomX = 1.0f / tan(horizontalFov * (M_PI / 180.0f) / 2.0f);
	float zoomY = zoomX * (float(rect.right - rect.left) / float(rect.bottom - rect.top));
	Matrixf4x4 projection_matrix(frustum(zoomX, zoomY, left, right, bottom, top, nearPlane, farPlane));

	Matrixf4x4 combinedProjectionMatrix = rotateMatrix * translateMatrix * projection_matrix;


	return combinedProjectionMatrix;
}

Matrixf4x4 CEyeSpace::getOnlyProjectionMatrix(CMy3DObjViewerDoc* pDoc, CDeviceContext* pDevCon) {

	CViewportProperty viewportTranslation = pDoc->getViewportProperty();

	if (true == m_pMyEnginePropertiesWnd->getViewportPropertyUpdateFlag()) {

		float translateX = viewportTranslation.GetViewportTranslationX();
		float translateY = viewportTranslation.GetViewportTranslationY();
		float translateZ = viewportTranslation.GetViewportTranslationZ();

		SetPosX(translateX);
		SetPosY(translateY);
		SetPosZ(translateZ);

		float rotationX = viewportTranslation.GetViewportRotationX();
		float rotationY = viewportTranslation.GetViewportRotationY();
		float rotationZ = viewportTranslation.GetViewportRotationZ();
		SetRotateX(rotationX);
		SetRotateY(rotationY);
		SetRotateZ(rotationZ);

		m_pMyEnginePropertiesWnd->clearViewportPropertyUpdateFlag();

	}

	//
	// Construct perspective matrix.
	//

	float left = viewportTranslation.GetPerspectiveFrustumLeft();
	float right = viewportTranslation.GetPerspectiveFrustumRight();
	float top = viewportTranslation.GetPerspectiveFrustumTop();
	float bottom = viewportTranslation.GetPerspectiveFrustumBottom();
	float nearPlane = viewportTranslation.GetPerspectiveFrustumNear();
	float farPlane = viewportTranslation.GetPerspectiveFrustumFar();

	float horizontalFov = viewportTranslation.GetHorizontalFov();
	float verticalFov = viewportTranslation.GetVerticalFov();

	RECT rect;
	pDevCon->GetDC()->GetWindow()->GetClientRect(&rect);

	float zoomX = 1.0f / tan(horizontalFov * (M_PI / 180.0f) / 2.0f);
	float zoomY = zoomX * (float(rect.right - rect.left) / float(rect.bottom - rect.top));
	Matrixf4x4 projection_matrix(frustum_nozoom(zoomX, zoomY, left, right, bottom, top, nearPlane, farPlane));

	Matrixf4x4 combinedProjectionMatrix = projection_matrix;


	return combinedProjectionMatrix;
}

Matrixf4x4 CEyeSpace::getOnlyTranslationRotationScalingMatrix(CMy3DObjViewerDoc* pDoc, CDeviceContext* pDevCon) {

	CViewportProperty viewportTranslation = pDoc->getViewportProperty();

	if (true == m_pMyEnginePropertiesWnd->getViewportPropertyUpdateFlag()) {

		float translateX = viewportTranslation.GetViewportTranslationX();
		float translateY = viewportTranslation.GetViewportTranslationY();
		float translateZ = viewportTranslation.GetViewportTranslationZ();

		SetPosX(translateX);
		SetPosY(translateY);
		SetPosZ(translateZ);

		float rotationX = viewportTranslation.GetViewportRotationX();
		float rotationY = viewportTranslation.GetViewportRotationY();
		float rotationZ = viewportTranslation.GetViewportRotationZ();
		SetRotateX(rotationX);
		SetRotateY(rotationY);
		SetRotateZ(rotationZ);

		m_pMyEnginePropertiesWnd->clearViewportPropertyUpdateFlag();

	}

	Matrixf4x4 translateMatrix = GetTranslateMatrix4x4Rev();
	Matrixf4x4 rotateMatrix = GetRotateMatrix4x4();

//	Matrixf4x4 combinedProjectionMatrix = rotateMatrix * translateMatrix * projection_matrix;
	Matrixf4x4 combinedProjectionMatrix = translateMatrix * rotateMatrix;


	return combinedProjectionMatrix;
}

Matrixf4x4 CEyeSpace::getOnlyTranslationRotationScalingMatrixRev(CMy3DObjViewerDoc* pDoc, CDeviceContext* pDevCon) {

	CViewportProperty viewportTranslation = pDoc->getViewportProperty();

	if (true == m_pMyEnginePropertiesWnd->getViewportPropertyUpdateFlag()) {

		float translateX = viewportTranslation.GetViewportTranslationX();
		float translateY = viewportTranslation.GetViewportTranslationY();
		float translateZ = viewportTranslation.GetViewportTranslationZ();

		SetPosX(translateX);
		SetPosY(translateY);
		SetPosZ(translateZ);

		float rotationX = viewportTranslation.GetViewportRotationX();
		float rotationY = viewportTranslation.GetViewportRotationY();
		float rotationZ = viewportTranslation.GetViewportRotationZ();
		SetRotateX(rotationX);
		SetRotateY(rotationY);
		SetRotateZ(rotationZ);

		m_pMyEnginePropertiesWnd->clearViewportPropertyUpdateFlag();

	}

	Matrixf4x4 translateMatrixRev = GetTranslateMatrix4x4Rev();
	Matrixf4x4 rotateMatrixRev = GetRotateMatrix4x4Rev();

	//	Matrixf4x4 combinedProjectionMatrix = rotateMatrix * translateMatrix * projection_matrix;
	Matrixf4x4 combinedProjectionMatrix = translateMatrixRev * rotateMatrixRev;


	return combinedProjectionMatrix;
}

Vectorf3 CEyeSpace::GetTransformInW(float moveX, float moveY, float moveZ)
{
	Vectorf4 moveInE{ moveX, moveY, moveZ, 0 };
	Matrixf4x4 rotMat = GetRotateMatrix4x4();

	Vectorf4 moveInW = moveInE * rotMat;
	return moveInW.homogeniousDivide();
}

Matrixf3x3 CEyeSpace::GetRotationInW( float rotX, float rotY, float rotZ )
{
	Matrixf4x4 rotMatInE = GetRotateMatrix4x4(rotX, rotY, rotZ);
	Matrixf4x4 rotMatEtoW = GetRotateMatrix4x4();

	Matrixf4x4 rotMat = rotMatInE * rotMatEtoW;
	return rotMat.homogeniousDivide();
}
