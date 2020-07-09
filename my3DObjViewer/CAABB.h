#pragma once

#ifndef _CAABB_H_
#define _CAABB_H_

#include "CViewportProperty.h"
#include "CDeviceContext.h"
#include "etc.h"
#include "CActor.h"
#include "mymath.h"

using namespace mymath;
//
//
// This class is a temptative class for debugging.  This canbe deleted at anytime.
// This class is a temptative class for debugging.  This canbe deleted at anytime.
// This class is a temptative class for debugging.  This canbe deleted at anytime.
// This class is a temptative class for debugging.  This canbe deleted at anytime.
// This class is a temptative class for debugging.  This canbe deleted at anytime.
// This class is a temptative class for debugging.  This canbe deleted at anytime.
// This class is a temptative class for debugging.  This canbe deleted at anytime.
// This class is a temptative class for debugging.  This canbe deleted at anytime.
// This class is a temptative class for debugging.  This canbe deleted at anytime.
//
//

class CActor;

class CAABB
{
public:
	CAABB(CActor* pMyActor);

	int  InitializeShaders(CDeviceContext& devCon);
	void prepareDrawing2(CDeviceContext& devCon);

	void Render2(CDeviceContext& devCon, Matrixf4x4 projectionMatrix);
	void attachShaders(GLuint programId, ShaderInfo shaders[], int size);

	/*                          */
	/* world space coordinates. */
	/*                          */

	float m_xMin;
	float m_xMax;

	float m_yMin;
	float m_yMax;

	float m_zMin;
	float m_zMax;

	int getActorId();

private:
	GLuint renderProgramId;
	GLint render_model_matrix_loc;
	GLint render_projection_matrix_loc;

	enum VAO_ID { vao, num_vao };
	GLuint vaoid[num_vao];
	enum ATTRIB_ID2 { vPosition = 0 };

	// Buffer Object
	GLuint buffer;
	GLuint bufferForIndex;
	GLfloat* pVertexBuffer;
	GLuint* pVertexIndexBuffer;

	CActor* m_pMyActor;
};

#endif
