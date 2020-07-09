
#include "CViewportActor.h"

void CViewportActor::importActorProperty() {
	
	// retrieve actor property.(maybe some other actor's, not mine)
	CActorProperty actorPropertyWnd;
	m_pActorPropertiesWnd->getActorProperty(actorPropertyWnd);
	importActorProperty(actorPropertyWnd);
}

