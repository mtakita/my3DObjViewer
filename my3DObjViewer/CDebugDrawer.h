#pragma once

#ifndef _CDEBUGDRAWER_H_
#define _CDEBUGDRAWER_H_

#include "CViewportProperty.h"
#include "CDeviceContext.h"
#include "etc.h"
#include "CActor.h"
#include "mymath.h"
#include <vector>
#include "CGeometryBase.h"

using std::vector;

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
class CLine;

//class CDebugDrawer : CGeometryBase
class CDebugDrawer
{
public:
	CDebugDrawer() : m_lines{} {

	}
	/*
	CDebugDrawer(float wPosX, float wPosY, float wPosZ, float k, float xAxis, float yAxis, float zAxis) 
		: m_lines{}
		,CGeometryBase(wPosX, wPosY, wPosZ, k, xAxis, yAxis, zAxis) {
	};
	*/

	int  InitializeShaders(CDeviceContext& devCon);
	void prepareDrawing2(CDeviceContext& devCon);

	void Render2(CDeviceContext& devCon, Matrixf4x4 projectionMatrix);
	void attachShaders(GLuint programId, ShaderInfo shaders[], int size);

	void AddLine(Vectorf3 origin, Vectorf3 direction, Vectorf3 endPos ) {

		CLine newLine{ origin, direction, endPos };
		m_lines.push_back(newLine);
	}

private:
	GLuint renderProgramId;
	GLint render_model_matrix_loc;
	GLint render_projection_matrix_loc;

	enum VAO_ID { vao, num_vao };
	GLuint vaoid[num_vao];
	enum ATTRIB_ID2 { vPosition = 0, vColor };

	// Buffer Object
	GLuint buffer;
	GLuint bufferForIndex;

	void PutAwayExpiredLine();

	class CLine {
	public:
		float red_comp;
		float blue_comp;

	public:
		CLine(Vectorf3 origin, Vectorf3 direction, Vectorf3 endPos ) {
			m_origin = origin;
			m_direction = direction;
			m_endPos = endPos;

			m_appTime = app_time();

//			red_comp = static_cast<float>(rand()) / RAND_MAX;
//			blue_comp = static_cast<float>(rand()) / RAND_MAX;
			red_comp = 1.0f;
			blue_comp = 1.0f;

		}

		Vectorf3 GetOrigin() { return m_origin;  }
		Vectorf3 GetDirection() { return m_direction;  }
		Vectorf3 GetEndPos() { return m_endPos;  }
		unsigned int GetAppTime() { return m_appTime;  }
	public:
		Vectorf3 m_origin;
		Vectorf3 m_direction;
		Vectorf3 m_endPos;

		unsigned int m_appTime;
	};

	vector<CLine> m_lines;

};

#endif
