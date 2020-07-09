
#include "pch.h"
#include "CActorSelector.h"
#include "CDebugDrawer.h"
#include "CMy3DObjViewerDoc.h"
#include "CActorProperty.h"
#include "CEyeSpace.h"

#define _USE_MATH_DEFINES
#include <math.h>

CActorSelector::CActorSelector() {

}

CActorSelector::~CActorSelector() {

	map<int, CActorAABB*>::iterator itor = m_ActorAABBs.begin();
	while (itor != m_ActorAABBs.end()) {

		int index = itor->first;
		CActorAABB* pActorAABB = m_ActorAABBs[index];
		delete pActorAABB;

		itor++;
	}


}


void CActorSelector::AddActor(CActorAABB* pActorAABB) {

	int index = pActorAABB->getActorId();
	m_ActorAABBs[index] = pActorAABB;

}

void CActorSelector::DeleteActor( int actorId ) {

	map<int, CActorAABB*>::iterator itorActorAABB = m_ActorAABBs.begin();

	while (itorActorAABB != m_ActorAABBs.end()) {

		CActorAABB* pActorAABB = m_ActorAABBs[(*itorActorAABB).first];
		if (pActorAABB->getActorId() == actorId) {

			m_ActorAABBs.erase((*itorActorAABB).first);
			break;

		}

		itorActorAABB++;
	}

}

// 
// 
// 
// ScreenX : x coordinate on screen coordinate from mouse input.
// ScreenY : y coordinate on screen coordinate from mouse input.
// width   : screen width.
// height  : screen height.
// left    : left of the near place of frustum.
// right   : right of the near place of frustum.
// bottom  : bottom of the near place of frustum.
// top     : top of the near place of frustum.
// n       : near plane.
void CActorSelector::Select(int screenPosX, int screenPosY,
	int width, int height,
	float left, float right, float bottom, float top, float n, 
	CDeviceContext& devCon, 
	CViewportProperty& viewportTranslation, 
	CDebugDrawer& debugDrawers,
	CMy3DObjViewerDoc* pDoc, CEyeSpace eyeSpace, bool recordRay ) {

	float farPlane = viewportTranslation.GetPerspectiveFrustumFar();

	float horizontalFov = viewportTranslation.GetHorizontalFov();
	float verticalFov = viewportTranslation.GetVerticalFov();

	RECT rect;
	devCon.GetDC()->GetWindow()->GetClientRect(&rect);

	float zoomX = 1.0f / tan(horizontalFov * (M_PI / 180.0f) / 2.0f);
	float zoomY = zoomX * (float(rect.right - rect.left) / float(rect.bottom - rect.top));

	// Convert screen coordinates to NDC.
	float ndcPosXrecon = (2.0f / width) * screenPosX - 1.0f;
	float ndcPosYrecon = -(2.0f / height) * screenPosY + 1.0f;

	// Convert NDC to near plane coordinates.
	float npPosXrecon = ((ndcPosXrecon * (right - left) + right + left) / 2.0f) ;
	float npPosYrecon = ((ndcPosYrecon * (top - bottom) + top + bottom) / 2.0f) ;
	float npPosZrecon = n;

	Vectorf4 nearPlanePos{ npPosXrecon, npPosYrecon, npPosZrecon, 1 };
	nearPlanePos.normalize();
	Vectorf4 nearPlaneOrigin{ 0, 0, 0, 1 };

	Matrixf4x4 rotMat = eyeSpace.GetRotateMatrix4x4Rev();
	Matrixf4x4 scaleMat = eyeSpace.GetScaleMatrix4x4();
	Matrixf4x4 transMat = eyeSpace.GetTranslateMatrix4x4_2();

	Matrixf4x4 negZMat;
	negZMat.identityNegateZAxis();

	Matrixf4x4 transform_matrix = negZMat * rotMat * scaleMat * transMat;

	Vectorf3 worldNearPlanePos = (nearPlanePos * transform_matrix).homogeniousDivide();
	Vectorf3 worldNearPlaneOrigin = (nearPlaneOrigin * transform_matrix).homogeniousDivide();

	Vectorf3 direction = worldNearPlanePos - worldNearPlaneOrigin;
	direction.normalize();

	if (recordRay == true) {

		//
		// Draw eye lines for debugging purpose.
		//
		debugDrawers.AddLine(worldNearPlaneOrigin, direction, worldNearPlanePos);

	}

	//
	// Repeat check for each one of triangles from AABB. 
	//
	map<int, CActorAABB*>::iterator itor = m_ActorAABBs.begin();

	Vectorf3 outPointOfIntersection;

	int actorId = 0;

	while (itor != m_ActorAABBs.end()) {

		int index = itor->first;
		CActorAABB* pAabb = m_ActorAABBs[index];

		bool ret = pAabb->isActorPointed(worldNearPlanePos, worldNearPlaneOrigin, devCon, viewportTranslation, outPointOfIntersection);
		if (ret == true) {

			//
			// Draw eye lines for debugging purpose.
			//
			Vectorf3 absoluteOriginForDebug;
			Vectorf3 direction = outPointOfIntersection - worldNearPlaneOrigin;
			direction.normalize();

			debugDrawers.AddLine(worldNearPlaneOrigin, direction, outPointOfIntersection );

			// Choose an actor to select.
			actorId = pAabb->getActorId();
			break;
		}

		itor++;
	}

	SelectActor(actorId, pDoc);

}

void CActorSelector::SelectActor(CActorAABB& selectTarget, CMy3DObjViewerDoc* pDoc) {
	SelectActor(selectTarget.getActorId(), pDoc);
}



void CActorSelector::SelectActor(int actorId, CMy3DObjViewerDoc* pDoc) {

	//
	// Select an actor and deselect the others.
	//

	CActorAABB* pSelectedActorAABB = nullptr;

	map<int, CActorAABB*>::iterator itor = m_ActorAABBs.begin();
	while (itor != m_ActorAABBs.end()) {

		int index = itor->first;
		CActorAABB* pAabb = m_ActorAABBs[index];
		int actorIdTemp = pAabb->getActorId();
		if (actorIdTemp == 999) {
			return;
		}

		if ( actorId == pAabb->getActorId()) {

			// Choose the actor aabb.
			pSelectedActorAABB = pAabb;

			//
			// Update actor propert with the actor now selected.
			//
			CActorProperty actorProperty;
			pAabb->getActorProperty(actorProperty);
			pDoc->updateActorProperty(actorProperty);

			pAabb->SelectActor();
		}
		else {
			pAabb->DeselectActor();
		}

		itor++;
	}

	//
	// Bring Actor property window.
	//
	if (pSelectedActorAABB != nullptr) {

		pDoc->showActorProperty();
	}
	else {

		pDoc->hideActorProperty();
		pDoc->selectItem(NULL);

	}

}

