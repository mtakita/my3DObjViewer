#pragma once

#include "CActor.h"
#include "WavefrontObjFile.h"

class CConverter
{

public:
	static CActor* convertWavefrontObjFileToActor(const WavefrontObjFile& objFile);
	static CActor* convertWavefrontObjFileToActor2(const WavefrontObjFile& objFile);

	static Face& getLastFace(const UseMaterial& useMaterial);
	static int getNumOfIndicesPerFace(const UseMaterial& useMaterial);
	static size_t getLengthOfFacesVertexIndicies(const vector<UseMaterial>& useMaterials);
	static size_t getNumOfFaces2(const vector<UseMaterial>& useMaterials);
	static size_t getNumOfFace(const UseMaterial& useMaterial);

private:
	static struct ATTRIBUTE_USE_MATERIAL_LIST* getAttributeUseMaterialList(const WavefrontObjFile& objFile);

	static void getTexCoordsWithRearrange2(float* pTexCoords, UseMaterial& useMaterial, const vector<Vectorf2>& attributeVTs);
	static Vectorf2 getItemATTRIBUTE_VT_LIST(int index, const vector<Vectorf2>& attributeVTs);
	static void getNormalsWithRearrange2(float* pNormalsSuzannu, UseMaterial& useMaterial, const vector<Vectorf3>& attributeVNs );
	static void getVertexIndicies2PerFaces(unsigned int* pFacesSuzannu, vector<Face>* pFaces );

private:
	static size_t getNumOfAttributeUseMaterials(const WavefrontObjFile& objFile);

};

