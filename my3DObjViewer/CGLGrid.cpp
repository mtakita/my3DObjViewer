#include "pch.h"
#include "CGLGrid.h"
#include <memory>
#include "mymath.h"
#include "etc.h"
#include "ShaderVariableNames.h"

using namespace mymath;

using std::unique_ptr;
using mymath::Vectorf3;

int CGLGrid::InitializeShaders(CDeviceContext& devCon) {

	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "shaders/grid/grid.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/grid/grid.flag" },
	};

	renderProgramId = glCreateProgram();
	logGlError();

	attachShaders(renderProgramId, shaders, sizeof(shaders) / sizeof(ShaderInfo));

	glLinkProgram(renderProgramId);
	logGlError();

	glUseProgram(renderProgramId);
	logGlError();

	render_model_matrix_loc = glGetUniformLocation(renderProgramId, GRID_VERT_model_matrix);
	logGlError();

	render_projection_matrix_loc = glGetUniformLocation(renderProgramId, GRID_VERT_projection_matrix);
	logGlError();

	wglMakeCurrent(NULL, NULL);

	return renderProgramId;
}

void CGLGrid::attachShaders(GLuint programId, ShaderInfo shaders[], int size) {

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

#define NUM_OF_LINES 19 // Shoud be odd number to pass through the origin.
#define LENGTH_OF_LINE 1000
#define LINE_INTERVAL ( LENGTH_OF_LINE / (NUM_OF_LINES-1.0f) )

#define NUM_OF_ELEMENTS ( 2 * 2 * 2 * 2 * NUM_OF_LINES)

void CGLGrid::PrepareScene(CDeviceContext& devCon)
{
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	// vertices and colors.

	sizeOfLineBuffer = sizeof(GLfloat[NUM_OF_LINES][16][3]);
	sizeOfColorBuffer = sizeof(GLfloat[NUM_OF_LINES][16][3]);

	unique_ptr<GLfloat[][16][3]> pLine{ new GLfloat[NUM_OF_LINES][16][3] };
	unique_ptr<GLfloat[][16][3]> pColor{ new GLfloat[NUM_OF_LINES][16][3] };

	GLfloat black[3] = { 0.0f,0.0f,0.0f };
	GLfloat red[3] = { 1.0f,0.0f,0.0f };

	GLfloat blue[3] = { 0.0f,0.0f,1.0f };
	GLfloat red2[3] = { 0.0f,1.0f,0.0f };

	for (int i = 0; i < NUM_OF_LINES; i++) {

		float inc = i * LINE_INTERVAL - ((float)(LENGTH_OF_LINE) / 2.0f);
		float fixedSide = -((float)(LENGTH_OF_LINE) / 2.0f);
		float fixedOtherSide = +((float)(LENGTH_OF_LINE) / 2.0f);

		int n = 0;
		// 1
		pLine[i][n][0] = inc;
		pLine[i][n][1] = fixedSide;
		pLine[i][n++][2] = fixedOtherSide;

		pLine[i][n][0] = inc;
		pLine[i][n][1] = fixedOtherSide;
		pLine[i][n++][2] = fixedOtherSide;

		// 2
		pLine[i][n][0] = fixedSide;
		pLine[i][n][1] = inc;
		pLine[i][n++][2] = fixedOtherSide;

		pLine[i][n][0] = fixedOtherSide;
		pLine[i][n][1] = inc;
		pLine[i][n++][2] = fixedOtherSide;

		// 3
		pLine[i][n][0] = fixedSide; 
		pLine[i][n][1] = fixedSide;
		pLine[i][n++][2] = inc; 

		pLine[i][n][0] = fixedOtherSide; 
		pLine[i][n][1] = fixedSide;
		pLine[i][n++][2] = inc; 

		// 4
		pLine[i][n][0] = inc;
		pLine[i][n][1] = fixedSide;
		pLine[i][n++][2] = fixedSide;

		pLine[i][n][0] = inc;
		pLine[i][n][1] = fixedSide;
		pLine[i][n++][2] = fixedOtherSide;



		// 1-2
		pLine[i][n][0] = inc;
		pLine[i][n][1] = fixedSide;
		pLine[i][n++][2] = fixedSide;

		pLine[i][n][0] = inc;
		pLine[i][n][1] = fixedOtherSide;
		pLine[i][n++][2] = fixedSide;

		// 2-2
		pLine[i][n][0] = fixedSide;
		pLine[i][n][1] = inc;
		pLine[i][n++][2] = fixedSide;

		pLine[i][n][0] = fixedOtherSide;
		pLine[i][n][1] = inc;
		pLine[i][n++][2] = fixedSide;

		// 3-2
		pLine[i][n][0] = fixedSide;
		pLine[i][n][1] = fixedOtherSide;
		pLine[i][n++][2] = inc;

		pLine[i][n][0] = fixedOtherSide;
		pLine[i][n][1] = fixedOtherSide;
		pLine[i][n++][2] = inc;

		// 4-2
		pLine[i][n][0] = inc;
		pLine[i][n][1] = fixedOtherSide;
		pLine[i][n++][2] = fixedSide;

		pLine[i][n][0] = inc;
		pLine[i][n][1] = fixedOtherSide;
		pLine[i][n++][2] = fixedOtherSide;

		if (i != (NUM_OF_LINES - 1) / 2) {

			int n = 0;

			pColor[i][n  ][0] = black[0];
			pColor[i][n  ][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];


			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];


			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

			pColor[i][n][0] = black[0];
			pColor[i][n][1] = black[1];
			pColor[i][n++][2] = black[2];

		}
		else {

			int n = 0;

			pColor[i][n  ][0] = red[0];
			pColor[i][n  ][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];


			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];


			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

			pColor[i][n][0] = red[0];
			pColor[i][n][1] = red[1];
			pColor[i][n++][2] = red[2];

		}
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

	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfLineBuffer) );
	logGlError();

	glEnableVertexAttribArray(vColor);
	logGlError();


	wglMakeCurrent(NULL, NULL);
}


void CGLGrid::DestroyScene(CDeviceContext& devCon)
{
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	wglMakeCurrent(NULL, NULL);
	if (devCon.GetHGLRC())
	{
		wglDeleteContext(devCon.GetHGLRC());
	}
}

void CGLGrid::Reshape(CDeviceContext& devCon, int w, int h)
{
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	glViewport(0, 0, w, h);

	aspect = float(h) / float(w);


	wglMakeCurrent(NULL, NULL);
}


//void CGLGrid::Render(CDeviceContext& devCon, Matrixf4x4 projectionMatrix, CViewportProperty& viewportTranslation )
void CGLGrid::Render(CDeviceContext& devCon, Matrixf4x4 projectionMatrix )
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

		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfLineBuffer ));
		logGlError();

		glEnableVertexAttribArray(vColor);
		logGlError();

		Matrixf4x4 modelTransform = Matrixf4x4{};
		modelTransform.identity();

		glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_TRUE, projectionMatrix); logGlError();
		glUniformMatrix4fv(render_model_matrix_loc, 1, GL_TRUE, modelTransform); logGlError();

		glDrawElements(GL_LINES, NUM_OF_ELEMENTS, GL_UNSIGNED_INT, NULL); logGlError();
	}
	catch (const std::out_of_range & oor) {

		TRACE(oor.what());

	}

}

void CGLGrid::Render(CDeviceContext& devCon, Matrixf4x4 projectionMatrix, Matrixf4x4 translationScalingMatrix)
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

		Matrixf4x4 modelTransform = Matrixf4x4{};
		modelTransform.identity();

		glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_TRUE, projectionMatrix); logGlError();
		glUniformMatrix4fv(render_model_matrix_loc, 1, GL_TRUE, modelTransform * translationScalingMatrix ); logGlError();

		//		glLineWidth(2.0f);

		glDrawElements(GL_LINES, NUM_OF_ELEMENTS, GL_UNSIGNED_INT, NULL); logGlError();
	}
	catch (const std::out_of_range & oor) {

		TRACE(oor.what());

	}

}

