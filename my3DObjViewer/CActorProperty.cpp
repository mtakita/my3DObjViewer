#include "pch.h"
#include "CActorProperty.h"
#include "CActor.h"

void CActorProperty::SetActorProperty(CActor* inpMyActor) {

	// Translation.
	float translationX = inpMyActor->GetPosX();
	float translationY = inpMyActor->GetPosY();
	float translationZ = inpMyActor->GetPosZ();

	// Rotation.
	float rotationX = inpMyActor->GetRotateX();
	float rotationY = inpMyActor->GetRotateY();
	float rotationZ = inpMyActor->GetRotateZ();

	// Scale k
	float scale_k = inpMyActor->GetScale();

	// Tessellation On/Off
	bool tessellationOnOff = inpMyActor->GetTessellationOnOff();

	// Tessellation Domain
	int tessellationDomain = inpMyActor->GetTessellationDomain();

	// Tessellation Factor Outer 0-3
	int tessellationFactorOuter0 = inpMyActor->GetTessellationLevelFactorOuter0();
	int tessellationFactorOuter1 = inpMyActor->GetTessellationLevelFactorOuter1();
	int tessellationFactorOuter2 = inpMyActor->GetTessellationLevelFactorOuter2();
	int tessellationFactorOuter3 = inpMyActor->GetTessellationLevelFactorOuter3();

	// Tessellation Factor Inner 0-1
	int tessellationFactorInner0 = inpMyActor->GetTessellationLevelFactorInner0();
	int tessellationFactorInner1 = inpMyActor->GetTessellationLevelFactorInner1();

	bool geometryOnOff = inpMyActor->GetFurGeometryOnOff();
	int furLayer = inpMyActor->GetFurLayer();
	float furDepth = inpMyActor->GetFurDepth();

	SetActorTranslationX(translationX);
	SetActorTranslationY(translationY);
	SetActorTranslationZ(translationZ);

	SetActorRotationX(rotationX);
	SetActorRotationY(rotationY);
	SetActorRotationZ(rotationZ);

	SetActorScaleK(scale_k);

	SetTessellationOnOff(tessellationOnOff);

	SetTessellationDomain(tessellationDomain);

	SetTessellationLevelFactorOuter0(tessellationFactorOuter0);
	SetTessellationLevelFactorOuter1(tessellationFactorOuter1);
	SetTessellationLevelFactorOuter2(tessellationFactorOuter2);
	SetTessellationLevelFactorOuter3(tessellationFactorOuter3);

	SetTessellationLevelFactorInner0(tessellationFactorInner0);
	SetTessellationLevelFactorInner1(tessellationFactorInner1);

	SetGeometryOnOff(geometryOnOff);
	SetGeometryLevelFactorFurLayer(furLayer);
	SetGeometryLevelFactorFurDepth(furDepth);

}

// Translation.
float CActorProperty::GetActorTranslationX() { return actorTranslation[0]; }
float CActorProperty::GetActorTranslationY() { return actorTranslation[1]; }
float CActorProperty::GetActorTranslationZ() { return actorTranslation[2]; }
void CActorProperty::SetActorTranslationX(float x) { actorTranslation[0] = x; }
void CActorProperty::SetActorTranslationY(float y) { actorTranslation[1] = y; }
void CActorProperty::SetActorTranslationZ(float z) { actorTranslation[2] = z; }

// Rotation.
float CActorProperty::GetActorRotationX() { return actorRotation[0]; }
float CActorProperty::GetActorRotationY() { return actorRotation[1]; }
float CActorProperty::GetActorRotationZ() { return actorRotation[2]; }
void CActorProperty::SetActorRotationX(float x) { actorRotation[0] = x; }
void CActorProperty::SetActorRotationY(float y) { actorRotation[1] = y; }
void CActorProperty::SetActorRotationZ(float z) { actorRotation[2] = z; }

// Scale
float CActorProperty::GetActorScaleK() { return scale_k; }
void CActorProperty::SetActorScaleK(float k) {
	scale_k = k;
}

void CActorProperty::SetTessellationOnOff(bool inTessellationOnOff) {
	m_TessellationOnOff = inTessellationOnOff;
}
void CActorProperty::SetTessellationDomain(int inTessellationDomain) {
	m_TessellationDomain = inTessellationDomain;
}
void CActorProperty::SetTessellationLevelFactorOuter0(int inTessellationFactorOuter0) {
	m_TessellationFactorOuter0 = inTessellationFactorOuter0;
}
void CActorProperty::SetTessellationLevelFactorOuter1(int inTessellationFactorOuter1) {
	m_TessellationFactorOuter1 = inTessellationFactorOuter1;
}
void CActorProperty::SetTessellationLevelFactorOuter2(int inTessellationFactorOuter2) {
	m_TessellationFactorOuter2 = inTessellationFactorOuter2;
}
void CActorProperty::SetTessellationLevelFactorOuter3(int inTessellationFactorOuter3) {
	m_TessellationFactorOuter3 = inTessellationFactorOuter3;
}
void CActorProperty::SetTessellationLevelFactorInner0(int inTessellationFactorInner0) {
	m_TessellationFactorInner0 = inTessellationFactorInner0;
}
void CActorProperty::SetTessellationLevelFactorInner1(int inTessellationFactorInner1) {
	m_TessellationFactorInner1 = inTessellationFactorInner1;
}

bool CActorProperty::GetTessellationOnOff() {
	return m_TessellationOnOff;
}
int CActorProperty::GetTessellationDomain() {
	return m_TessellationDomain;
}
int CActorProperty::GetTessellationLevelFactorOuter0() {
	return m_TessellationFactorOuter0;
}
int CActorProperty::GetTessellationLevelFactorOuter1() {
	return m_TessellationFactorOuter1;
}
int CActorProperty::GetTessellationLevelFactorOuter2() {
	return m_TessellationFactorOuter2;
}
int CActorProperty::GetTessellationLevelFactorOuter3() {
	return m_TessellationFactorOuter3;
}
int CActorProperty::GetTessellationLevelFactorInner0() {
	return m_TessellationFactorInner0;
}
int CActorProperty::GetTessellationLevelFactorInner1() {
	return m_TessellationFactorInner1;
}

void CActorProperty::SetGeometryOnOff(bool inGeometryOnOff) {
	m_GeometryOnOff = inGeometryOnOff;
}

bool CActorProperty::GetGeometryOnOff() {
	return m_GeometryOnOff;
}
void CActorProperty::SetGeometryOnOff(int inGeometryOnOff) {
	m_GeometryOnOff = inGeometryOnOff;
}

int CActorProperty::GetGeometryLevelFactorFurLayer() {
	return m_GeometryLevelFactorFurLayer;
}
void CActorProperty::SetGeometryLevelFactorFurLayer(int inGeometryLevelFactorFurLayer) {
	m_GeometryLevelFactorFurLayer = inGeometryLevelFactorFurLayer;
}

float CActorProperty::GetGeometryLevelFactorFurDepth() {
	return m_GeometryLevelFactorFurDepth;
}

void CActorProperty::SetGeometryLevelFactorFurDepth(float inGeometryLevelFactorFurDepth) {
	m_GeometryLevelFactorFurDepth = inGeometryLevelFactorFurDepth;
}
