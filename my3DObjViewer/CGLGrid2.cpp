#include "pch.h"
#include "CGLGrid2.h"

#include "pch.h"
#include "CGLGrid.h"
#include <memory>
#include "mymath.h"
#include "etc.h"
#include "CEyeSpace.h"
#include "ShaderVariableNames.h"

using namespace mymath;

using std::unique_ptr;
using mymath::Vectorf3;
using std::numeric_limits;

int CGLGrid2::InitializeShaders(CDeviceContext& devCon) {

	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "shaders/grid2/grid2.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/grid2/grid2.flag" },
	};

	renderProgramId = glCreateProgram();
	logGlError();

	attachShaders(renderProgramId, shaders, sizeof(shaders) / sizeof(ShaderInfo));

	glLinkProgram(renderProgramId);
	logGlError();

	glUseProgram(renderProgramId);
	logGlError();

	render_model_matrix_loc = glGetUniformLocation(renderProgramId, GRID2_VERT_model_matrix);
	logGlError();

	render_projection_matrix_loc = glGetUniformLocation(renderProgramId, GRID2_VERT_projection_matrix);
	logGlError();

	wglMakeCurrent(NULL, NULL);

	return renderProgramId;
}

void CGLGrid2::attachShaders(GLuint programId, ShaderInfo shaders[], int size) {

	for (int i = 0; i < size; i++) {

		FILE* infile;
		fopen_s(&infile, shaders[i].shaderFile, "rb");
		if (!infile) {
			std::cerr << "Unable to open file '" << shaders[i].shaderFile << "'" << std::endl;
			exit(1);
		}

		fseek(infile, 0, SEEK_END);
		int len = ftell(infile);
		fseek(infile, 0, SEEK_SET);

		GLchar* source = new GLchar[len + 1];
		memset(source, 0, len + 1);

		fread(source, sizeof(GLchar), len, infile);
		fclose(infile);


		GLuint shaderId = glCreateShader(shaders[i].shaderType);
		logGlError();

		if (shaderId == 0) {
			fprintf(stderr, "failed to create shader\n");
			exit(1);
		}

		glShaderSource(shaderId, 1, &source, NULL);
		delete[] source, source = NULL;
		logGlError();

		glCompileShader(shaderId);
		logGlError();

		GLint ret;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ret);
		logGlError();

		if (ret == GL_FALSE) {

			char buff[250];
			GLint ret;
			glGetShaderInfoLog(shaderId, sizeof(buff), &ret, buff);
			fprintf(stderr, "failed to compile shader %s\n", buff);

			exit(1);
		}

		glAttachShader(programId, shaderId);
		logGlError();

	}
}

#define NUM_OF_LINES 1000
#define NUM_OF_ELEMENTS ( NUM_OF_LINES * 2 )
#define LINE_INTERVAL 100

void CGLGrid2::PrepareScene(CDeviceContext& devCon)
{
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	// vertices and colors.

	sizeOfLineBuffer = sizeof(GLfloat[NUM_OF_LINES][2][3]);
	sizeOfColorBuffer = sizeof(GLfloat[NUM_OF_LINES][2][3]);

	unique_ptr<GLfloat[][2][3]> pLine{ new GLfloat[NUM_OF_LINES][2][3] };
	unique_ptr<GLfloat[][2][3]> pColor{ new GLfloat[NUM_OF_LINES][2][3] };

	GLfloat gray[3] = { 0.7f, 0.7f, 0.7f };
	GLfloat lgray[3] = { 0.7f, 0.7f, 0.7f };

	GLfloat red[3] = { 0.7f, 0.7f, 0.7f };
	GLfloat lred[3] = { 0.7f, 0.7f, 0.7f };

	float fMin = -100000.0f;
	float fMax = +100000.0f;

	float shift = (NUM_OF_LINES * LINE_INTERVAL) / 4;

	// Horizontal line along x-axis.
	for (int i = 0; i < 500; i++) {
		pLine[i][0][0] = fMin - shift;
		pLine[i][0][1] = 0.0f;
		pLine[i][0][2] = i* LINE_INTERVAL - shift;

		pLine[i][1][0] = fMax - shift;
		pLine[i][1][1] = 0.0f;
		pLine[i][1][2] = i * LINE_INTERVAL - shift;
	}
	// Horizontal line along z-aixs.
	for (int i = 0; i < 500; i++) {

		pLine[500 + i][0][0] = i * LINE_INTERVAL - shift;
		pLine[500 + i][0][1] = 0.0f;
		pLine[500 + i][0][2] = fMin - shift;

		pLine[500 + i][1][0] = i * LINE_INTERVAL - shift;
		pLine[500 + i][1][1] = 0.0f;
		pLine[500 + i][1][2] = fMax - shift;
	}
	// Color for horizontal line along x-axis.
	for (int i = 0; i < 500; i++) {
		pColor[i][0][0] = gray[0];
		pColor[i][0][1] = gray[1];
		pColor[i][0][2] = gray[2];

		pColor[i][1][0] = lgray[0];
		pColor[i][1][1] = lgray[1];
		pColor[i][1][2] = lgray[2];
	}
	// Color for horizontal line along z-axis.
	for (int i = 0; i < 500; i++) {
		pColor[500+i][0][0] = red[0];
		pColor[500 + i][0][1] = red[1];
		pColor[500 + i][0][2] = red[2];

		pColor[500 + i][1][0] = lred[0];
		pColor[500 + i][1][1] = lred[1];
		pColor[500 + i][1][2] = lred[2];
	}

	GLuint elements[NUM_OF_ELEMENTS];

	for (int i = 0; i < NUM_OF_ELEMENTS; i++) {
		elements[i] = i;
	}

	glGenBuffers(1, &buffer);
	logGlError();

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	logGlError();

	glBufferStorage(GL_ARRAY_BUFFER,
		sizeOfLineBuffer + sizeOfColorBuffer,
		nullptr,
		GL_DYNAMIC_STORAGE_BIT);
	logGlError();

	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeOfLineBuffer,
		pLine.get());
	logGlError();

	glBufferSubData(GL_ARRAY_BUFFER,
		sizeOfLineBuffer,
		sizeOfColorBuffer,
		pColor.get());
	logGlError();

	glGenBuffers(1, &bufferForIndex);
	logGlError();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferForIndex);
	logGlError();

	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(elements),
		nullptr,
		GL_DYNAMIC_STORAGE_BIT);
	logGlError();

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
		0,
		sizeof(elements),
		elements);
	logGlError();



	glGenVertexArrays(num_vao, vaoid);
	logGlError();

	glBindVertexArray(vaoid[vao]);
	logGlError();

	/**/
	// Vertex Index.
	/**/
	glVertexAttribPointer(vPositionIndex, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	logGlError();

	glEnableVertexAttribArray(vPositionIndex);
	logGlError();

	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfLineBuffer));
	logGlError();

	glEnableVertexAttribArray(vColor);
	logGlError();


	wglMakeCurrent(NULL, NULL);
}


void CGLGrid2::DestroyScene(CDeviceContext& devCon)
{
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	wglMakeCurrent(NULL, NULL);
	if (devCon.GetHGLRC())
	{
		wglDeleteContext(devCon.GetHGLRC());
	}
}

void CGLGrid2::Reshape(CDeviceContext& devCon, int w, int h)
{
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	glViewport(0, 0, w, h);

	aspect = float(h) / float(w);


	wglMakeCurrent(NULL, NULL);
}


void CGLGrid2::Render(CDeviceContext& devCon, Matrixf4x4 projectionMatrix, Matrixf4x4 translationScalingMatrix, CEyeSpace& eyeSpace )
{
	try {

		glUseProgram(renderProgramId);
		logGlError();

		glBindVertexArray(vaoid[vao]);
		logGlError();

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		logGlError();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferForIndex);
		logGlError();

		/**/
		// Vertex Index.
		/**/
		glVertexAttribPointer(vPositionIndex, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		logGlError();

		glEnableVertexAttribArray(vPositionIndex);
		logGlError();

		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfLineBuffer));
		logGlError();

		glEnableVertexAttribArray(vColor);
		logGlError();

		Matrixf4x4 modelTransform;
		modelTransform.identity();

		glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_TRUE, projectionMatrix); logGlError();
		glUniformMatrix4fv(render_model_matrix_loc, 1, GL_TRUE, modelTransform * translationScalingMatrix); logGlError();

		glLineWidth(1.0f);

		glDrawElements(GL_LINES, NUM_OF_ELEMENTS, GL_UNSIGNED_INT, NULL); logGlError();
	}
	catch (const std::out_of_range & oor) {

		TRACE(oor.what());

	}

}

