#pragma once

#ifndef _CACTORPROPERTY_H_
#define _CACTORPROPERTY_H_

#include "mymath.h"

using namespace mymath;

class CActorProperty
{
public:
	// Translation.
	float GetActorTranslationX() { return actorTranslation[0]; }
	float GetActorTranslationY() { return actorTranslation[1]; }
	float GetActorTranslationZ() { return actorTranslation[2]; }
	void SetActorTranslationX(float x) { actorTranslation[0] = x; }
	void SetActorTranslationY(float y) { actorTranslation[1] = y; }
	void SetActorTranslationZ(float z) { actorTranslation[2] = z; }

	// Rotation.
	float GetActorRotationX() { return actorRotation[0]; }
	float GetActorRotationY() { return actorRotation[1]; }
	float GetActorRotationZ() { return actorRotation[2]; }
	void SetActorRotationX(float x) { actorRotation[0] = x; }
	void SetActorRotationY(float y) { actorRotation[1] = y; }
	void SetActorRotationZ(float z) { actorRotation[2] = z; }

	// Scale
	float GetActorScaleK() { return scale_k; }
	void SetActorScaleK(float k) {
		scale_k = k;
	}

	void SetTessellationOnOff(bool inTessellationOnOff) {
		m_TessellationOnOff = inTessellationOnOff;
	}
	void SetTessellationDomain(int inTessellationDomain) {
		m_TessellationDomain = inTessellationDomain;
	}
	void SetTessellationLevelFactorOuter0(int inTessellationFactorOuter0) {
		m_TessellationFactorOuter0 = inTessellationFactorOuter0;
	}
	void SetTessellationLevelFactorOuter1(int inTessellationFactorOuter1) {
		m_TessellationFactorOuter1 = inTessellationFactorOuter1;
	}
	void SetTessellationLevelFactorOuter2(int inTessellationFactorOuter2) {
		m_TessellationFactorOuter2 = inTessellationFactorOuter2;
	}
	void SetTessellationLevelFactorOuter3(int inTessellationFactorOuter3) {
		m_TessellationFactorOuter3 = inTessellationFactorOuter3;
	}
	void SetTessellationLevelFactorInner0(int inTessellationFactorInner0) {
		m_TessellationFactorInner0 = inTessellationFactorInner0;
	}
	void SetTessellationLevelFactorInner1(int inTessellationFactorInner1) {
		m_TessellationFactorInner1 = inTessellationFactorInner1;
	}

	bool GetTessellationOnOff() {
		return m_TessellationOnOff;
	}
	int GetTessellationDomain() {
		return m_TessellationDomain;
	}
	int GetTessellationLevelFactorOuter0() {
		return m_TessellationFactorOuter0;
	}
	int GetTessellationLevelFactorOuter1() {
		return m_TessellationFactorOuter1;
	}
	int GetTessellationLevelFactorOuter2() {
		return m_TessellationFactorOuter2;
	}
	int GetTessellationLevelFactorOuter3() {
		return m_TessellationFactorOuter3;
	}
	int GetTessellationLevelFactorInner0() {
		return m_TessellationFactorInner0;
	}
	int GetTessellationLevelFactorInner1() {
		return m_TessellationFactorInner1;
	}

private:
	Vectorf4 actorTranslation;
	Vectorf4 actorRotation;
	float scale_k;

	bool m_TessellationOnOff;
	int m_TessellationDomain;
	int m_TessellationFactorOuter0;
	int m_TessellationFactorOuter1;
	int m_TessellationFactorOuter2;
	int m_TessellationFactorOuter3;
	int m_TessellationFactorInner0;
	int m_TessellationFactorInner1;
};

#endif
