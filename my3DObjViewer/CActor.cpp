#include "pch.h"
#include "CActor.h"
#include "WavefrontObjFile.h"
#include "logGLError.h"
#include "etc.h"
#include "CDeviceContext.h"
#include "CActorAABB.h"
#include "CAABB.h"
#include "CActorProperty.h"
#include <list>
#include "ShaderVariableNames.h"

using std::list;


CActor::CActor() {
	m_ActorId = actorIdGenerator.GetNextNumber();
	m_selectedFlag = false;
}

int CActor::getActorId() {
	return m_ActorId;
}

void CActor::prepareDrawing(CDeviceContext& devCon)
{
	// TODO: ここに実装コードを追加します.
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	glGenVertexArrays(num_vao, vaoid);
	logGlError();

	glBindVertexArray(vaoid[vao]);
	logGlError();

	/*
	Loading images from use material.
	*/

	for (string useMaterialName : useMaterialNameList) {

		UseMaterialTextureInfo* pUseMaterial = useMaterialList[useMaterialName];

		try {

			// 1. create texture
			glGenTextures(1, &pUseMaterial->texture);
			logGlError();

			// 2. create buffer
			glGenBuffers(1, &pUseMaterial->bufferObject);
			logGlError();

			// 3. bind buffer
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pUseMaterial->bufferObject);
			logGlError();

			// 4. place source data into the buffer.
			glBufferData(GL_PIXEL_UNPACK_BUFFER,
				pUseMaterial->bufferSize,
				pUseMaterial->buffer,
				GL_STATIC_DRAW);
			logGlError();

			// 5.
			glBindTexture(GL_TEXTURE_2D, pUseMaterial->texture);
			logGlError();

			// 6. texture storage
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, // GL_RGBA8,
				pUseMaterial->imageWidth,
				pUseMaterial->imageHeight);
			logGlError();

			// 7. specify the data for the texture.
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
				pUseMaterial->imageWidth,
				pUseMaterial->imageHeight,
				GL_RGB, GL_UNSIGNED_BYTE,
				BUFFER_OFFSET(0));
			logGlError();

			static const GLint swizzles[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ONE };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles); logGlError();

			//			pUseMaterialListForImagesItor = (*pUseMaterialListForImagesItor).pNext;
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
		//		sizeof(GLfloat) * 3 * ( numOfVertices + numOfTexCoords + numOfNormalsSuzannu ),
		sizeof(GLfloat) * 3 * (numOfVertices * 3) * numOfUseMaterials,
		nullptr,
		GL_DYNAMIC_STORAGE_BIT);
	logGlError();

	int bufferPosition = 0;

	/**/
	// Vertex non-index.
	/**/
	// Put 'positions' at offset zero in the buffer.
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(GLfloat) * 3 * numOfVertices,
		pVertices);
	logGlError();
	bufferPosition += sizeof(GLfloat) * 3 * numOfVertices;


	/**/
	// Texture coordinates non-index.
	/**/
	for (int i = 0; i < numOfUseMaterials; i++) {

		int numOfComponent = 2;

		glBufferSubData(GL_ARRAY_BUFFER,
			bufferPosition,
			sizeof(GLfloat) * numOfComponent * numOfVertices,
			pTexCoordsPerUseMaterial[i]);
		logGlError();


		bufferPosition += sizeof(GLfloat) * numOfComponent * numOfVertices;
	}


	/**/
	// Normal non-index.
	/**/
	for (int i = 0; i < numOfUseMaterials; i++) {

		int numOfComponent = 3;

		glBufferSubData(GL_ARRAY_BUFFER,
			bufferPosition,
			sizeof(GLfloat) * numOfComponent * numOfVertices,
			pNormalSuzannuPerUseMaterial[i]);
		logGlError();

		bufferPosition += sizeof(GLfloat) * numOfComponent * numOfVertices;
	}

	wglMakeCurrent(NULL, NULL);

}


void CActor::prepareDrawing2( CDeviceContext& devCon )
{
	// TODO: ここに実装コードを追加します.
	wglMakeCurrent( devCon.GetDC()->m_hDC, devCon.GetHGLRC() );

	glGenVertexArrays(num_vao, vaoid);
	logGlError();

	glBindVertexArray(vaoid[vao]);
	logGlError();
		
	/*
	Loading images from use material.
	*/

	for ( string useMaterialName : useMaterialNameList) {

		UseMaterialTextureInfo* pUseMaterial = useMaterialList[useMaterialName];

		try {

			//
			// Prepare texture images.
			//

			// 1. create texture
			glGenTextures(1, &pUseMaterial->texture );
			logGlError();

			// 2. create buffer
			glGenBuffers(1, &pUseMaterial->bufferObject);
			logGlError();

			// 3. bind buffer
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pUseMaterial->bufferObject);
			logGlError();

			// 4. place source data into the buffer.
			glBufferData(GL_PIXEL_UNPACK_BUFFER,
				pUseMaterial->bufferSize,
				pUseMaterial->buffer,
				GL_STATIC_DRAW);
			logGlError();

			// 5.
			glBindTexture(GL_TEXTURE_2D, pUseMaterial->texture);
			logGlError();

			// 6. texture storage
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, // GL_RGBA8,
				pUseMaterial->imageWidth,
				pUseMaterial->imageHeight);
			logGlError();

			// 7. specify the data for the texture.
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
				pUseMaterial->imageWidth,
				pUseMaterial->imageHeight,
				GL_RGB, GL_UNSIGNED_BYTE,
				BUFFER_OFFSET(0));
			logGlError();

			static const GLint swizzles[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ONE };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles); logGlError();

			//
			// Prepare for element drawing.
			//

			glGenBuffers(1, &bufferForIndecies[useMaterialName]);
			logGlError();

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferForIndecies[useMaterialName]);
			logGlError();

			glBufferStorage(GL_ELEMENT_ARRAY_BUFFER,
				this->bytesOfUnifiedIndecies[useMaterialName],
				nullptr,
				GL_DYNAMIC_STORAGE_BIT);
			logGlError();

			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
				0,
				this->bytesOfUnifiedIndecies[useMaterialName],
				this->pBufferForUnifiedIndecies[useMaterialName]);
			logGlError();
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


	int bytesOfBufferSizeForVerticesUVsNormals = 
		  this->bytesOfBufferSizeForVertices
		+ this->bytesOfBufferSizeForUVs
		+ this->bytesOfBufferSizeForNormals;

	// Allocate a space for it.
	glBufferStorage(GL_ARRAY_BUFFER,
		bytesOfBufferSizeForVerticesUVsNormals,
		nullptr,
		GL_DYNAMIC_STORAGE_BIT);
	logGlError();


	/**/
	// Verties.
	/**/
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		this->bytesOfBufferSizeForVertices,
		this->pBufferForVerties);
	logGlError();


	/**/
	// Texture coordinates.
	/**/
	glBufferSubData(GL_ARRAY_BUFFER,
		this->bytesOfBufferSizeForVertices,
		this->bytesOfBufferSizeForUVs,
		this->pBufferForUVs);
	logGlError();


	/**/
	// Normal.
	/**/
	glBufferSubData(GL_ARRAY_BUFFER,
		this->bytesOfBufferSizeForVertices
		+ this->bytesOfBufferSizeForUVs,
		this->bytesOfBufferSizeForNormals,
		this->pBufferForNormals);
	logGlError();

	//
	// Vertex attribute array pointer setup start.
	//

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	logGlError();

	int bufferPosition = 0;

	/**/
	// Vertex non-index.
	/**/
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferPosition)); logGlError();
	bufferPosition += this->bytesOfBufferSizeForVertices;

	/**/
	// Texture coordinates non-index.
	/**/
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferPosition)); logGlError();
	bufferPosition += this->bytesOfBufferSizeForUVs;

	/**/
	// Normal non-index.
	/**/
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferPosition)); logGlError();


	//
	// Prepare texture for fur geometry shader will generate.
	//
	glGenTextures(1, &fur_texture);

	int dimension = 1024;
	int furBuffSize = dimension * dimension * 4;
	unsigned char* pTextureBuff = (unsigned char*)malloc(furBuffSize);

	memset(pTextureBuff, 0, dimension * dimension * 4);

	for (int n = 0; n < dimension; n++) {
		for (int m = 0; m < 270; m++) {
			int x = rand() & 0x3ff;
			int y = rand() & 0x3ff;

			pTextureBuff[(y * dimension + x) * 4 + 0] = (rand() & 0x3F) + 0xC0;
			pTextureBuff[(y * dimension + x) * 4 + 1] = (rand() & 0x3F) + 0xC0;
			pTextureBuff[(y * dimension + x) * 4 + 2] = (rand() & 0x3F) + 0xC0;
			pTextureBuff[(y * dimension + x) * 4 + 3] = n;

		}
	}

	glGenBuffers(1, &fur_bufferObject); logGlError();
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, fur_bufferObject); logGlError();
	glBufferData(GL_PIXEL_UNPACK_BUFFER, furBuffSize, pTextureBuff, GL_STATIC_DRAW); logGlError();

	glBindTexture(GL_TEXTURE_2D, fur_texture); logGlError();

	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimension, dimension, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTextureBuff); logGlError();
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, dimension, dimension ); logGlError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, dimension, dimension, GL_RGBA, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0)); logGlError();


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); logGlError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); logGlError();

	static const GLint swizzles[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA };
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles); logGlError();

	free(pTextureBuff);



	wglMakeCurrent(NULL, NULL);

}

/*
Obtain a number of faces holding.
*/
int CActor::getNumOfFaces2( struct USE_MATERIAL* pUseMaterial ) {

	int numOfFaces = 0;

	if ((*pUseMaterial).pAttributeFList) {

		struct ATTRIBUTE_F_LIST* pAttributeFList = (*pUseMaterial).pAttributeFList;

		while (pAttributeFList != NULL) {

			numOfFaces++;
			pAttributeFList = (*pAttributeFList).pNext;
		}

	}

	return numOfFaces;
}

void CActor::RenderBoundary(CDeviceContext& devCon, Matrixf4x4 projectionMatrix, Matrixf4x4 model_matrix )
{
	//	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC() );

	try {

		glUseProgram(renderProgramIdForBoundary);
		logGlError();

		// save before change.
		GLboolean bCullFace;
		bCullFace = glIsEnabled(GL_CULL_FACE);

		GLint cullFace;
		glGetIntegerv(GL_CULL_FACE_MODE, &cullFace); logGlError();

		// enable cull face.
		glEnable(GL_CULL_FACE); logGlError();

		// change cullface for drawing boundrary.
		glCullFace(GL_FRONT); logGlError();

		glUniformMatrix4fv(render_model_matrix_loc_bdry, 1, GL_TRUE, model_matrix); logGlError();
		glUniformMatrix4fv(render_projection_matrix_loc_bdry, 1, GL_TRUE, projectionMatrix); logGlError();


		glBindVertexArray(vaoid[vao]);		logGlError();
		glBindBuffer(GL_ARRAY_BUFFER, buffer); logGlError();

		for (string useMaterialName : useMaterialNameList) {

			UseMaterialTextureInfo* pUseMaterialTextureInfo = useMaterialList[useMaterialName];
			int currentNumOfUseMaterialList = useMaterialNameListIndex[useMaterialName];

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferForIndecies[useMaterialName]);
			logGlError();

			//
			// Vertex attribute array pointer setup start.
			//

			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			logGlError();

			int bufferPosition = 0;

			glEnableVertexAttribArray(vPosition); logGlError();
			glEnableVertexAttribArray(vTexCoord); logGlError();
			glEnableVertexAttribArray(vNormal); logGlError();

			glBindTexture(GL_TEXTURE_2D, (*pUseMaterialTextureInfo).texture); logGlError();

			glDrawElements(GL_TRIANGLES, this->numOfUnifiedIndecies[useMaterialName], GL_UNSIGNED_INT, BUFFER_OFFSET(0)); logGlError();

		}

		// restore cullface.

		if (bCullFace) {
			glEnable(GL_CULL_FACE); logGlError();
		}
		else {
			glDisable(GL_CULL_FACE); logGlError();
		}

		glCullFace(cullFace); logGlError();

	}
	catch (const std::out_of_range & oor) {

		TRACE(oor.what());

	}

	//	wglMakeCurrent(NULL, NULL);

}


void CActor::RenderRegular(CDeviceContext& devCon, Matrixf4x4 projectionMatrix, Matrixf4x4 model_matrix)
{
	//	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC() );

	try {

		GLint render_model_matrix_loc;
		GLint render_projection_matrix_loc;
		GLint fur_layers_loc;

		GLint tessellation_level_inner_0_loc;
		GLint tessellation_level_inner_1_loc;
		GLint tessellation_level_outer_0_loc;
		GLint tessellation_level_outer_1_loc;
		GLint tessellation_level_outer_2_loc;
		GLint tessellation_level_outer_3_loc;

		if (GetTessellationOnOff() == true) {
			glUseProgram(renderProgramIdTessellation);
			logGlError();

			render_model_matrix_loc = glGetUniformLocation(renderProgramIdTessellation, ACTOR_VERT_model_matrix);
			logGlError();

			render_projection_matrix_loc = glGetUniformLocation(renderProgramIdTessellation, ACTOR_VERT_projection_matrix);
			logGlError();

			tessellation_level_inner_0_loc = glGetUniformLocation(renderProgramIdTessellation, ACTOR_TESSCTRL_tessellation_level_inner_0);
			logGlError();
			tessellation_level_inner_1_loc = glGetUniformLocation(renderProgramIdTessellation, ACTOR_TESSCTRL_tessellation_level_inner_1);
			logGlError();
			tessellation_level_outer_0_loc = glGetUniformLocation(renderProgramIdTessellation, ACTOR_TESSCTRL_tessellation_level_outer_0);
			logGlError();
			tessellation_level_outer_1_loc = glGetUniformLocation(renderProgramIdTessellation, ACTOR_TESSCTRL_tessellation_level_outer_1);
			logGlError();
			tessellation_level_outer_2_loc = glGetUniformLocation(renderProgramIdTessellation, ACTOR_TESSCTRL_tessellation_level_outer_2);
			logGlError();
			tessellation_level_outer_3_loc = glGetUniformLocation(renderProgramIdTessellation, ACTOR_TESSCTRL_tessellation_level_outer_3);
			logGlError();
		}
		else {
			glUseProgram(renderProgramId);
			logGlError();

			render_model_matrix_loc = glGetUniformLocation(renderProgramId, ACTOR_VERT_model_matrix);
			logGlError();

			render_projection_matrix_loc = glGetUniformLocation(renderProgramId, ACTOR_VERT_projection_matrix);
			logGlError();
		}

		glUniformMatrix4fv(render_model_matrix_loc, 1, GL_TRUE, model_matrix); logGlError();
		glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_TRUE, projectionMatrix); logGlError();


		glBindVertexArray(vaoid[vao]);		logGlError();
		glBindBuffer(GL_ARRAY_BUFFER, buffer); logGlError();

		for (string useMaterialName : useMaterialNameList) {

			UseMaterialTextureInfo* pUseMaterialTextureInfo = useMaterialList[useMaterialName];
			int currentNumOfUseMaterialList = useMaterialNameListIndex[useMaterialName];

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferForIndecies[useMaterialName]);
			logGlError();

			//
			// Vertex attribute array pointer setup start.
			//

			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			logGlError();

			int bufferPosition = 0;

			glEnableVertexAttribArray(vPosition); logGlError();
			glEnableVertexAttribArray(vTexCoord); logGlError();
			glEnableVertexAttribArray(vNormal); logGlError();

			glBindTexture(GL_TEXTURE_2D, (*pUseMaterialTextureInfo).texture); logGlError();
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, fur_bufferObject); logGlError();

			if (GetTessellationOnOff() ) {

				GLfloat tessLevelFactorOuter0 = GetTessellationLevelFactorOuter0();
				GLfloat tessLevelFactorOuter1 = GetTessellationLevelFactorOuter1();
				GLfloat tessLevelFactorOuter2 = GetTessellationLevelFactorOuter2();
				GLfloat tessLevelFactorOuter3 = GetTessellationLevelFactorOuter3();
				GLfloat tessLevelFactorInner0 = GetTessellationLevelFactorInner0();
				GLfloat tessLevelFactorInner1 = GetTessellationLevelFactorInner1();

				glUniform1f(tessellation_level_inner_0_loc, tessLevelFactorOuter0); logGlError();
				glUniform1f(tessellation_level_inner_1_loc, tessLevelFactorOuter1); logGlError();
				glUniform1f(tessellation_level_outer_0_loc, tessLevelFactorOuter2); logGlError();
				glUniform1f(tessellation_level_outer_1_loc, tessLevelFactorOuter3); logGlError();
				glUniform1f(tessellation_level_outer_2_loc, tessLevelFactorInner0); logGlError();
				glUniform1f(tessellation_level_outer_3_loc, tessLevelFactorInner1); logGlError();

				glPatchParameteri(GL_PATCH_VERTICES, 3);

				static GLenum mode = GL_LINE;
				glPolygonMode(GL_FRONT_AND_BACK, mode);

				glDrawElements(GL_PATCHES, this->numOfUnifiedIndecies[useMaterialName], GL_UNSIGNED_INT, BUFFER_OFFSET(0)); logGlError();
			}
			else {

				static GLenum mode = GL_FILL;
				glPolygonMode(GL_FRONT_AND_BACK, mode);

				glDrawElements(GL_TRIANGLES, this->numOfUnifiedIndecies[useMaterialName], GL_UNSIGNED_INT, BUFFER_OFFSET(0)); logGlError();
			}

		}

		//
		// Geometry shader
		//

		if (GetFurGeometryOnOff() == true) {

			glUseProgram(renderProgramIdFurGeometry);
			logGlError();

			render_model_matrix_loc = glGetUniformLocation(renderProgramIdFurGeometry, ACTOR_VERT_model_matrix);
			logGlError();

			render_projection_matrix_loc = glGetUniformLocation(renderProgramIdFurGeometry, ACTOR_VERT_projection_matrix);
			logGlError();

			fur_layers_loc = glGetUniformLocation(renderProgramIdFurGeometry, ACTOR_GEO_fur_layers); logGlError();

			glUniformMatrix4fv(render_model_matrix_loc, 1, GL_TRUE, model_matrix); logGlError();
			glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_TRUE, projectionMatrix); logGlError();
			glUniform1f(fur_layers_loc, 30); logGlError();

			for (string useMaterialName : useMaterialNameList) {

				UseMaterialTextureInfo* pUseMaterialTextureInfo = useMaterialList[useMaterialName];
				int currentNumOfUseMaterialList = useMaterialNameListIndex[useMaterialName];

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferForIndecies[useMaterialName]);
				logGlError();

				//
				// Vertex attribute array pointer setup start.
				//

				glBindBuffer(GL_ARRAY_BUFFER, buffer);
				logGlError();

				int bufferPosition = 0;

				glEnableVertexAttribArray(vPosition); logGlError();
				glEnableVertexAttribArray(vTexCoord); logGlError();
				glEnableVertexAttribArray(vNormal); logGlError();

				glBindTexture(GL_TEXTURE_2D, fur_texture); logGlError();

				static GLenum mode = GL_FILL;
				glPolygonMode(GL_FRONT_AND_BACK, mode);


				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glDepthMask(GL_FALSE);

				glDrawElements(GL_TRIANGLES, this->numOfUnifiedIndecies[useMaterialName], GL_UNSIGNED_INT, BUFFER_OFFSET(0)); logGlError();

				glDisable(GL_BLEND);
				glDepthMask(GL_TRUE);

			}
		}

	}
	catch (const std::out_of_range & oor) {

		TRACE(oor.what());

	}

	//	wglMakeCurrent(NULL, NULL);

}

void CActor::LinkRegularShaderProgram() {

	renderProgramId = glCreateProgram();
	logGlError();

	glAttachShader(renderProgramId, m_regular_vertex_shader);
	logGlError();

	glAttachShader(renderProgramId, m_regular_fragment_shader);
	logGlError();

	glLinkProgram(renderProgramId);
	LinkError(renderProgramId);

}

void CActor::LinkGeometryShaderProgram() {

	renderProgramIdFurGeometry = glCreateProgram();
	logGlError();

	glAttachShader(renderProgramIdFurGeometry, m_fur_vertex_shader);
	logGlError();

	glAttachShader(renderProgramIdFurGeometry, m_fur_fragment_shader);
	logGlError();

	glAttachShader(renderProgramIdFurGeometry, m_fur_geometry_shader );
	logGlError();

	glLinkProgram(renderProgramIdFurGeometry);
	LinkError(renderProgramIdFurGeometry);

}

void CActor::LinkRegularTessellationShaderProgram() {

	renderProgramIdTessellation = glCreateProgram();
	logGlError();

	glAttachShader(renderProgramIdTessellation, m_tessellation_vertex_shader);
	logGlError();

	glAttachShader(renderProgramIdTessellation, m_tessellation_fragment_shader);
	logGlError();

	glAttachShader(renderProgramIdTessellation, m_tessellation_control_shader);
	logGlError();

	glAttachShader(renderProgramIdTessellation, m_tessellation_evaluation_shader);
	logGlError();

	glLinkProgram(renderProgramIdTessellation);
	LinkError(renderProgramIdTessellation);

}

void CActor::LinkError(GLuint inProgramId ) {

	GLsizei size;
	GLchar log[256];

	GLint linkStatus;
	glGetProgramiv(inProgramId, GL_LINK_STATUS, &linkStatus);
	logGlError();

	glGetProgramInfoLog(inProgramId,
		sizeof(log),
		&size,
		log);
	logGlError();


}

void CActor::LinkBoundaryShaderProgram() {

	renderProgramIdForBoundary = glCreateProgram();
	logGlError();

	glAttachShader(renderProgramIdForBoundary, m_boundary_vertex_shader);
	logGlError();

	glAttachShader(renderProgramIdForBoundary, m_boundary_fragment_shader);
	logGlError();

	glLinkProgram(renderProgramIdForBoundary);
	logGlError();

	render_model_matrix_loc_bdry = glGetUniformLocation(renderProgramIdForBoundary, ACTOR_BDRY_VERT_model_matrix);
	logGlError();

	render_projection_matrix_loc_bdry = glGetUniformLocation(renderProgramIdForBoundary, ACTOR_BDRY_VERT_projection_matrix);
	logGlError();

}

void CActor::InitializeShaders(CDeviceContext& devCon) {

	InitializeAllShaders( devCon );

	LinkRegularShaderProgram();

	LinkGeometryShaderProgram();

	LinkRegularTessellationShaderProgram();

	LinkBoundaryShaderProgram();
	
}



//void GLRenderer::Reshape(CDC *pDC, int w, int h)
void CActor::Reshape(CDeviceContext& devCon, int w, int h)
{
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	glViewport(0, 0, w, h);

	aspect = float(h) / float(w);


	wglMakeCurrent(NULL, NULL);
}


CActorAABB* CActor::createActorAABB() {

	float xMin;
	float yMin;
	float zMin;

	float xMax;
	float yMax;
	float zMax;

	getAABB( xMin, yMin, zMin, xMax, yMax, zMax);

	CActorAABB* pAabb = new CActorAABB(this, getActorId(), xMin, yMin, zMin, xMax, yMax, zMax);
	return pAabb;
}

CAABB* CActor::createAABB(CActorAABB& actorAABB ) {

	CAABB* pAABB = new CAABB(this);

	pAABB->m_xMax = actorAABB.m_xMax;
	pAABB->m_xMin = actorAABB.m_xMin;
	pAABB->m_yMax = actorAABB.m_yMax;
	pAABB->m_yMin = actorAABB.m_yMin;
	pAABB->m_zMax = actorAABB.m_zMax;
	pAABB->m_zMin = actorAABB.m_zMin;

	return pAABB;
}

void CActor::getAABB(float& xMin, float& yMin, float& zMin, float& xMax, float& yMax, float& zMax )
{
	// TODO: ここに実装コードを追加します.

	xMin = std::numeric_limits<float>::infinity();
	yMin = std::numeric_limits<float>::infinity();
	zMin = std::numeric_limits<float>::infinity();

	xMax = -std::numeric_limits<float>::infinity();
	yMax = -std::numeric_limits<float>::infinity();
	zMax = -std::numeric_limits<float>::infinity();

	int numOfVertices = numFloatOfBufferSizeForVertices / 3.0f;

	for (int i = 0; i < numOfVertices; i++) {
		float x = pBufferForVerties[i*3   ];
		float y = pBufferForVerties[i*3 +1];
		float z = pBufferForVerties[i*3 +2];

		if (x < xMin) { xMin = x; }
		if (y < yMin) { yMin = y; }
		if (z < zMin) { zMin = z; }

		if (x > xMax) { xMax = x; }
		if (y > yMax) { yMax = y; }
		if (z > zMax) { zMax = z; }
	}
}

void CActor::SelectActor() {

	m_selectedFlag = true;

}

void CActor::DeselectActor() {
	m_selectedFlag = false;
}


IncreasingNumber CActor::actorIdGenerator{};

void CActor::setActorPropertiesWnd(CActorPropertiesWnd* pActorPropertiesWnd){
	m_pActorPropertiesWnd = pActorPropertiesWnd;
}

//void CActor::Render2(CDeviceContext& devCon, Matrixf4x4 projectionMatrix, CViewportProperty& viewportTranslation)
void CActor::Render2(CDeviceContext& devCon, Matrixf4x4 projectionMatrix )
{
	// import actor property.
	CActorProperty actorPropertyWnd;
	m_pActorPropertiesWnd->getActorProperty(actorPropertyWnd); // property may not be mine.
	importActorProperty(actorPropertyWnd);

	// prepare shaders.
	prepareShadersBeforeRender(actorPropertyWnd);

	// store flags to ideytify necessity of shader loading for next frame.
	SetPrevSelectedFlag(m_selectedFlag);
	SetPrevTessellationOnOff(actorPropertyWnd.GetTessellationOnOff());


	if (m_selectedFlag == true) {

		Matrixf4x4 model_matrix = mymath::translate2( GetPosX(), GetPosY(), GetPosZ() );

		// Add boundary to render image.
		RenderBoundary(devCon, projectionMatrix, model_matrix );
		RenderRegular(devCon, projectionMatrix, model_matrix);
	}
	else {

		Matrixf4x4 model_matrix = mymath::translate2(GetPosX(), GetPosY(), GetPosZ());

		// Render normally.
		RenderRegular(devCon, projectionMatrix, model_matrix);

	}

}

void CActor::setupTessellationShaders( list<GLuint>* outShaders ) {

	list<GLuint> shaders = { 
		m_tessellation_control_shader, 
		m_tessellation_evaluation_shader
	};

	for (GLuint shaderid : shaders) {
		(*outShaders).push_back(shaderid);
	}

	return;

}

void CActor::setupBoundaryShaders( list<GLuint>* outShaders ) {

	list<GLuint> shaders = { 
		m_boundary_vertex_shader, 
		m_boundary_fragment_shader  
	};

	for (GLuint shaderid : shaders) {
		(*outShaders).push_back(shaderid);
	}

	return;

	render_model_matrix_loc_bdry = glGetUniformLocation(renderProgramId, ACTOR_BDRY_VERT_model_matrix);
	logGlError();

	render_projection_matrix_loc_bdry = glGetUniformLocation(renderProgramId, ACTOR_BDRY_VERT_projection_matrix);
	logGlError();

}


void CActor::replaceShaders( list<GLuint> inNewShaders, GLuint inProgramId ) {

	int numOfShaders = inNewShaders.size();

	GLsizei size;
	GLuint shaderIds[10];
	vector<GLuint> shadersTobeRemoved;

	glGetAttachedShaders(
		inProgramId,
		sizeof(shaderIds)/ sizeof(GLuint), 
		&size, 
		shaderIds);

	//
	// detach shaders from attahced shaders.
	//

	for (int i = 0; i < size; i++) {

		boolean found = false;
		for (GLuint shaderId : inNewShaders) {
			if (shaderId == shaderIds[i]) {
				found = true;
				break;
			}
		}

		if (found == false) {

			// detach this shader.
			glDetachShader(inProgramId, shaderIds[i]);
			logGlError();
		}
	}

	//
	// attach shaders.
	//

	for (GLuint shaderId : inNewShaders) {

		boolean found = false;
		for (int i = 0; i < size; i++) {

			if (shaderId == shaderIds[i]) {
				found = true;
				break;
			}
		}

		if (found == false) {

			// attach shader.
			glAttachShader(inProgramId, shaderId );
			logGlError();
		}
	}

}

void CActor::setupRegularShaders( list<GLuint>* outShaders ) {

	list<GLuint> shaders = { 
		m_regular_vertex_shader, 
		m_regular_fragment_shader 
	};

	for(GLuint shaderid : shaders) {
		(*outShaders).push_back(shaderid);
	}

	return;

}

boolean CActor::GetPrevSelectedFlag() {

	return m_prevSelectedFlag;
}

boolean CActor::GetPrevTessellationOnOff() {

	return m_prevTessellationOnOff;
}

void CActor::SetPrevSelectedFlag( boolean inSelectedFlag ) {

	m_prevSelectedFlag = inSelectedFlag;
}

void CActor::SetPrevTessellationOnOff( boolean inTessellationOnOff ) {

	m_prevTessellationOnOff = inTessellationOnOff;
}

boolean CActor::shaderNeedsUpdate(CActorProperty inActorProperty) {

	if (GetPrevTessellationOnOff() != inActorProperty.GetTessellationOnOff() ||
		GetPrevSelectedFlag() != m_selectedFlag) {

		return true;
	}

	return false;

}

void CActor::prepareShadersBeforeRender( CActorProperty inActorProperty ) {

	int actorShaderType = 0;

	list<GLuint> regularShaders;
	list<GLuint> boundaryShaders;

	if (shaderNeedsUpdate(inActorProperty) == true) {

		//
		// identify necessary shaders.
		//

		if (m_selectedFlag == true) {
			actorShaderType |= CACTOR_SHADER_SELECTED_FLAG;
		}

		if (inActorProperty.GetTessellationOnOff() == true) {
			actorShaderType |= CACTOR_SHADER_TESSELLATION_FLAG;
		}

		switch (actorShaderType) {
		case 0x0000:
		case CACTOR_SHADER_SELECTED_FLAG:

			regularShaders.push_back(m_regular_vertex_shader);
			regularShaders.push_back(m_regular_fragment_shader);

			boundaryShaders.push_back(m_boundary_vertex_shader);
			boundaryShaders.push_back(m_boundary_fragment_shader);

			break;

		case CACTOR_SHADER_SELECTED_FLAG | CACTOR_SHADER_TESSELLATION_FLAG:
		case CACTOR_SHADER_TESSELLATION_FLAG:

			regularShaders.push_back(m_regular_vertex_shader);
			regularShaders.push_back(m_regular_fragment_shader);
			regularShaders.push_back(m_tessellation_control_shader);
			regularShaders.push_back(m_tessellation_evaluation_shader);

			boundaryShaders.push_back(m_boundary_vertex_shader);
			boundaryShaders.push_back(m_boundary_fragment_shader);
			boundaryShaders.push_back(m_tessellation_control_shader);
			boundaryShaders.push_back(m_tessellation_evaluation_shader);

			break;

		}

		replaceShaders(regularShaders, renderProgramId );
		replaceShaders(boundaryShaders, renderProgramIdForBoundary );

	}

	//
	// try to retrieve shaders variables.
	//

	glLinkProgram(renderProgramId);
	logGlError();

	glLinkProgram(renderProgramIdForBoundary);
	logGlError();

	glUseProgram(0);
	logGlError();

	glUseProgram(renderProgramId);
	logGlError();

	glUseProgram(renderProgramIdForBoundary);
	logGlError();

	render_model_matrix_loc_bdry = glGetUniformLocation(renderProgramIdForBoundary, ACTOR_BDRY_VERT_model_matrix);
	logGlError();

	render_projection_matrix_loc_bdry = glGetUniformLocation(renderProgramIdForBoundary, ACTOR_BDRY_VERT_projection_matrix);
	logGlError();


}

void CActor::importActorProperty( CActorProperty inActorProperty ) {
	//
	// retrive properties from actor property panel if updated.
	//

	if ( m_selectedFlag == true && 
		m_pActorPropertiesWnd->getPropertyUpdateFlag() == true) {

		//
		// Retrieve new values from actor property.
		//

		SetPosX(inActorProperty.GetActorTranslationX());
		SetPosY(inActorProperty.GetActorTranslationY());
		SetPosZ(inActorProperty.GetActorTranslationZ());

		SetScale(inActorProperty.GetActorScaleK());

		SetRotateX(inActorProperty.GetActorRotationX());
		SetRotateY(inActorProperty.GetActorRotationY());
		SetRotateZ(inActorProperty.GetActorRotationZ());

		SetTessellationOnOff(inActorProperty.GetTessellationOnOff());
		SetTessellationDomain(inActorProperty.GetTessellationDomain());
		SetTessellationLevelFactorOuter0(inActorProperty.GetTessellationLevelFactorOuter0());
		SetTessellationLevelFactorOuter1(inActorProperty.GetTessellationLevelFactorOuter1());
		SetTessellationLevelFactorOuter2(inActorProperty.GetTessellationLevelFactorOuter2());
		SetTessellationLevelFactorOuter3(inActorProperty.GetTessellationLevelFactorOuter3());
		SetTessellationLevelFactorInner0(inActorProperty.GetTessellationLevelFactorInner0());
		SetTessellationLevelFactorInner1(inActorProperty.GetTessellationLevelFactorInner1());

		SetFurGeometryOnOff(inActorProperty.GetGeometryOnOff());

		// Clear the flag.
		m_pActorPropertiesWnd->clearPropertyUpdateFlag();
	}


}

void CActor::Render2(CDeviceContext& devCon, Matrixf4x4 projectionMatrix, Matrixf4x4 translationMatrix)
{
	// retrieve actor property.(maybe some other actor's, not mine)
	CActorProperty actorPropertyWnd;
	m_pActorPropertiesWnd->getActorProperty(actorPropertyWnd);
	importActorProperty(actorPropertyWnd);

	// store flags to ideytify necessity of shader loading for next frame.
	SetPrevSelectedFlag(m_selectedFlag);
	SetPrevTessellationOnOff(GetTessellationOnOff());


	Matrixf4x4 transMat = GetTranslateMatrix4x4_2();
	Matrixf4x4 rotMat = GetRotateMatrix4x4Rev();
	Matrixf4x4 scaleMat = GetScaleMatrix4x4();

	Matrixf4x4 model_matrix = scaleMat * rotMat * transMat;

	if (m_selectedFlag == true) {

		// Add boundary to render image.
		RenderBoundary(devCon, projectionMatrix, model_matrix * translationMatrix);
		//		RenderRegular(devCon, projectionMatrix, viewportTranslation);
		RenderRegular(devCon, projectionMatrix, model_matrix * translationMatrix );
	}
	else {

		// Render normally.
		RenderRegular(devCon, projectionMatrix, model_matrix * translationMatrix );

	}

}

void CActor::InitializeAllShaders(CDeviceContext& devCon) {

	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	compileShaders(renderProgramId, shaders, sizeof(shaders) / sizeof(ShaderInfo2));

}

void CActor::compileShaders(GLuint programId, ShaderInfo2 shaders[], int size) {

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

		*shaders[i].pShaderId = shaderId;

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
	}
}

