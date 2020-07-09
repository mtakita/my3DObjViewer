#pragma once


#ifndef _OBJ_READER_FUNC_
#define _OBJ_READER_FUNC_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "obj_reader.h"

	extern struct OBJECT_ATTRIBUTES* pCurrentObj;
	extern struct MATERIAL* pCurrentMtl;
	extern struct USE_MATERIAL* pCurrentUseMaterial;

	extern char* yytext;

	extern void yylog(char* msg);

	extern void yylog(char* msg);


	extern void yyerror(char* s, ...);

	static unsigned symhash(const char* sym);

	extern struct OBJECT_ATTRIBUTES* lookupObject(const char* sym);

	extern struct MATERIAL* createNewmtl(const char* pNewmtlName);

	extern struct ATTRIBUTE_NS* createAttributeNs(float ns);

	extern struct ATTRIBUTE_KA* createAttributeKa(float x, float y, float z);

	extern struct ATTRIBUTE_KD* createAttributeKd(float x, float y, float z);

	extern struct ATTRIBUTE_KS* createAttributeKs(float x, float y, float z);

	extern struct ATTRIBUTE_KE* createAttributeKe(float x, float y, float z);

	extern struct ATTRIBUTE_NI* createAttributeNi(float ni);

	extern struct ATTRIBUTE_D* createAttributeD(float d);

	extern struct ATTRIBUTE_ILLUM* createAttributeIllum(float illum);

	extern const char* createAttributeMapKd(const char* pMapKd);

	extern struct MATERIAL_LIBRARY* lookupMaterialLibrary2(const char* pMaterialLibraryName);

	extern struct MATERIAL_LIBRARY* createMaterialLibrary(const char* pMaterialLibraryName);

	extern struct MATERIAL* lookupMaterial(const char* pMaterialName);

	extern struct VERTNUM* createVertNum(float x, float y, float z);

	extern struct VERT2NUM* createVert2Num(float u, float v);

	extern struct FACENUM* createFaceNum(enum FaceNumType type, int vertnum, int texnum, int normnum);

	extern struct ATTRIBUTE_VN_LIST* getLastAttributeVN(struct OBJECT_ATTRIBUTES* pObject);

	extern struct ATTRIBUTE_VT_LIST* getLastAttributeVT(struct OBJECT_ATTRIBUTES* pObject);

	extern struct ATTRIBUTE_V_LIST* getLastAttributeV(struct OBJECT_ATTRIBUTES* pObject);

	extern struct ATTRIBUTE_USE_MATERIAL_LIST* getLastAttributeUseMaterialList(struct OBJECT_ATTRIBUTES* pObject);


	extern struct ATTRIBUTE_USE_MATERIAL_LIST* getAttributeUseMaterialList(struct OBJECT_ATTRIBUTES* pObject);

	extern int getNumOfAttributeUseMaterials();

	extern struct ATTRIBUTE_V* createAttributeV(struct VERTNUM* pVertNum);

	extern struct MATERIAL* findMaterial(const char* pUseMaterialName);

	extern struct ATTRIBUTE_USE_MATERIAL_LIST* createUseMaterial(char* pUseMaterialName);

	extern struct ATTRIBUTE_VN* createAttributeVN(struct VERTNUM* pVertNum);

	extern struct ATTRIBUTE_VT* createAttributeVT(struct VERT2NUM* pVert2Num);

	extern struct ATTRIBUTE_F_LIST* getLastAttributeF(struct OBJECT_ATTRIBUTES* pObject);

	extern struct ATTRIBUTE_F* createAttributeF(struct FACENUM* pFaceNum);

	extern struct OBJECT_ATTRIBUTES* createObjectAttributes(char* name);

	extern int getNumOfFaces();

	extern int getNumOfFaces2(struct USE_MATERIAL* pUseMaterial);

	extern int getNumOfIndicesPerFace();

	extern void xgetFaces(unsigned int* pFacesSuzannu);

	extern int xgetLengthOfFaces();

	extern void getVertexIndiciesPerFaces(unsigned int* pFacesSuzannu);

	extern void getVertexIndicies2PerFaces(unsigned int* pFacesSuzannu, struct ATTRIBUTE_F_LIST* pFL);

	extern int getLengthOfFacesVertexIndicies();

	extern void getVertices(float* pVerticesSuzannu);

	extern void getTexCoords(float* pTexCoords);

	extern int getNumOfVertices();

	extern int getNumOfTexCoords();

	extern void getNormals(float* pNormalsSuzannu);

	extern struct ATTRIBUTE_VN* getItemATTRIBUTE_VN_LIST(int index);

	extern struct ATTRIBUTE_VT* getItemATTRIBUTE_VT_LIST(int index);

	extern void getNormalsWithRearrange2(float* pNormalsSuzannu, struct USE_MATERIAL* pUseMaterial);

	extern void getNormalsWithRearrange(float* pNormalsSuzannu);

	extern void getTexCoordsWithRearrange(float* pTexCoords);

	extern void getTexCoordsWithRearrange2(float* pTexCoords, struct USE_MATERIAL* pUseMaterial);

	extern int getNumOfNormals();

	extern void dump_one_object(struct OBJECT_ATTRIBUTES* object);

	extern void dump_all_objects();

	extern struct MATERIAL* createMaterial(const char* pMaterialName);


#ifdef __cplusplus
}
#endif

#endif

