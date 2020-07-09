#include "pch.h"
#include "CActorAABB.h"

#include "geometryhelper.h"
#include "mymath.h"

using namespace mymath;

CActorAABB::CActorAABB() {

	m_ActorId = 0;

	m_xMin = 0;
	m_xMax = 0;

	m_yMin = 0;
	m_yMax = 0;

	m_zMin = 0;
	m_zMax = 0;

	m_ActorId = 0;
}


CActorAABB::CActorAABB(int actorId, CActor* pMyActor ) {

	init();	// initialize members.

	m_ActorId = actorId;
	m_pMyActor = pMyActor;

}

CActorAABB::CActorAABB( CActor* pMyActor, CActorAABB& aabb){

	m_xMin = aabb.m_xMin;
	m_xMax = aabb.m_xMax;

	m_yMin = aabb.m_yMin;
	m_yMax = aabb.m_yMax;

	m_zMin = aabb.m_zMin;
	m_zMax = aabb.m_zMax;

	m_ActorId = aabb.m_ActorId;
	m_pMyActor = pMyActor;
}

CActorAABB::CActorAABB( CActor* pMyActor, int actorId, float xMin, float yMin, float zMin, float xMax, float yMax, float zMax) {

	init();	// initialize members.

	m_ActorId = actorId;
	m_pMyActor = pMyActor;

	m_xMin = xMin;
	m_xMax = xMax;

	m_yMin = yMin;
	m_yMax = yMax;

	m_zMin = zMin;
	m_zMax = zMax;
}


CActorAABB::CActorAABB(CActor* pMyActor, float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {

	init();	// initialize members.

	m_xMin = xMin;
	m_xMax = xMax;

	m_yMin = yMin;
	m_yMax = yMax;

	m_zMin = zMin;
	m_zMax = zMax;
}

int CActorAABB::getActorId() {
	return m_ActorId;
}

void CActorAABB::init() {

	m_xMin = 0;
	m_xMax = 0;

	m_yMin = 0;
	m_yMax = 0;

	m_zMin = 0;
	m_zMax = 0;

	m_ActorId = 0;

}


bool CActorAABB::isActorPointed(Vectorf3 worldNearPlanePos, Vectorf3 worldNearPlaneOrigin, CDeviceContext& devCon, CViewportProperty& viewportTranslation, Vectorf3& outPointOfInersection){

	//
	// Repeat check intersection of eye direction with all triangles consisting AABB.
	// Triangle v1,v2,v3 should consist of vertices in COUNTER CLOCKWISE.
	// 

	//
	// Convert near plane coordinates to world space near plane coordinates.
	//
	float translateX = viewportTranslation.GetViewportTranslationX();
	float translateY = viewportTranslation.GetViewportTranslationY();
	float translateZ = viewportTranslation.GetViewportTranslationZ();

	Matrixf4x4 rotateMatrix = m_pMyActor->GetRotateMatrix4x4Rev();
	Matrixf4x4 scalingMatrix = m_pMyActor->GetScaleMatrix4x4();
	Matrixf4x4 translateMatrix = m_pMyActor->GetTranslateMatrix4x4_2();

	Matrixf4x4 modelMatrix =  rotateMatrix * scalingMatrix * translateMatrix;


	// Direction
	Vectorf3 direction = worldNearPlanePos - worldNearPlaneOrigin;
	direction.normalize();

	Vectorf3 v1m;
	Vectorf3 v2m;
	Vectorf3 v3m;

	Vectorf3 v1w;
	Vectorf3 v2w;
	Vectorf3 v3w;

	bool ret = false;

	// 2 triangles for z = m_zMax
	v1m = Vectorf3{ m_xMin, m_yMax, m_zMax };
	v2m = Vectorf3{ m_xMin, m_yMin, m_zMax };
	v3m = Vectorf3{ m_xMax, m_yMax, m_zMax };
	v1w = (v1m * modelMatrix).homogeniousDivide();
	v2w = (v2m * modelMatrix).homogeniousDivide();
	v3w = (v3m * modelMatrix).homogeniousDivide();
	ret = isRayIntersectWithPlane(v1w, v2w, v3w, worldNearPlaneOrigin, direction, outPointOfInersection);
	if (ret == true) {
		return true;
	}
	v1m = Vectorf3{ m_xMin, m_yMin, m_zMax };
	v2m = Vectorf3{ m_xMax, m_yMin, m_zMax };
	v3m = Vectorf3{ m_xMax, m_yMax, m_zMax };
	v1w = (v1m * modelMatrix).homogeniousDivide();
	v2w = (v2m * modelMatrix).homogeniousDivide();
	v3w = (v3m * modelMatrix).homogeniousDivide();
	ret = isRayIntersectWithPlane(v1w, v2w, v3w, worldNearPlaneOrigin, direction, outPointOfInersection);
	if (ret == true) {
		return true;
	}
}


void CActorAABB::SelectActor() {
	m_pMyActor->SelectActor();
}

void CActorAABB::DeselectActor() {
	m_pMyActor->DeselectActor();
}


void CActorAABB::getActorProperty(CActorProperty& actorProperty) {

	// Translation.
	float translationX = m_pMyActor->GetPosX();
	float translationY = m_pMyActor->GetPosY();
	float translationZ = m_pMyActor->GetPosZ();

	// Rotation.
	float rotationX = m_pMyActor->GetRotateX();
	float rotationY = m_pMyActor->GetRotateY();
	float rotationZ = m_pMyActor->GetRotateZ();

	// Scale k
	float scale_k = m_pMyActor->GetScale();

	// Tessellation On/Off
	bool tessellationOnOff = m_pMyActor->GetTessellationOnOff();

	// Tessellation Domain
	int tessellationDomain = m_pMyActor->GetTessellationDomain();

	// Tessellation Factor Outer 0-3
	int tessellationFactorOuter0 = m_pMyActor->GetTessellationLevelFactorOuter0();
	int tessellationFactorOuter1 = m_pMyActor->GetTessellationLevelFactorOuter1();
	int tessellationFactorOuter2 = m_pMyActor->GetTessellationLevelFactorOuter2();
	int tessellationFactorOuter3 = m_pMyActor->GetTessellationLevelFactorOuter3();

	// Tessellation Factor Inner 0-1
	int tessellationFactorInner0 = m_pMyActor->GetTessellationLevelFactorInner0();
	int tessellationFactorInner1 = m_pMyActor->GetTessellationLevelFactorInner1();

	actorProperty.SetActorTranslationX(translationX);
	actorProperty.SetActorTranslationY(translationY);
	actorProperty.SetActorTranslationZ(translationZ);

	actorProperty.SetActorRotationX(rotationX);
	actorProperty.SetActorRotationY(rotationY);
	actorProperty.SetActorRotationZ(rotationZ);

	actorProperty.SetActorScaleK(scale_k);

	actorProperty.SetTessellationOnOff(tessellationOnOff);

	actorProperty.SetTessellationDomain(tessellationDomain);

	actorProperty.SetTessellationLevelFactorOuter0(tessellationFactorOuter0);
	actorProperty.SetTessellationLevelFactorOuter1(tessellationFactorOuter1);
	actorProperty.SetTessellationLevelFactorOuter2(tessellationFactorOuter2);
	actorProperty.SetTessellationLevelFactorOuter3(tessellationFactorOuter3);

	actorProperty.SetTessellationLevelFactorInner0(tessellationFactorInner0);
	actorProperty.SetTessellationLevelFactorInner1(tessellationFactorInner1);

}
