#include "pch.h"
#include "CConverter.h"

#include <GL/gl3w.h>

#include "mylibjpeg.h"
#include "logGLError.h"

#define BUFFER_OFFSET(pointer) ((void*)(pointer))

size_t CConverter::getNumOfAttributeUseMaterials(const WavefrontObjFile& objFile) {

	size_t numOfAttributeUseMaterials = objFile.getNumOfUseMaterials();
	return numOfAttributeUseMaterials;
}

/*
Obtain all texture coordinates to specified storage after rearrangement along with order of vertices.
*/
void CConverter::getTexCoordsWithRearrange2(float* pTexCoords, UseMaterial& useMaterial, const vector<Vectorf2>& attributeVTs ) {

	struct VT* pTexCoordsVT = (struct VT*)pTexCoords;

	for (Face face : *useMaterial.pFaces) {

		for (OneSetOfIndecies* pOnesetofIndex : face.pOneSetOfIndecies) {

			int destIndex = pOnesetofIndex->indecies.getX() - 1;
			int srcIndex = pOnesetofIndex->indecies.getY() - 1;

			//			Vectorf2& vec = getItemATTRIBUTE_VT_LIST(srcIndex, attributeVTs);
			Vectorf2 vec = attributeVTs[srcIndex];
			pTexCoordsVT[destIndex].u = vec[0];
			pTexCoordsVT[destIndex].v = 1.0f - vec[1];
		}
	}
}


/*
Obtain all normals to specified storage after rearrangement to along with order of vertices.
*/
void CConverter::getNormalsWithRearrange2(float* pNormalsSuzannu, UseMaterial& useMaterial, const vector<Vectorf3>& attributeVNs) {

	struct VN* pNormalsSuzannuVN = (struct VN*)pNormalsSuzannu;

	for (Face face : *useMaterial.pFaces) {

		for (OneSetOfIndecies* pOneSetOfIndecies : face.pOneSetOfIndecies ){

			int destIndex = pOneSetOfIndecies->indecies.getX() - 1;
			int srcIndex = pOneSetOfIndecies->indecies.getZ() - 1;

			//			Vectorf3 vec = getItemATTRIBUTE_VN_LIST(srcIndex, attributeVNs);
			Vectorf3 vec = attributeVNs[srcIndex];
			pNormalsSuzannuVN[destIndex].x = vec.getX();
			pNormalsSuzannuVN[destIndex].y = vec.getY();
			pNormalsSuzannuVN[destIndex].z = vec.getZ();
		}

	}
}

Face& CConverter::getLastFace( const UseMaterial& useMaterial) {

	size_t index = useMaterial.pFaces->size() - 1;
	return (*useMaterial.pFaces)[index];
}



/*
Obtain a number of vertices per face.
*/
int CConverter::getNumOfIndicesPerFace( const UseMaterial& useMaterial ){

	int numOfIndicesPerFace = 0;

	Face& face = getLastFace(useMaterial);
	return face.pOneSetOfIndecies.size();
}

/*
Obtain a number of faces holding.
*/
size_t CConverter::getNumOfFaces2(const vector<UseMaterial>& useMaterials) {

	int size = 0;
	for (const UseMaterial useMaterial : useMaterials) {
		size += useMaterial.pFaces->size();
	}

	return size;
}

/*
Obtain a number of faces holding.
*/
size_t CConverter::getNumOfFace(const UseMaterial& useMaterial) {

	return useMaterial.pFaces->size();
}

/*
Return the length in bytes needed to store vertex indicies value.
*/
size_t CConverter::getLengthOfFacesVertexIndicies( const vector<UseMaterial>& useMaterials ) {

	// Obtain the number of faces.
	// number of faces * number of vertices per face * number of sizeof(int)
	size_t length = getNumOfFaces2(useMaterials) * getNumOfIndicesPerFace(useMaterials[0]) * sizeof(int) * 3;

	return length;
}

/*
Obtain only vertex indicies to specified storage.
*/
void CConverter::getVertexIndicies2PerFaces(unsigned int* pFacesSuzannu, vector<Face>* pFaces ) {

	for (Face face : *pFaces) {
		for (OneSetOfIndecies* pOneSetOfIndex : face.pOneSetOfIndecies) {
			switch (pOneSetOfIndex->type) {

			case enumFaceNumType1:
				*pFacesSuzannu++ = pOneSetOfIndex->indecies.getX() - 1;
				break;
			case enumFaceNumType2:
				*pFacesSuzannu++ = pOneSetOfIndex->indecies.getX() - 1;
				break;
			case enumFaceNumType3:
				*pFacesSuzannu++ = pOneSetOfIndex->indecies.getX() - 1;
				break;
			case enumFaceNumType4:
				*pFacesSuzannu++ = pOneSetOfIndex->indecies.getX() - 1;
				break;
			default:
				abort();
			}
		}
	}
}


CActor* CConverter::convertWavefrontObjFileToActor(const WavefrontObjFile& objFile) {

	CActor* pActor = new CActor();

	pActor->numOfUseMaterials = getNumOfAttributeUseMaterials( objFile );

	/**/
	// Retriving vertices.
	/**/
	pActor->numOfVertices = objFile.getNumOfVertices();
	GLfloat* pVerticesSuzannu = (GLfloat*)malloc(sizeof(GLfloat) * 3 * pActor->numOfVertices);
	pActor->pVertices = pVerticesSuzannu;

	const vector<Vectorf3>& attributeVs = objFile.getAttributeVs();
	for (Vectorf3 vec : attributeVs) {
		*pVerticesSuzannu++ = vec.getX();
		*pVerticesSuzannu++ = vec.getY();
		*pVerticesSuzannu++ = vec.getZ();
	}

	const vector<UseMaterial>& useMaterials = objFile.getUseMaterials();

	/**/
	// Retriving texture coordinates for each use material.
	/**/
	GLfloat** pTexCoordsPerUseMaterial = (GLfloat**)malloc(sizeof(GLfloat*) * pActor->numOfUseMaterials);
	pActor->pTexCoordsPerUseMaterial = pTexCoordsPerUseMaterial;

	int numOfTexCoords = 0;
	int index = 0;
	for (UseMaterial useMaterial : useMaterials) {

		numOfTexCoords = objFile.getNumOfTexCoords();
		pTexCoordsPerUseMaterial[index] = (GLfloat*)malloc(sizeof(GLfloat) * 2 * pActor->numOfVertices);
		memset(pTexCoordsPerUseMaterial[index], 0, sizeof(GLfloat) * 2 * pActor->numOfVertices);
		getTexCoordsWithRearrange2(pTexCoordsPerUseMaterial[index], useMaterial, objFile.getAttributeVTs() );

		// Create map between usematerialname and index of it.
		pActor->useMaterialNameListIndex[useMaterial.useMaterialName] = index;

		index++;
	}

	/**/
	// Retriving normals for each use material.
	/**/

	GLfloat** pNormalSuzannuPerUseMaterial = (GLfloat**)malloc(sizeof(GLfloat*) * pActor->numOfUseMaterials);
	pActor->pNormalSuzannuPerUseMaterial = pNormalSuzannuPerUseMaterial;

	for (UseMaterial useMaterial : useMaterials) {

		int idx = pActor->useMaterialNameListIndex[useMaterial.useMaterialName];

		pNormalSuzannuPerUseMaterial[idx] = (GLfloat*)malloc(sizeof(GLfloat) * 3 * pActor->numOfVertices);

		getNormalsWithRearrange2(pNormalSuzannuPerUseMaterial[idx], useMaterial, objFile.getAttributeVNs() );

	}

	//	numOfFaces = getNumOfFaces();
	pActor->numOfIndicesPerFace = getNumOfIndicesPerFace(useMaterials[0]);
	pActor->lengthOfFaces = getLengthOfFacesVertexIndicies(useMaterials);

	//	getFacesVertexIndicies(pFacesVertexIndicies);

	for (UseMaterial useMaterial : useMaterials) {

		try {

			CActor::UseMaterialTextureInfo* pUseMaterialTextureInfo = new CActor::UseMaterialTextureInfo();

			pActor->useMaterialNameList.push_back(useMaterial.useMaterialName);

			pActor->useMaterialList[useMaterial.useMaterialName] = pUseMaterialTextureInfo;

			int numOfFaces = getNumOfFace(useMaterial);
			pUseMaterialTextureInfo->numOfFaces = numOfFaces;

			using namespace jpeglib;

			struct jpeg_decompress_struct cinfo;
			struct my_error_mgr jerr;
			FILE* infile = 0;		/* source file */

			WavefrontObjFile::Material* pMaterial = objFile.getMaterial(useMaterial.useMaterialName);
			if (pMaterial == nullptr) {
				// No material.
				continue;
			}

			JSAMPARRAY buffer;		/* Output row buffer */
			int row_stride;		/* physical row width in output buffer */
			errno_t err = fopen_s(&infile, ( pMaterial->dirname + pMaterial->mapKd).c_str(), "r+b");
			if (err != 0) {
				fprintf(stderr, "can't open %s\n", pMaterial->mapKd.c_str());
				throw "can't open material file!";
			}

			cinfo.err = jpeg_std_error(&jerr.pub);
			jerr.pub.error_exit = my_error_exit;
			if (setjmp(jerr.setjmp_buffer)) {
				jpeg_destroy_decompress(&cinfo);
				fclose(infile);
				throw "can't read material file!";
			}

			using namespace jpeglib;
			jpeg_create_decompress(&cinfo);

			jpeglib::jpeg_stdio_src(&cinfo, infile);

			(void)jpeg_read_header(&cinfo, TRUE);
			(void)jpeg_start_decompress(&cinfo);
			row_stride = cinfo.output_width * cinfo.output_components;
			buffer = (*cinfo.mem->alloc_sarray)
				((jpeglib::j_common_ptr) & cinfo, JPOOL_IMAGE, row_stride, 1);

			int bufferSize = cinfo.output_width * cinfo.output_height * cinfo.output_components;
			(*pUseMaterialTextureInfo).buffer = (GLubyte*)malloc(bufferSize);
			(*pUseMaterialTextureInfo).bufferSize = bufferSize;
			(*pUseMaterialTextureInfo).imageWidth = cinfo.output_width;
			(*pUseMaterialTextureInfo).imageHeight = cinfo.output_height;
			int current_row = 0;

			while (cinfo.output_scanline < cinfo.output_height) {
				(void)jpeg_read_scanlines(&cinfo, buffer, 1);

				memcpy((*pUseMaterialTextureInfo).buffer + current_row * row_stride, (*buffer), row_stride);
				current_row++;
			}

			(void)jpeg_finish_decompress(&cinfo);
			jpeg_destroy_decompress(&cinfo);
			fclose(infile);


			// Prepare indices for drawElement
			GLuint*  pFacesVertexIndicies = (GLuint*)malloc(pActor->lengthOfFaces);
			pActor->pFacesVertexIndicies[useMaterial.useMaterialName] = pFacesVertexIndicies;

			vector<Face>* pFaces = useMaterial.pFaces;
			getVertexIndicies2PerFaces(pFacesVertexIndicies, pFaces);

		}
		catch (...) {

			std::cout << "An exception occured!" << std::endl;
		}
	}
	return pActor;

}

CActor* CConverter::convertWavefrontObjFileToActor2(const WavefrontObjFile& objFile) {

	CActor* pActor = new CActor();

	// まだ複数オブジェクト読み込みに対応してない。
	pActor->setObjectName(objFile.getObjectNames());


	pActor->numOfUseMaterials = getNumOfAttributeUseMaterials(objFile);
	const vector<UseMaterial>& useMaterials = objFile.getUseMaterials();

	pActor->numOfFaces = 0;
	for (UseMaterial useMaterial : useMaterials) {
		pActor->numOfFaces += (*useMaterial.pFaces).size();
	}

	//
	// Prepare buffer for vertices and UVs and Normals.
	//

	// Calculate buffer size for vertices(3 components.)
	pActor->numFloatOfBufferSizeForVertices = pActor->numOfFaces * 3 * 3;
	pActor->bytesOfBufferSizeForVertices = pActor->numOfFaces * 3 * 3 * sizeof(GLfloat);

	// Calculate buffer size for UVs(2 components.)
	pActor->numFloatOfBufferSizeForUVs = pActor->numOfFaces * 3 * 2;
	pActor->bytesOfBufferSizeForUVs = pActor->numOfFaces * 3 * 2 * sizeof(GLfloat);

	// Calculate buffer size for Normals(3 components.)
	pActor->numFloatOfBufferSizeForNormals = pActor->numOfFaces * 3 * 3;
	pActor->bytesOfBufferSizeForNormals = pActor->numOfFaces * 3 * 3 * sizeof(GLfloat);

	GLfloat* pBufferForVertices = new GLfloat[pActor->numFloatOfBufferSizeForVertices];
	GLfloat* pBufferForUVs = new GLfloat[pActor->numFloatOfBufferSizeForUVs];
	GLfloat* pBufferForNormals = new GLfloat[pActor->numFloatOfBufferSizeForNormals];

	pActor->pBufferForVerties = pBufferForVertices;
	pActor->pBufferForUVs = pBufferForUVs;
	pActor->pBufferForNormals = pBufferForNormals;

	//
	// Construct buffer of vertices and UVs and Normals for OpenGL rendering.
	//

	const vector<Vectorf3>& attributeVs = objFile.getAttributeVs();
	const vector<Vectorf2>& attributeVTs = objFile.getAttributeVTs();
	const vector<Vectorf3>& attributeVNs = objFile.getAttributeVNs();

	int unifiedIndex = 0;

	for (UseMaterial useMaterial : useMaterials) {

		int* pBufferForUnifiedIndex = new int[ useMaterial.pFaces->size() * 3 ];
		pActor->pBufferForUnifiedIndecies[useMaterial.useMaterialName] = pBufferForUnifiedIndex;

		int numOfUnifiedIndexForUseMaterial = 0;

		for (Face face : *useMaterial.pFaces) {

			for (OneSetOfIndecies* pOnesetofIndex : face.pOneSetOfIndecies) {

				int vertexIndex = pOnesetofIndex->indecies.getX() - 1;
				int uvIndex = pOnesetofIndex->indecies.getY() - 1;
				int normalIndex = pOnesetofIndex->indecies.getZ() - 1;

				Vectorf3 vertex = attributeVs[vertexIndex];
				Vectorf2 uv = attributeVTs[uvIndex];
				Vectorf3 normal = attributeVNs[normalIndex];

				*pBufferForVertices++ = vertex[0];
				*pBufferForVertices++ = vertex[1];
				*pBufferForVertices++ = vertex[2];

				*pBufferForUVs++ = uv[0];
				*pBufferForUVs++ = 1.0f - uv[1];		// テクスチャーが反転しているのか、vが反転しているのか?

				*pBufferForNormals++ = normal[0];
				*pBufferForNormals++ = normal[1];
				*pBufferForNormals++ = normal[2];

				pBufferForUnifiedIndex[numOfUnifiedIndexForUseMaterial++] = unifiedIndex++;
			}

		}

		pActor->numOfUnifiedIndecies[useMaterial.useMaterialName] = numOfUnifiedIndexForUseMaterial;
		pActor->bytesOfUnifiedIndecies[useMaterial.useMaterialName] = numOfUnifiedIndexForUseMaterial * sizeof(int);

	}

	//
	// Read texture images.
	//

	for (UseMaterial useMaterial : useMaterials) {

		try {

			CActor::UseMaterialTextureInfo* pUseMaterialTextureInfo = new CActor::UseMaterialTextureInfo();

			pActor->useMaterialNameList.push_back(useMaterial.useMaterialName);

			pActor->useMaterialList[useMaterial.useMaterialName] = pUseMaterialTextureInfo;

			int numOfFaces = getNumOfFace(useMaterial);
			pUseMaterialTextureInfo->numOfFaces = numOfFaces;

			using namespace jpeglib;

			struct jpeg_decompress_struct cinfo;
			struct my_error_mgr jerr;
			FILE* infile = 0;		/* source file */

			WavefrontObjFile::Material* pMaterial = objFile.getMaterial(useMaterial.useMaterialName);
			if (pMaterial == nullptr) {
				// No material.
				continue;
			}

			JSAMPARRAY buffer;		/* Output row buffer */
			int row_stride;		/* physical row width in output buffer */
			string slash{ '\\' };
			errno_t err = fopen_s(&infile, (pMaterial->dirname+slash+pMaterial->mapKd).c_str(), "r+b");
			if (err != 0) {
				fprintf(stderr, "can't open %s\n", pMaterial->mapKd.c_str());
				throw "can't open material file!";
			}

			cinfo.err = jpeg_std_error(&jerr.pub);
			jerr.pub.error_exit = my_error_exit;
			if (setjmp(jerr.setjmp_buffer)) {
				jpeg_destroy_decompress(&cinfo);
				fclose(infile);
				throw "can't read material file!";
			}

			using namespace jpeglib;
			jpeg_create_decompress(&cinfo);

			jpeglib::jpeg_stdio_src(&cinfo, infile);

			(void)jpeg_read_header(&cinfo, TRUE);
			(void)jpeg_start_decompress(&cinfo);
			row_stride = cinfo.output_width * cinfo.output_components;
			buffer = (*cinfo.mem->alloc_sarray)
				((jpeglib::j_common_ptr) & cinfo, JPOOL_IMAGE, row_stride, 1);

			int bufferSize = cinfo.output_width * cinfo.output_height * cinfo.output_components;
			(*pUseMaterialTextureInfo).buffer = (GLubyte*)malloc(bufferSize);
			(*pUseMaterialTextureInfo).bufferSize = bufferSize;
			(*pUseMaterialTextureInfo).imageWidth = cinfo.output_width;
			(*pUseMaterialTextureInfo).imageHeight = cinfo.output_height;
			int current_row = 0;

			while (cinfo.output_scanline < cinfo.output_height) {
				(void)jpeg_read_scanlines(&cinfo, buffer, 1);

				memcpy((*pUseMaterialTextureInfo).buffer + current_row * row_stride, (*buffer), row_stride);
				current_row++;
			}

			(void)jpeg_finish_decompress(&cinfo);
			jpeg_destroy_decompress(&cinfo);
			fclose(infile);

		}
		catch (...) {

			std::cout << "An exception occured!" << std::endl;
		}
	}

	return pActor;

}
