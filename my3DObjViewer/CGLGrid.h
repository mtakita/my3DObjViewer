#pragma once
#include "CDeviceContext.h"

#include "etc.h"
#include "logGLError.h"

#include "mymath.h"
#include "CViewportProperty.h"

using namespace mymath;

class CGLGrid
{
public:
	int InitializeShaders(CDeviceContext& devCon);
	void attachShaders(GLuint programId, ShaderInfo shaders[], int size);

	void PrepareScene(CDeviceContext& devCon);
	void DestroyScene(CDeviceContext& devCon);
	void Reshape(CDeviceContext& devCon, int w, int h);
	void Render(CDeviceContext& devCon, Matrixf4x4 projectionMatrix);
	void Render(CDeviceContext& devCon, Matrixf4x4 projectionMatrix, Matrixf4x4 translationScalingMatrix);

private:
	enum VAO_ID { vao, num_vao };
	GLuint vaoid[num_vao];
	enum ATTRIB_ID { vPositionIndex = 0, vColor };

	GLuint buffer;
	GLuint bufferForIndex;

	float aspect = 0;

	GLuint renderProgramId = 0;
	GLint render_model_matrix_loc = 0;
	GLint render_projection_matrix_loc = 0;

	size_t sizeOfLineBuffer;
	size_t sizeOfColorBuffer;

};

