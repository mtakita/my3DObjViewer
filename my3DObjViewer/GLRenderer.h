#pragma once

#include <GL/gl3w.h>
#include <vector>
#include "CDeviceContext.h"
#include "etc.h"

class GLRenderer
{
	struct UseMaterialTextureInfo {
		GLuint texture;
		GLubyte* buffer;
		int bufferSize;

		int imageWidth;
		int imageHeight;

		GLuint bufferObject;

	};

public:
	GLRenderer();
	~GLRenderer();

//	bool SetGLContext(CDC* pDC, HGLRC hrc);
//	bool CreateGLContext(CDC* pDC);

//	void PrepareScene(CDC* pDC);
//	void Reshape(CDC* pDC, int w, int h);
//	void DrawScene(CDC* pDC);
//	void DestroyScene(CDC* pDC);

	void PrepareScene(CDeviceContext& devCon);
	void Reshape(CDeviceContext& devCon, int w, int h);
	void DrawScene(CDeviceContext& devCon);
	void DestroyScene(CDeviceContext& devCon);

	void attachShaders(GLuint programId, ShaderInfo shaders[], int size);
	int InitializeShaders(CDeviceContext& devCon);

	GLuint renderProgramId = 0;
	GLint render_model_matrix_loc = 0;
	GLint render_projection_matrix_loc = 0;

protected:
//	void logGlErrorImpl(int line);
//	void attachShaders(GLuint programId, ShaderInfo shaders[], int size);

protected:

	int numOfVerticesSuzannu = 0;
	int numOfTexCoords = 0;
	int numOfNormalsSuzannu = 0;
	int numOfFaces = 0;
	int numOfIndicesPerFace = 0;
	int lengthOfFaces = 0;

	float aspect = 0;
//	GLint render_model_matrix_loc;
//	GLint render_projection_matrix_loc;

	// Buffer Object
	GLuint buffer = 0;

	enum VAO_ID { vao, num_vao };
	GLuint vaoid[num_vao];
	enum ATTRIB_ID { vPositionIndex = 0, vTexCoodIndex, vNormalIndex };

	struct ATTRIBUTE_USE_MATERIAL_LIST* pUseMaterialList = NULL;

	//	pUseMaterialTextureInfo = (struct UseMaterialTextureInfo*)malloc(sizeof(UseMaterialTextureInfo));
	//	memset(pUseMaterialTextureInfo, 0, sizeof(struct UseMaterialTextureInfo));

public:
	std::vector<UseMaterialTextureInfo*> useMaterialList;
	GLuint* pFacesVertexIndicies = 0;
	GLfloat* pVerticesSuzannu = 0;
	GLfloat** pTexCoordsPerUseMaterial = 0;
	GLfloat** pNormalSuzannuPerUseMaterial = 0;
protected:
	int numOfUseMaterials = 0;

public:
	/*
	void SetupShaderParameters(GLuint renderProgramId, GLint render_model_matrix_loc, GLint render_projection_matrix_loc);
	*/


};

