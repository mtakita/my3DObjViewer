#pragma once

#ifndef _CACTOR_SELECTOR_
#define _CACTOR_SELECTOR_

#include <map>
#include "CActorAABB.h"
#include "mymath.h"
#include "CDebugDrawer.h"
#include "CMy3DObjViewerDoc.h"
#include "CEyeSpace.h"

using namespace mymath;

using std::map;

class CActorSelector
{
public:
	CActorSelector();
	~CActorSelector();
	void AddActor(CActorAABB* actorAABB);
	void DeleteActor( int actorId );

	void CActorSelector::Select(int screenPosX, int screenPosY,
		int width, int height,
		float left, float right, float bottom, float top, float n,
		CDeviceContext& devCon,
		CViewportProperty& viewportTranslation,
		CDebugDrawer& debugDrawers,
		CMy3DObjViewerDoc* pDoc, CEyeSpace eyeSpace, bool recordRay );

	void SelectActor(CActorAABB& selectTarget, CMy3DObjViewerDoc* pDoc);
	void SelectActor(int actorId, CMy3DObjViewerDoc* pDoc);

private:

	map<int, CActorAABB*> m_ActorAABBs;
};

#endif
