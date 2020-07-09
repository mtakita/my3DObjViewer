#include "pch.h"

#include "GLRenderer.h"
#include <stdexcept>


//#include "GL\gl.h"
//#include "GL\glu.h"
//#include "GL\glaux.h"
//#include "GL\glut.h"

#include <GL/gl3w.h>



#include <iostream>
#include <vector>

#include "obj_reader/obj_reader.h"
#include "obj_reader/obj_reader_func.h"

#include "common/vmath.h"

#include "etc.h"
#include "logGLError.h"
#include "CDeviceContext.h"

extern "C" struct obj_reader_work* readObj(const char* objFileName);



#define BUFFER_OFFSET(pointer) ((void*)(pointer))

#include "mylibjpeg.h"

GLRenderer::GLRenderer(void)
{
}

GLRenderer::~GLRenderer(void)
{
}



/*
void GLRenderer::attachShaders(GLuint programId, ShaderInfo shaders[], int size) {

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
*/

//void GLRenderer::PrepareScene(CDC *pDC)
void GLRenderer::PrepareScene(CDeviceContext& devCon)
{
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC() );
/*
	//---------------------------------
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "shaders/vertext_shader.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/fragment_shader.flag" },
	};

	GLuint renderProgramId = glCreateProgram();
	logGlError();

	attachShaders(renderProgramId, shaders, sizeof(shaders) / sizeof(ShaderInfo));

	glLinkProgram(renderProgramId);
	logGlError();

	glUseProgram(renderProgramId);
	logGlError();

	render_model_matrix_loc = glGetUniformLocation(renderProgramId, "model_matrix");
	logGlError();

	render_projection_matrix_loc = glGetUniformLocation(renderProgramId, "projection_matrix");
	logGlError();
*/
	/*
	read a model data, suzannu.
	*/
	readObj("color_box.obj");
	//	readObj("log.obj");
	//	readObj("monkey.obj");

	numOfUseMaterials = getNumOfAttributeUseMaterials();

	pUseMaterialList = getAttributeUseMaterialList(*ppCurrentObj);


	/**/
	// Retriving vertices.
	/**/
	numOfVerticesSuzannu = getNumOfVertices();
	pVerticesSuzannu = (GLfloat*)malloc(sizeof(GLfloat) * 3 * numOfVerticesSuzannu);
	getVertices(pVerticesSuzannu);

	/**/
	// Retriving texture coordinates for each use material.
	/**/
//	GLfloat** pTexCoordsPerUseMaterial = (GLfloat**)malloc(sizeof(GLfloat*) * numOfUseMaterials);
	pTexCoordsPerUseMaterial = (GLfloat**)malloc(sizeof(GLfloat*) * numOfUseMaterials);
	ATTRIBUTE_USE_MATERIAL_LIST* pUseMaterialListForTexCoordisItor = pUseMaterialList;
	for (int i = 0; i < numOfUseMaterials; i++) {

		numOfTexCoords = getNumOfTexCoords();
		// GLfloat* pTexCoords = (GLfloat*)malloc(sizeof(GLfloat) * 2 * numOfTexCoords);
		pTexCoordsPerUseMaterial[i] = (GLfloat*)malloc(sizeof(GLfloat) * 2 * numOfVerticesSuzannu);
		//	getTexCoords(pTexCoords);
		getTexCoordsWithRearrange2(pTexCoordsPerUseMaterial[i], (*pUseMaterialListForTexCoordisItor).pUseMaterial);

		pUseMaterialListForTexCoordisItor = (*pUseMaterialListForTexCoordisItor).pNext;
	}

	/**/
	// Retriving normals for each use material.
	/**/
//	GLfloat** pNormalSuzannuPerUseMaterial = (GLfloat**)malloc(sizeof(GLfloat*) * numOfUseMaterials);
	pNormalSuzannuPerUseMaterial = (GLfloat**)malloc(sizeof(GLfloat*) * numOfUseMaterials);
	ATTRIBUTE_USE_MATERIAL_LIST* pUseMaterialListForNormalsItor = pUseMaterialList;
	for (int i = 0; i < numOfUseMaterials; i++) {

		pNormalSuzannuPerUseMaterial[i] = (GLfloat*)malloc(sizeof(GLfloat) * 3 * numOfVerticesSuzannu);

		//	getNormals(pNormalsSuzannu);
		getNormalsWithRearrange2(pNormalSuzannuPerUseMaterial[i], (*pUseMaterialListForNormalsItor).pUseMaterial);
		pUseMaterialListForNormalsItor = (*pUseMaterialListForNormalsItor).pNext;
	}

	//	numOfFaces = getNumOfFaces();
	numOfIndicesPerFace = getNumOfIndicesPerFace();

	lengthOfFaces = getLengthOfFacesVertexIndicies();
	pFacesVertexIndicies = (GLuint*)malloc(lengthOfFaces);
	//	getFacesVertexIndicies(pFacesVertexIndicies);

	/*
	Loading images from use material.
	*/

	ATTRIBUTE_USE_MATERIAL_LIST* pUseMaterialListForImagesItor = pUseMaterialList;
	for (int i = 0; i < numOfUseMaterials; i++) {

		try {
			struct UseMaterialTextureInfo* pUseMaterialTextureInfo;

			pUseMaterialTextureInfo = (struct UseMaterialTextureInfo*)malloc(sizeof(UseMaterialTextureInfo));
			memset(pUseMaterialTextureInfo, 0, sizeof(struct UseMaterialTextureInfo));

			useMaterialList.push_back(pUseMaterialTextureInfo);

			using namespace jpeglib;

			struct USE_MATERIAL* pUseMaterial = (*pUseMaterialListForImagesItor).pUseMaterial;

			struct jpeg_decompress_struct cinfo;
			struct my_error_mgr jerr;
			FILE * infile = 0;		/* source file */

			JSAMPARRAY buffer;		/* Output row buffer */
			int row_stride;		/* physical row width in output buffer */
			errno_t err = fopen_s(&infile, (*(*pUseMaterial).pMaterial).pMapKd, "r+b");
			if (err != 0) {
				fprintf(stderr, "can't open %s\n", (*(*pUseMaterial).pMaterial).pMapKd);
				return;
			}

			cinfo.err = jpeg_std_error(&jerr.pub);
			jerr.pub.error_exit = my_error_exit;
			if (setjmp(jerr.setjmp_buffer)) {
				jpeg_destroy_decompress(&cinfo);
				fclose(infile);
				return;
			}

			using namespace jpeglib;
			jpeg_create_decompress(&cinfo);

			jpeglib::jpeg_stdio_src(&cinfo, infile);

			(void)jpeg_read_header(&cinfo, TRUE);
			(void)jpeg_start_decompress(&cinfo);
			row_stride = cinfo.output_width * cinfo.output_components;
			buffer = (*cinfo.mem->alloc_sarray)
				((jpeglib::j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

			int bufferSize = cinfo.output_width * cinfo.output_height * cinfo.output_components;
			(*pUseMaterialTextureInfo).buffer = (GLubyte*)malloc(bufferSize);
			(*pUseMaterialTextureInfo).bufferSize = bufferSize;
			(*pUseMaterialTextureInfo).imageWidth = cinfo.output_width;
			(*pUseMaterialTextureInfo).imageHeight = cinfo.output_height;
			int current_row = 0;

			while (cinfo.output_scanline < cinfo.output_height) {
				(void)jpeg_read_scanlines(&cinfo, buffer, 1);

				memcpy((*pUseMaterialTextureInfo).buffer + current_row*row_stride, (*buffer), row_stride);
				current_row++;
			}

			(void)jpeg_finish_decompress(&cinfo);
			jpeg_destroy_decompress(&cinfo);
			fclose(infile);

			// 1. create texture
			glGenTextures(1, &(*pUseMaterialTextureInfo).texture);
			logGlError();

			// 2. create buffer
			glGenBuffers(1, &(*pUseMaterialTextureInfo).bufferObject);
			logGlError();

			// 3. bind buffer
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, (*pUseMaterialTextureInfo).bufferObject);
			logGlError();

			// 4. place source data into the buffer.
			glBufferData(GL_PIXEL_UNPACK_BUFFER,
				(*pUseMaterialTextureInfo).bufferSize,
				(*pUseMaterialTextureInfo).buffer,
				GL_STATIC_DRAW);
			logGlError();

			// 5.
			glBindTexture(GL_TEXTURE_2D, (*pUseMaterialTextureInfo).texture);
			logGlError();

			// 6. texture storage
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, // GL_RGBA8,
				(*pUseMaterialTextureInfo).imageWidth,
				(*pUseMaterialTextureInfo).imageHeight);
			logGlError();

			// 7. specify the data for the texture.
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
				(*pUseMaterialTextureInfo).imageWidth,
				(*pUseMaterialTextureInfo).imageHeight,
				GL_RGB, GL_UNSIGNED_BYTE,
				BUFFER_OFFSET(0));
			logGlError();

			static const GLint swizzles[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ONE };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles); logGlError();

			pUseMaterialListForImagesItor = (*pUseMaterialListForImagesItor).pNext;
		}
		catch (...) {

			std::cout << "An exception occured!" << std::endl;
		}
	}


	// Create a new buffer object.
	glGenBuffers(1, &buffer);
	logGlError();

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	logGlError();

	// Allocate a space for it.
	glBufferStorage(GL_ARRAY_BUFFER,
		//		sizeof(GLfloat) * 3 * ( numOfVerticesSuzannu + numOfTexCoords + numOfNormalsSuzannu ),
		sizeof(GLfloat) * 3 * (numOfVerticesSuzannu * 3) * numOfUseMaterials,
		nullptr,
		GL_DYNAMIC_STORAGE_BIT);
	logGlError();

	int bufferPosition = 0;

	// Put 'positions' at offset zero in the buffer.
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(GLfloat) * 3 * numOfVerticesSuzannu,
		pVerticesSuzannu);
	logGlError();
	bufferPosition += sizeof(GLfloat) * 3 * numOfVerticesSuzannu;

	for (int i = 0; i < numOfUseMaterials; i++) {

		int numOfComponent = 2;

		glBufferSubData(GL_ARRAY_BUFFER,
			bufferPosition,
			sizeof(GLfloat) * numOfComponent * numOfVerticesSuzannu,
			pTexCoordsPerUseMaterial[i]);
		logGlError();

		bufferPosition += sizeof(GLfloat) * numOfComponent * numOfVerticesSuzannu;
	}

	for (int i = 0; i < numOfUseMaterials; i++) {

		int numOfComponent = 3;

		glBufferSubData(GL_ARRAY_BUFFER,
			bufferPosition,
			sizeof(GLfloat) * numOfComponent * numOfVerticesSuzannu,
			pNormalSuzannuPerUseMaterial[i]);
		logGlError();

		bufferPosition += sizeof(GLfloat) * numOfComponent * numOfVerticesSuzannu;
	}

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

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

//void GLRenderer::DrawScene(CDC *pDC)
void GLRenderer::DrawScene(CDeviceContext& devCon)
{
//	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	try {

		float t = float(app_time() & 0x3FFFF) / float(0x3FFFF);

		vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f) * vmath::translate(0.0f, 0.0f, -10.0f));
		vmath::mat4 model_matrix(vmath::scale(2.3f) *
			vmath::rotate(t * 360.0f * 3.0f, 1.0f, 0.0f, 0.0f) *
			vmath::rotate(t * 360.0f * 3.0f, 0.0f, 1.0f, 0.0f) *
			vmath::rotate(t * 360.0f * 3.0f, 0.0f, 0.0f, 1.0f));

		float black[] = { 55,88,66,0 };
		float white[] = { 255,255,255,0 };
		float cyan[] = { 165,220,228,0 };
		float blue[] = { 71, 154, 209,0 };
		float one = 1.0f;

//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); logGlError(); logGlError();

		// Setup
		glEnable(GL_CULL_FACE); logGlError();
		glCullFace(GL_BACK); logGlError();
		glEnable(GL_DEPTH_TEST); logGlError();
		glDepthFunc(GL_LEQUAL); logGlError();

		glDepthMask(GL_TRUE); logGlError();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); logGlError();

		glUseProgram(renderProgramId);
		logGlError();

		glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix); logGlError();
		glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix); logGlError();

		int currentNumOfUseMaterialList = 0;

//		glClearBufferfv(GL_COLOR, 0, cyan); logGlError();



		glBindBuffer(GL_ARRAY_BUFFER, buffer); logGlError();

		glBindVertexArray(vaoid[vao]); logGlError();


		ATTRIBUTE_USE_MATERIAL_LIST* pUseMaterialListForMainItor = pUseMaterialList;
		while (pUseMaterialListForMainItor != NULL) {

			USE_MATERIAL* pUseMaterial = NULL;
			pUseMaterial = (*pUseMaterialListForMainItor).pUseMaterial;

			MATERIAL* pMaterial = NULL;
			ATTRIBUTE_F_LIST* pAttributeFList = NULL;

			pMaterial = (*pUseMaterial).pMaterial;
			pAttributeFList = (*pUseMaterial).pAttributeFList;

			GLuint bufferForIndex;

			glGenBuffers(1, &bufferForIndex);
			logGlError();

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferForIndex);
			logGlError();


			glBufferStorage(GL_ELEMENT_ARRAY_BUFFER,
				lengthOfFaces,
				nullptr,
				GL_DYNAMIC_STORAGE_BIT);
			logGlError();

			getVertexIndicies2PerFaces(pFacesVertexIndicies, pAttributeFList);

			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
				0,
				lengthOfFaces,
				pFacesVertexIndicies);
			logGlError();

			numOfFaces = getNumOfFaces2(pUseMaterial);

			glBindBuffer(GL_ARRAY_BUFFER, buffer); logGlError();

			/**/
			// Texture Coodinate Index
			/**/
			glVertexAttribPointer(vTexCoodIndex, 2, GL_FLOAT, GL_FALSE, 0, //sizeof(float) * 2,
				BUFFER_OFFSET(
					sizeof(GLfloat) * 3 * numOfVerticesSuzannu
					+ sizeof(GLfloat) * 2 * numOfVerticesSuzannu * currentNumOfUseMaterialList
				)
			);
			logGlError();

			glEnableVertexAttribArray(vTexCoodIndex);
			logGlError();

			/**/
			// Normals Index.
			/**/
			glVertexAttribPointer(vNormalIndex, 3, GL_FLOAT, GL_FALSE, 0, //sizeof(float) * 3,
				BUFFER_OFFSET(
					sizeof(GLfloat) * 3 * numOfVerticesSuzannu +
					sizeof(GLfloat) * 2 * numOfVerticesSuzannu * numOfUseMaterials +
					sizeof(GLfloat) * 3 * numOfVerticesSuzannu * currentNumOfUseMaterialList
				)
			);
			logGlError();

			glEnableVertexAttribArray(vNormalIndex);
			logGlError();

			struct UseMaterialTextureInfo* pUseMaterialTextureInfo = useMaterialList.at(currentNumOfUseMaterialList);
			glBindTexture(GL_TEXTURE_2D, (*pUseMaterialTextureInfo).texture); logGlError();

			glDrawElements(GL_TRIANGLES, numOfFaces * numOfIndicesPerFace, GL_UNSIGNED_INT, NULL); logGlError();


			glDisableVertexAttribArray(vTexCoodIndex);
			logGlError();

			glDisableVertexAttribArray(vNormalIndex);
			logGlError();

			//			glBindVertexArray(0); 	logGlError();

			// added start
			//			glBindTexture(GL_TEXTURE_2D, 0);  	logGlError();
			//			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);  	logGlError();
			//			glBindBuffer(GL_ARRAY_BUFFER, 0 ); logGlError();
			//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 ); logGlError();
			// added end

			glDeleteBuffers(1, &bufferForIndex);
			logGlError();

			currentNumOfUseMaterialList++;

			pUseMaterialListForMainItor = (*pUseMaterialListForMainItor).pNext;


		}

		//	glFlush();

	}
	catch (const std::out_of_range& oor) {

		TRACE(oor.what());

	}

//	SwapBuffers(pDC->m_hDC);
//	wglMakeCurrent(NULL, NULL);
}

//void GLRenderer::Reshape(CDC *pDC, int w, int h)
void GLRenderer::Reshape(CDeviceContext& devCon, int w, int h)
{
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	glViewport(0, 0, w, h);

	aspect = float(h) / float(w);


	//---------------------------------

//	glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);

//	glMatrixMode(GL_PROJECTION);

//	glLoadIdentity();

//	gluPerspective(45.0, (double)w / (double)h, 1, 2000);

//	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

//void GLRenderer::DestroyScene(CDC *pDC)
void GLRenderer::DestroyScene(CDeviceContext& devCon)
{
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (devCon.GetHGLRC() )
	{
		wglDeleteContext(devCon.GetHGLRC());
//		m_hrc = NULL;
	}
}

/*
void GLRenderer::SetupShaderParameters(GLuint renderProgramId, GLint render_model_matrix_loc, GLint render_projection_matrix_loc) {

	m_renderProgramId = renderProgramId;
	m_render_model_matrix_loc = render_model_matrix_loc;
	m_render_projection_matrix_loc = render_projection_matrix_loc;
}
*/

int GLRenderer::InitializeShaders(CDeviceContext& devCon) {

	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "shaders/vertext_shader.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/fragment_shader.flag" },
	};

	renderProgramId = glCreateProgram();
	logGlError();

	attachShaders(renderProgramId, shaders, sizeof(shaders) / sizeof(ShaderInfo));

	glLinkProgram(renderProgramId);
	logGlError();

	glUseProgram(renderProgramId);
	logGlError();

	render_model_matrix_loc = glGetUniformLocation(renderProgramId, "model_matrix");
	logGlError();

	render_projection_matrix_loc = glGetUniformLocation(renderProgramId, "projection_matrix");
	logGlError();

	wglMakeCurrent(NULL, NULL);

	return renderProgramId;
}

void GLRenderer::attachShaders(GLuint programId, ShaderInfo shaders[], int size) {

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
