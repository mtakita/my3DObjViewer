#pragma once

#ifndef _CACTOR_H_
#define _CACTOR_H_

#include <GL/gl3w.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include "CDeviceContext.h"
#include "etc.h"
#include "GLRenderer.h"
#include "CViewportProperty.h"
#include "IncreasingNumber.h"
#include "CActorAABB.h"
#include "CAABB.h"
#include "CGeometryBase.h"
#include "CSelectable.h"
#include "CActorPropertiesWnd.h"
#include "CTessellationBase.h"
#include "CFurBase.h"

using std::vector;
using std::list;
using std::map;
using std::string;

using TSequenceNumber = DWORD;

class CActorAABB;
class CAABB;
class CActorPropertiesWnd;

#define CACTOR_SHADER_SELECTED_FLAG     0x0001
#define CACTOR_SHADER_TESSELLATION_FLAG 0x0002

class CActor : public CGeometryBase, CSelectable, CTessellationBase, CFurBase
{
public:

	using CTessellationBase::GetTessellationOnOff;
	using CTessellationBase::GetTessellationDomain;
	using CTessellationBase::GetTessellationLevelFactorInner0;
	using CTessellationBase::GetTessellationLevelFactorInner1;
	using CTessellationBase::GetTessellationLevelFactorOuter0;
	using CTessellationBase::GetTessellationLevelFactorOuter1;
	using CTessellationBase::GetTessellationLevelFactorOuter2;
	using CTessellationBase::GetTessellationLevelFactorOuter3;
	using CFurBase::GetFurGeometryOnOff;
	using CFurBase::GetFurLayer;
	using CFurBase::GetFurDepth;

	using CTessellationBase::SetTessellationOnOff;
	using CTessellationBase::SetTessellationDomain;
	using CTessellationBase::SetTessellationLevelFactorInner0;
	using CTessellationBase::SetTessellationLevelFactorInner1;
	using CTessellationBase::SetTessellationLevelFactorOuter0;
	using CTessellationBase::SetTessellationLevelFactorOuter1;
	using CTessellationBase::SetTessellationLevelFactorOuter2;
	using CTessellationBase::SetTessellationLevelFactorOuter3;

	CActor();

	typedef struct structUseMaterialTextureInfo {
		GLuint texture;
		GLubyte* buffer;
		int bufferSize;

		int imageWidth;
		int imageHeight;

		GLuint bufferObject;

		int numOfFaces;
	}UseMaterialTextureInfo;

public:
	int m_ActorId;

	enum VAO_ID { vao, num_vao };
	GLuint vaoid[num_vao];

	// for non index.
	enum ATTRIB_ID2 { vPosition = 0, vTexCoord, vNormal };

	size_t numOfVertices;
	size_t numOfUseMaterials;
	GLfloat* pVertices;
	GLfloat** pTexCoordsPerUseMaterial;
	GLfloat** pNormalSuzannuPerUseMaterial;

	// Buffer Object
	GLuint buffer;
	float aspect = 0;

	map<string, GLuint*> pFacesVertexIndicies; // usematerialname, indices

	int lengthOfFaces = 0;
	int numOfIndicesPerFace = 0;

	map<string, UseMaterialTextureInfo*> useMaterialList;
	vector<string> useMaterialNameList;
	map<string, int/*index*/> useMaterialNameListIndex;

	int getNumOfFaces2(struct USE_MATERIAL* pUseMaterial);

	void prepareDrawing(CDeviceContext& devCon);

	void Reshape(CDeviceContext& devCon, int w, int h);


	int getActorId();
	void getAABB(float& xMin, float& yMin, float& zMin, float& xMax, float& yMax, float& zMax);
	CAABB*     createAABB(CActorAABB& actorAABB);
	CActorAABB* createActorAABB();
	void getActorProperty(CActorProperty& actorProperty);


private:

	static IncreasingNumber actorIdGenerator;



public:
	int numOfFaces;

	// 
	map<string, GLuint> bufferForIndecies;

	// Calculate buffer size for vertices(3 components.)
	int numFloatOfBufferSizeForVertices;
	int bytesOfBufferSizeForVertices;

	// Calculate buffer size for UVs(2 components.)
	int numFloatOfBufferSizeForUVs;
	int bytesOfBufferSizeForUVs;

	// Calculate buffer size for Normals(3 components.)
	int numFloatOfBufferSizeForNormals;
	int bytesOfBufferSizeForNormals;

	GLfloat* pBufferForVerties;
	GLfloat* pBufferForUVs;
	GLfloat* pBufferForNormals;

	// Unified indecies for each use material.
	map<string, int> numOfUnifiedIndecies;
	map<string, int> bytesOfUnifiedIndecies;
	map<string, int*> pBufferForUnifiedIndecies;

	void prepareDrawing2(CDeviceContext& devCon);
	void Render2(CDeviceContext& devCon, Matrixf4x4 projectionMatrix );
	void RenderBoundary(CDeviceContext& devCon, Matrixf4x4 projectionMatrix, Matrixf4x4 model_matrix );
	void RenderRegular(CDeviceContext& devCon, Matrixf4x4 projectionMatrix, Matrixf4x4 model_matrix);
	void Render2(CDeviceContext& devCon, Matrixf4x4 projectionMatrix, Matrixf4x4 translationMatrix);
	void checkActorParameters();

	virtual void SelectActor();
	virtual void DeselectActor();

	// Selected flag to represent the actor is currently selected.
	bool m_selectedFlag;

	void setActorPropertiesWnd( CActorPropertiesWnd* pActorPropertiesWnd );
	CActorPropertiesWnd* m_pActorPropertiesWnd;

	void setObjectName(string objectName) { m_objectName = objectName;  }
	string getObjectName() { return m_objectName;  }
	string m_objectName;

	//
	// Shaders
	//

	// Normal rendering.
	GLuint renderProgramId;
	GLuint renderProgramIdFurGeometry;
	GLuint renderProgramIdTessellation;

//	GLint render_model_matrix_loc;
//	GLint render_projection_matrix_loc;

	// Boundary rendering.
	GLuint renderProgramIdForBoundary;
	GLint render_model_matrix_loc_bdry;
	GLint render_projection_matrix_loc_bdry;

	void attachShaders(GLuint programId, ShaderInfo shaders[], int size);
	void compileShaders(GLuint programId, ShaderInfo2 shaders[], int size);
	void InitializeShaders(CDeviceContext& devCon);
	void InitializeAllShaders(CDeviceContext& devCon);
	int InitializeBoundaryShader(CDeviceContext& devCon);
	int InitializeRegularShader(CDeviceContext& devCon);
	int InitializeTessellationShader(CDeviceContext& devCon);
	boolean shaderNeedsUpdate(CActorProperty inActorProperty );
	void LinkRegularShaderProgram();
	void LinkGeometryShaderProgram();
	void LinkBoundaryShaderProgram();
	void LinkRegularTessellationShaderProgram();
	void LinkError(GLuint inProgramId);


	void replaceShaders(list<GLuint> inNewShaders, GLuint inProgramId );

	void setupBoundaryShaders( list<GLuint>* outShaders );
	void setupRegularShaders(list<GLuint>* outShaders);
	void setupTessellationShaders(list<GLuint>* outShaders);

	// shader ids
	GLuint m_regular_vertex_shader;
	GLuint m_regular_fragment_shader;
	GLuint m_regular_fragment_shader2;
	GLuint m_fur_fragment_shader;

	GLuint m_fur_vertex_shader;

	GLuint m_boundary_vertex_shader;
	GLuint m_boundary_fragment_shader;

	GLuint m_tessellation_vertex_shader;
	GLuint m_tessellation_fragment_shader;
	GLuint m_tessellation_control_shader;
	GLuint m_tessellation_evaluation_shader;
	GLuint m_fur_geometry_shader;

	//
	// fur texture for geometry shader.
	//
	GLuint fur_texture;
	GLuint fur_bufferObject;

	ShaderInfo2 shaders[12]{
		{ &m_regular_vertex_shader,          GL_VERTEX_SHADER,          "shaders/actor/actor.vert"      },
		{ &m_fur_vertex_shader,              GL_VERTEX_SHADER,          "shaders/actor/actor_fur.vert" },
		{ &m_regular_fragment_shader,        GL_FRAGMENT_SHADER,        "shaders/actor/actor.flag"      },
		{ &m_regular_fragment_shader2,       GL_FRAGMENT_SHADER,        "shaders/actor/actor.flag2"      },
		{ &m_fur_fragment_shader,            GL_FRAGMENT_SHADER,        "shaders/actor/actor_fur.flag"  },

		{ &m_fur_geometry_shader,            GL_GEOMETRY_SHADER,        "shaders/actor/actor_fur.geo"    },

		{ &m_boundary_vertex_shader,         GL_VERTEX_SHADER,          "shaders/actor/actor_bdry.vert" },
		{ &m_boundary_fragment_shader,       GL_FRAGMENT_SHADER,        "shaders/actor/actor_bdry.flag" },

		{ &m_tessellation_vertex_shader,     GL_VERTEX_SHADER,          "shaders/actor_tess/actor.vert"      },
		{ &m_tessellation_fragment_shader,   GL_FRAGMENT_SHADER,        "shaders/actor_tess/actor.flag"      },
		{ &m_tessellation_control_shader,    GL_TESS_CONTROL_SHADER,    "shaders/actor_tess/actor.tessctrl"  },
		{ &m_tessellation_evaluation_shader, GL_TESS_EVALUATION_SHADER, "shaders/actor_tess/actor.tesseval"  },


	};



	//
	// actor property handling.
	//
	void importActorProperty(CActorProperty inActorProperty);


	//
	// function and flags for identifying necessity of shader loading.
	//

	void prepareShadersBeforeRender(CActorProperty inActorProperty);
	boolean GetPrevSelectedFlag();
	boolean GetPrevTessellationOnOff();
	void SetPrevSelectedFlag(boolean inSelectedFlag);
	void SetPrevTessellationOnOff(boolean inTessellationFlag);
	boolean m_prevSelectedFlag;
	boolean m_prevTessellationOnOff;

};

#endif
