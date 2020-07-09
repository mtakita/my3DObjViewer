#pragma once
#ifndef _CACTORAABB_H_
#define _CACTORAABB_H_

#include "mymath.h"
#include "CDeviceContext.h"
#include "CViewportProperty.h"
#include "CActor.h"
#include "CSelectable.h"
#include "CActorProperty.h"

using namespace mymath;

class CActor;

class CActorAABB : CSelectable
{
	friend class CActor;

public:
	CActorAABB();
	CActorAABB( CActor* pMyActor, CActorAABB& aabb );

	CActorAABB(int actorId, CActor* pMyActor);

	CActorAABB(CActor* pMyActor, float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

	int getActorId();
	void getActorProperty(CActorProperty& actorProperty);

	bool isActorPointed(Vectorf3 worldNearPlanePos, Vectorf3 worldNearPlaneOrigin, CDeviceContext& devCon, CViewportProperty& viewportTranslation, Vectorf3& outPointOfInersection);

	virtual void SelectActor();
	virtual void DeselectActor();

private:
	void init();
	CActorAABB(CActor* pMyActor, int actorId, float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

private:

	int m_ActorId;
	CActor* m_pMyActor;

	/*                          */
	/* world space coordinates. */
	/*                          */

	float m_xMin;
	float m_xMax;

	float m_yMin;
	float m_yMax;

	float m_zMin;
	float m_zMax;

};

#endif
