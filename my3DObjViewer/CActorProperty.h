#pragma once

#ifndef _CACTORPROPERTY_H_
#define _CACTORPROPERTY_H_

#include "mymath.h"
#include "CActor.h"

using namespace mymath;

class CActor;

class CActorProperty
{
public:

	// set actor property.
	void SetActorProperty( CActor * inpMyActor );

	// Translation.
	float GetActorTranslationX();
	float GetActorTranslationY();
	float GetActorTranslationZ();
	void SetActorTranslationX(float x);
	void SetActorTranslationY(float y);
	void SetActorTranslationZ(float z);

	// Rotation.
	float GetActorRotationX();
	float GetActorRotationY();
	float GetActorRotationZ();
	void SetActorRotationX(float x);
	void SetActorRotationY(float y);
	void SetActorRotationZ(float z);

	// Scale
	float GetActorScaleK();
	void SetActorScaleK(float k);

	void SetTessellationOnOff(bool inTessellationOnOff);
	void SetTessellationDomain(int inTessellationDomain);
	void SetTessellationLevelFactorOuter0(int inTessellationFactorOuter0);
	void SetTessellationLevelFactorOuter1(int inTessellationFactorOuter1);
	void SetTessellationLevelFactorOuter2(int inTessellationFactorOuter2);
	void SetTessellationLevelFactorOuter3(int inTessellationFactorOuter3);
	void SetTessellationLevelFactorInner0(int inTessellationFactorInner0);
	void SetTessellationLevelFactorInner1(int inTessellationFactorInner1);

	bool GetTessellationOnOff();
	int GetTessellationDomain();
	int GetTessellationLevelFactorOuter0();
	int GetTessellationLevelFactorOuter1();
	int GetTessellationLevelFactorOuter2();
	int GetTessellationLevelFactorOuter3();
	int GetTessellationLevelFactorInner0();
	int GetTessellationLevelFactorInner1();

	void SetGeometryOnOff(bool inGeometryOnOff);

	bool GetGeometryOnOff();
	void SetGeometryOnOff(int inGeometryOnOff);

	int GetGeometryLevelFactorFurLayer();
	void SetGeometryLevelFactorFurLayer(int inGeometryLevelFactorFurLayer);
	float GetGeometryLevelFactorFurDepth();
	void SetGeometryLevelFactorFurDepth(float inGeometryLevelFactorFurDepth);

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

	bool m_GeometryOnOff;
	int m_GeometryLevelFactorFurLayer;
	float m_GeometryLevelFactorFurDepth;

};

#endif
