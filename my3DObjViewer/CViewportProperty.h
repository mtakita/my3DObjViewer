#pragma once

#include "mymath.h"

using mymath::Vectorf4;

class CViewportProperty
{
public:
	CViewportProperty() :
		viewportTranslation{},
		viewportRotation{},
		horizontalFov{0.0f},
		verticalFov{ 0.0f },
		perspectiveFrustumLeft{ 0.0f },
		perspectiveFrustumRight{ 0.0f },
		perspectiveFrustumTop{ 0.0f },
		perspectiveFrustumBottom{ 0.0f },
		perspectiveFrustumNear{ 0.0f },
		perspectiveFrustumFar{ 0.0f }
	{
	}

	// Translation.
	float GetViewportTranslationX()  { return viewportTranslation[0]; }
	float GetViewportTranslationY()  { return viewportTranslation[1]; }
	float GetViewportTranslationZ()  { return viewportTranslation[2]; }
	void SetViewportTranslationX(float x) { viewportTranslation[0] = x; }
	void SetViewportTranslationY(float y) { viewportTranslation[1] = y; }
	void SetViewportTranslationZ(float z) { viewportTranslation[2] = z; }

	// Rotation.
	float GetViewportRotationX() { return viewportRotation[0]; }
	void SetViewportRotationX(float x) { viewportRotation[0] = x; }

	float GetViewportRotationY() { return viewportRotation[1]; }
	void SetViewportRotationY(float y) { viewportRotation[1] = y; }

	float GetViewportRotationZ() { return viewportRotation[2]; }
	void SetViewportRotationZ(float z) { viewportRotation[2] = z; }

	float GetHorizontalFov() { return horizontalFov; }
	float GetVerticalFov() { return verticalFov; }
	void SetHorizontalFov(float x) { horizontalFov = x; }
	void SetVerticalFov(float x) { verticalFov = x; }

	float GetPerspectiveFrustumLeft() { return perspectiveFrustumLeft; }
	void SetPerspectiveFrustumLeft(float x) { perspectiveFrustumLeft = x; }
	float GetPerspectiveFrustumRight() { return perspectiveFrustumRight; }
	void SetPerspectiveFrustumRight(float x) { perspectiveFrustumRight = x; }
	float GetPerspectiveFrustumTop() { return perspectiveFrustumTop; }
	void SetPerspectiveFrustumTop(float x) { perspectiveFrustumTop = x; }
	float GetPerspectiveFrustumBottom() { return perspectiveFrustumBottom; }
	void SetPerspectiveFrustumBottom(float x) { perspectiveFrustumBottom = x; }
	float GetPerspectiveFrustumNear() { return perspectiveFrustumNear; }
	void SetPerspectiveFrustumNear(float x) { perspectiveFrustumNear = x; }
	float GetPerspectiveFrustumFar() { return perspectiveFrustumFar; }
	void SetPerspectiveFrustumFar(float x) { perspectiveFrustumFar = x; }
private:
	Vectorf4 viewportTranslation;
	Vectorf4 viewportRotation;

	float horizontalFov;
	float verticalFov;

	float perspectiveFrustumLeft;
	float perspectiveFrustumRight;
	float perspectiveFrustumTop;
	float perspectiveFrustumBottom;
	float perspectiveFrustumNear;
	float perspectiveFrustumFar;

};
