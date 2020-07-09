
#include "pch.h"
#include "CDebugDrawer.h"
#include "CDeviceContext.h"
#include "etc.h"
#include "logGLError.h"
#include "mymath.h"
#include "CActor.h"
#include "CDebugDrawer.h"
#include <memory>
#include "ShaderVariableNames.h"

using namespace mymath;
using std::unique_ptr;


int CDebugDrawer::InitializeShaders(CDeviceContext& devCon) {

	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "shaders/debugdrawer/debugdrawer.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/debugdrawer/debugdrawer.flag" }
	};
	renderProgramId = glCreateProgram();
	logGlError();

	attachShaders(renderProgramId, shaders, sizeof(shaders) / sizeof(ShaderInfo));

	glLinkProgram(renderProgramId);
	logGlError();

	glUseProgram(renderProgramId);
	logGlError();

	render_model_matrix_loc = glGetUniformLocation(renderProgramId, DEBUGDRAWER_VERT_model_matrix);
	logGlError();

	render_projection_matrix_loc = glGetUniformLocation(renderProgramId, DEBUGDRAWER_VERT_projection_matrix);
	logGlError();

	wglMakeCurrent(NULL, NULL);

	return renderProgramId;
}

void CDebugDrawer::attachShaders(GLuint programId, ShaderInfo shaders[], int size) {

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


void CDebugDrawer::prepareDrawing2(CDeviceContext& devCon)
{
	// TODO: ‚±‚±‚ÉŽÀ‘•ƒR[ƒh‚ð’Ç‰Á‚µ‚Ü‚·.
}


void CDebugDrawer::Render2(CDeviceContext& devCon, Matrixf4x4 projectionMatrix )
{
	try {

		if (m_lines.size() == 0) {
			// No lines.
			return;
		}

		glUseProgram(renderProgramId);
		logGlError();

		Matrixf4x4 modelMatrix{};
		modelMatrix.identity();

		glUniformMatrix4fv(render_model_matrix_loc, 1, GL_TRUE, modelMatrix); logGlError();
		glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_TRUE, projectionMatrix); logGlError();


		// Size needed for vertex buffer to draw a starting and an ending position for all lines.
		int numOfFloatBufferForLines = m_lines.size() * 3 * 2;
		int bytesOfBufferForLines = m_lines.size() * 3 * 2 * sizeof(GLfloat);
		unique_ptr<GLfloat> pVertexBuffer{ new GLfloat[numOfFloatBufferForLines] };

		int numOfFloatBufferForLineColors = m_lines.size() * 3 * 2;
		int bytesOfBufferForColorLines = m_lines.size() * 3 * 2 * sizeof(GLfloat);
		unique_ptr<GLfloat> pVertexColorBuffer{ new GLfloat[numOfFloatBufferForLineColors] };

		// Size needed for vertex index buffer to draw a starting and an ending position for all lines.
		int numOfIntOfIndexBufferForLines = m_lines.size() * 2;
		int bytesOfIndexBufferForLines = m_lines.size() * 2 * sizeof(int);
		unique_ptr<int> pVertexIndexBuffer{ new int[numOfFloatBufferForLines] };

		//
		// Arrange the contents of the buffer for drawing.
		//

		GLfloat* pBuffer = pVertexBuffer.get();
		GLfloat* pColorBuffer = pVertexColorBuffer.get();
		int* pIndexBuffer = pVertexIndexBuffer.get();

		int bufferCnt = 0;
		int colorBufferCnt = 0;
		int indexCnt = 0;
		for (CLine line : m_lines) {

			float red[3]{ line.blue_comp, 0.0f, 0.0f };
			float blue[3]{ 0.0f, 0.0f, line.red_comp };

			Vectorf3 startingPos = line.GetOrigin();
//			Vectorf3 endingPos = startingPos + line.GetDirection();
			Vectorf3 endingPos = line.GetEndPos();

			pBuffer[bufferCnt++] = startingPos.getX();
			pBuffer[bufferCnt++] = startingPos.getY();
			pBuffer[bufferCnt++] = startingPos.getZ();

			pBuffer[bufferCnt++] = endingPos.getX();
			pBuffer[bufferCnt++] = endingPos.getY();
			pBuffer[bufferCnt++] = endingPos.getZ();

			pColorBuffer[colorBufferCnt++] = red[0];
			pColorBuffer[colorBufferCnt++] = red[1];
			pColorBuffer[colorBufferCnt++] = red[2];

			pColorBuffer[colorBufferCnt++] = blue[0];
			pColorBuffer[colorBufferCnt++] = blue[1];
			pColorBuffer[colorBufferCnt++] = blue[2];

			pIndexBuffer[indexCnt++] = indexCnt;
			pIndexBuffer[indexCnt++] = indexCnt;


		}

		//
		// Upload vertex buffer.
		//

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
			bytesOfBufferForLines + bytesOfBufferForColorLines,
			nullptr,
			GL_DYNAMIC_STORAGE_BIT);
		logGlError();

		// Verties.
		glBufferSubData(GL_ARRAY_BUFFER,
			0,
			bytesOfBufferForLines,
			pVertexBuffer.get());
		logGlError();

		// Verties.
		glBufferSubData(GL_ARRAY_BUFFER,
			bytesOfBufferForLines,
			bytesOfBufferForColorLines,
			pVertexColorBuffer.get());
		logGlError();

		// Vertex attribute array pointer setup start.
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); logGlError();
		glEnableVertexAttribArray(vPosition); logGlError();

		// Vertex attribute array pointer setup start.
		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bytesOfBufferForLines)); logGlError();
		glEnableVertexAttribArray(vColor); logGlError();

		//
		// Upload element buffer.
		//

		glGenBuffers(1, &bufferForIndex);
		logGlError();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferForIndex);
		logGlError();

		glBufferStorage(GL_ELEMENT_ARRAY_BUFFER,
			bytesOfIndexBufferForLines,
			nullptr,
			GL_DYNAMIC_STORAGE_BIT);
		logGlError();

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
			0,
			bytesOfIndexBufferForLines,
			pVertexIndexBuffer.get());
		logGlError();

		glLineWidth(2.0f);


		glDrawElements(GL_LINES, numOfIntOfIndexBufferForLines, GL_UNSIGNED_INT, BUFFER_OFFSET(0)); logGlError();

		//
		// Clean up everything.
		//

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		logGlError();

		glDeleteBuffers(1, &buffer);
		logGlError();

		glDeleteVertexArrays(num_vao, vaoid);
		logGlError();
			   

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		logGlError();

		glDeleteBuffers(1, &bufferForIndex);
		logGlError();

		//
		// Eliminate line expired.
		//
		PutAwayExpiredLine();

	}
	catch (const std::out_of_range & oor) {

		TRACE(oor.what());

	}

}

void CDebugDrawer::PutAwayExpiredLine() {

	unsigned int currentTime = app_time();

	for (CLine line : m_lines) {

		unsigned int timeBorn = line.GetAppTime();
		if ( currentTime - timeBorn > 3000 ) {

		}
	}

}
