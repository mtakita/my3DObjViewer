
#include "pch.h"
#include "CAABB.h"
#include "CDeviceContext.h"
#include "etc.h"
#include "logGLError.h"
#include "mymath.h"
#include "CActor.h"
#include "ShaderVariableNames.h"

using namespace mymath;

CAABB::CAABB(CActor* pMyActor) {
	m_pMyActor = pMyActor;
}

int CAABB::InitializeShaders(CDeviceContext& devCon) {

	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "shaders/aabb/aabb.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/aabb/aabb.flag" },
	};
	/*
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "shaders/vertext_shader.vert" },
			{ GL_FRAGMENT_SHADER, "shaders/fragment_shader.flag" },
		};
	*/
	renderProgramId = glCreateProgram();
	logGlError();

	attachShaders(renderProgramId, shaders, sizeof(shaders) / sizeof(ShaderInfo));

	glLinkProgram(renderProgramId);
	logGlError();

	glUseProgram(renderProgramId);
	logGlError();

	render_model_matrix_loc = glGetUniformLocation(renderProgramId, AABB_VERT_model_matrix);
	logGlError();

	render_projection_matrix_loc = glGetUniformLocation(renderProgramId, AABB_VERT_projection_matrix);
	logGlError();

	wglMakeCurrent(NULL, NULL);

	return renderProgramId;
}

void CAABB::attachShaders(GLuint programId, ShaderInfo shaders[], int size) {

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

		GLchar* source = new GLchar[(size_t)len + 1];
		memset(source, 0, (size_t)len + 1);

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


void CAABB::prepareDrawing2(CDeviceContext& devCon)
{
	// TODO: ‚±‚±‚ÉŽÀ‘•ƒR[ƒh‚ð’Ç‰Á‚µ‚Ü‚·.
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	//
	// Arrange vertices for rendering.
	//

	int loc = 0;
	pVertexBuffer = new GLfloat[8 * 3];

	//
	// Vertices for z = zMin plane.
	//

	pVertexBuffer[loc++] = m_xMin;
	pVertexBuffer[loc++] = m_yMin;
	pVertexBuffer[loc++] = m_zMin;

	pVertexBuffer[loc++] = m_xMax;
	pVertexBuffer[loc++] = m_yMin;
	pVertexBuffer[loc++] = m_zMin;

	pVertexBuffer[loc++] = m_xMax;
	pVertexBuffer[loc++] = m_yMax;
	pVertexBuffer[loc++] = m_zMin;

	pVertexBuffer[loc++] = m_xMin;
	pVertexBuffer[loc++] = m_yMax;
	pVertexBuffer[loc++] = m_zMin;

	//
	// Vertices for z = zMax plane.
	//

	pVertexBuffer[loc++] = m_xMin;
	pVertexBuffer[loc++] = m_yMin;
	pVertexBuffer[loc++] = m_zMax;

	pVertexBuffer[loc++] = m_xMax;
	pVertexBuffer[loc++] = m_yMin;
	pVertexBuffer[loc++] = m_zMax;

	pVertexBuffer[loc++] = m_xMax;
	pVertexBuffer[loc++] = m_yMax;
	pVertexBuffer[loc++] = m_zMax;

	pVertexBuffer[loc++] = m_xMin;
	pVertexBuffer[loc++] = m_yMax;
	pVertexBuffer[loc++] = m_zMax;

	//
	// Arrange vertix indeces for rendering.
	//

	loc = 0;
	pVertexIndexBuffer = new GLuint[3*2*6];
/*
	// two triangles for z = zMin plane.
	pVertexIndexBuffer[loc++] = 0;
	pVertexIndexBuffer[loc++] = 1;
	pVertexIndexBuffer[loc++] = 2;

	pVertexIndexBuffer[loc++] = 0;
	pVertexIndexBuffer[loc++] = 2;
	pVertexIndexBuffer[loc++] = 3;
	*/
	// two triangles for z = zMax plane.
	pVertexIndexBuffer[loc++] = 4;
	pVertexIndexBuffer[loc++] = 5;
	pVertexIndexBuffer[loc++] = 6;

	pVertexIndexBuffer[loc++] = 4;
	pVertexIndexBuffer[loc++] = 6;
	pVertexIndexBuffer[loc++] = 7;
/*
	// two triangles for x = xMin plane.
	pVertexIndexBuffer[loc++] = 0;
	pVertexIndexBuffer[loc++] = 3;
	pVertexIndexBuffer[loc++] = 4;

	pVertexIndexBuffer[loc++] = 4;
	pVertexIndexBuffer[loc++] = 3;
	pVertexIndexBuffer[loc++] = 7;

	// two triangles for x = xMax plane.
	pVertexIndexBuffer[loc++] = 1;
	pVertexIndexBuffer[loc++] = 5;
	pVertexIndexBuffer[loc++] = 6;

	pVertexIndexBuffer[loc++] = 1;
	pVertexIndexBuffer[loc++] = 6;
	pVertexIndexBuffer[loc++] = 2;

	// two triangles for y = yMin plane.

	pVertexIndexBuffer[loc++] = 0;
	pVertexIndexBuffer[loc++] = 5;
	pVertexIndexBuffer[loc++] = 1;

	pVertexIndexBuffer[loc++] = 0;
	pVertexIndexBuffer[loc++] = 4;
	pVertexIndexBuffer[loc++] = 5;

	// two triangles for y = yMax plane.

	pVertexIndexBuffer[loc++] = 3;
	pVertexIndexBuffer[loc++] = 2;
	pVertexIndexBuffer[loc++] = 6;

	pVertexIndexBuffer[loc++] = 3;
	pVertexIndexBuffer[loc++] = 7;
	pVertexIndexBuffer[loc++] = 6;
*/	

	glGenVertexArrays(num_vao, vaoid);
	logGlError();

	glBindVertexArray(vaoid[vao]);
	logGlError();
	
	// Create a new buffer object.
	glGenBuffers(1, &buffer);
	logGlError();

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	logGlError();

	// Allocate a space for it.
	glBufferStorage(GL_ARRAY_BUFFER,
		sizeof(GLfloat[8 * 3]),
		nullptr,
		GL_DYNAMIC_STORAGE_BIT);
	logGlError();

	// Verties.
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(GLfloat[8 * 3]),
		pVertexBuffer);
	logGlError();


	//
	// Vertex attribute array pointer setup start.
	//

	/**/
	// Vertex non-index.
	/**/
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); logGlError();

	//
	// Prepare for element drawing.
	//

	glGenBuffers(1, &bufferForIndex);
	logGlError();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferForIndex);
	logGlError();

	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLuint[3 * 2 * 6]),
		nullptr,
		GL_DYNAMIC_STORAGE_BIT);
	logGlError();

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
		0,
		sizeof(GLuint[3 * 2 * 6]),
		pVertexIndexBuffer);
	logGlError();

	wglMakeCurrent(NULL, NULL);

}


void CAABB::Render2(CDeviceContext& devCon, Matrixf4x4 projectionMatrix )
{
	try {

		Matrixf4x4 model_matrix = mymath::translate2( m_pMyActor->GetPosX(), m_pMyActor->GetPosY(), m_pMyActor->GetPosZ());

		glUseProgram(renderProgramId);
		logGlError();

		glUniformMatrix4fv(render_model_matrix_loc, 1, GL_TRUE, model_matrix); logGlError();
		glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_TRUE, projectionMatrix); logGlError();


		glBindVertexArray(vaoid[vao]);		logGlError();
		glBindBuffer(GL_ARRAY_BUFFER, buffer); logGlError();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferForIndex);
		logGlError();

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		logGlError();

		glEnableVertexAttribArray(vPosition); logGlError();
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0)); logGlError();

	}
	catch (const std::out_of_range & oor) {

		TRACE(oor.what());

	}

}


int CAABB::getActorId() 
{
	return m_pMyActor->getActorId();
}
