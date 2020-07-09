#include "pch.h"
#include "WavefrontObjFile.h"
//#include "WavefrontObject.h"
#include "mymath.h"
#include <memory>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace mymath;
using namespace std;

size_t WavefrontObjFile::getNumOfAttributeUseMaterials() const{

	const WavefrontObjFile::UseMaterials* pUseMaterial = objectAttributes->getUseMaterials();
	return pUseMaterial->getNumOfUseMaterials();


}


WavefrontObjFile::AttributeVTs* WavefrontObjFile::AttributeVTs::create(struct ATTRIBUTE_VT_LIST* pAttributeVTList) {
	WavefrontObjFile::AttributeVTs* pAttributeVT = new WavefrontObjFile::AttributeVTs();
	pAttributeVT->createImpl(pAttributeVTList);
	return pAttributeVT;
}

void WavefrontObjFile::AttributeVTs::createImpl(struct ATTRIBUTE_VT_LIST* attributeVTList) {
	
	struct ATTRIBUTE_VT_LIST* pAttributeVTList = attributeVTList;

	do {
		struct ATTRIBUTE_VT* pAttributeVT = pAttributeVTList->pAttributeVT;
		if (pAttributeVT != NULL) {

			unique_ptr<Vectorf2> pVector2{ new Vectorf2(pAttributeVT->pVert2Num->u,
												pAttributeVT->pVert2Num->v) };
			attributeVTs.push_back(*pVector2);
		}

		pAttributeVTList = pAttributeVTList->pNext;

	} while (pAttributeVTList != NULL);
}

WavefrontObjFile::AttributeVNs* WavefrontObjFile::AttributeVNs::create(struct ATTRIBUTE_VN_LIST* pAttributeVNList) {
	WavefrontObjFile::AttributeVNs* pAttributeVN = new WavefrontObjFile::AttributeVNs();
	pAttributeVN->createImpl(pAttributeVNList);
	return pAttributeVN;
}

void WavefrontObjFile::AttributeVNs::createImpl(struct ATTRIBUTE_VN_LIST* attributeVNList) {

	struct ATTRIBUTE_VN_LIST* pAttributeVNList = attributeVNList;

	do {
		struct ATTRIBUTE_VN* pAttributeVN = pAttributeVNList->pAttributeVN;
		if (pAttributeVN != NULL) {

			unique_ptr<Vectorf3> pVector3{ new Vectorf3(pAttributeVN->pVertNum->x,
												pAttributeVN->pVertNum->y,
												pAttributeVN->pVertNum->z ) };
			attributeVNs.push_back(*pVector3);
		}

		pAttributeVNList = pAttributeVNList->pNext;

	} while (pAttributeVNList != NULL);
}


WavefrontObjFile::UseMaterials* WavefrontObjFile::UseMaterials::create(struct ATTRIBUTE_USE_MATERIAL_LIST* pAttributeUseMaterialList) {
	WavefrontObjFile::UseMaterials* pUseMaterial = new WavefrontObjFile::UseMaterials();
	pUseMaterial->createImpl(pAttributeUseMaterialList);
	return pUseMaterial;
}

void WavefrontObjFile::UseMaterials::createImpl(struct ATTRIBUTE_USE_MATERIAL_LIST* attributeUseMaterialList) {

	struct ATTRIBUTE_USE_MATERIAL_LIST* pAttributeUseMaterialList = attributeUseMaterialList;

	do {
		struct USE_MATERIAL* pUseMaterial = pAttributeUseMaterialList->pUseMaterial;
		if (pUseMaterial != NULL) {

			vector<Face>* pFaces = createAttributeF(pUseMaterial->pAttributeFList);

			UseMaterial* pUseMaterialInner = new UseMaterial();
			pUseMaterialInner->useMaterialName = pUseMaterial->pUseMaterialName;
			pUseMaterialInner->pFaces = pFaces;

			useMaterials.push_back(*pUseMaterialInner);

/* ‚±‚±‚Å“Ç‚ñ‚Å‚Íƒ_ƒ@struct obj_reader_work‚ÌMATERIAL‚ð“Ç‚Þ‚Ì‚ª³‚µ‚¢B
			ns = pUseMaterial->pMaterial->pAttributeNs->ns;

			ka = vectorf3(pUseMaterial->pMaterial->pAttributeKa->x,
				pUseMaterial->pMaterial->pAttributeKa->y,
				pUseMaterial->pMaterial->pAttributeKa->z);

			kd = vectorf3(pUseMaterial->pMaterial->pAttributeKd->x,
				pUseMaterial->pMaterial->pAttributeKd->y,
				pUseMaterial->pMaterial->pAttributeKd->z);

			ks = vectorf3(pUseMaterial->pMaterial->pAttributeKs->x,
				pUseMaterial->pMaterial->pAttributeKs->y,
				pUseMaterial->pMaterial->pAttributeKs->z);

			ke = vectorf3(pUseMaterial->pMaterial->pAttributeKe->x,
				pUseMaterial->pMaterial->pAttributeKe->y,
				pUseMaterial->pMaterial->pAttributeKe->z);

			ni = pUseMaterial->pMaterial->pAttributeNi->ni;

			d = pUseMaterial->pMaterial->pAttributeD->d;

			illum = pUseMaterial->pMaterial->pAttributeIllum->illum;

			mapKd = pUseMaterial->pMaterial->pMapKd;
			*/
		}

		pAttributeUseMaterialList = pAttributeUseMaterialList->pNext;

	} while (pAttributeUseMaterialList != NULL);
}

vector<Face>* WavefrontObjFile::UseMaterials::createAttributeF(struct ATTRIBUTE_F_LIST* attributeFList) {

	struct ATTRIBUTE_F_LIST* pAttributeFList = attributeFList;
	vector<Face>* pFaces = new vector<Face>();

	do {
		struct ATTRIBUTE_F* pAttributeF = pAttributeFList->pAttributeF;
		if (pAttributeF->pFaceNum != nullptr) {

			struct FACENUM* pFaceNum = pAttributeF->pFaceNum;
			int numOfIndecies = 0;

			// Counting number of indecies
			do{
				numOfIndecies++;
				pFaceNum = pFaceNum->pNext;
			} while (pFaceNum != nullptr);

			Face* pFace = new Face();

			// Retrieve indecies.
			pFaceNum = pAttributeF->pFaceNum;

			do {
				OneSetOfIndecies* pOneSetOfIndecies = new OneSetOfIndecies();
				pFace->pOneSetOfIndecies.push_back(pOneSetOfIndecies);

				pOneSetOfIndecies->type = pFaceNum->type;
				pOneSetOfIndecies->indecies.setX(pFaceNum->vertnum);
				pOneSetOfIndecies->indecies.setY(pFaceNum->texnum);
				pOneSetOfIndecies->indecies.setZ(pFaceNum->normnum);

				pFaceNum = pFaceNum->pNext;
			} while (pFaceNum != nullptr);

			pFaces->push_back(*pFace);
		}

		pAttributeFList = pAttributeFList->pNext;

	} while (pAttributeFList != NULL);

	return pFaces;
}


WavefrontObjFile::AttributeVs* WavefrontObjFile::AttributeVs::create(struct ATTRIBUTE_V_LIST* attributeVList) {
	WavefrontObjFile::AttributeVs* pAttributeV = new WavefrontObjFile::AttributeVs();
	pAttributeV->createImpl(attributeVList);
	return pAttributeV;
}

void WavefrontObjFile::AttributeVs::createImpl(struct ATTRIBUTE_V_LIST* attributeVList) {

	struct ATTRIBUTE_V_LIST* pAttributeVList = attributeVList;

	do {
		struct ATTRIBUTE_V* pAttributeV = pAttributeVList->pAttributeV;
		if (pAttributeV != NULL) {

			unique_ptr<Vectorf3> pVector3{ new Vectorf3(pAttributeV->pVertNum->x,
												pAttributeV->pVertNum->y,
												pAttributeV->pVertNum->z) };
			attributeVs.push_back( *pVector3);
		}

		pAttributeVList = pAttributeVList->pNext;

	} while (pAttributeVList != NULL);
}

WavefrontObjFile::ObjectAttributes* WavefrontObjFile::ObjectAttributes::create(struct OBJECT_ATTRIBUTES* objectAttributes) {

	WavefrontObjFile::ObjectAttributes* pObjectAttributes = new WavefrontObjFile::ObjectAttributes();
	pObjectAttributes->createImpl(objectAttributes);

	return pObjectAttributes;
}

void WavefrontObjFile::ObjectAttributes::createImpl(struct OBJECT_ATTRIBUTES* objectAttributes) {

	objectName = string(objectAttributes->pObjectName);

	AttributeVs* pAttributeV = AttributeVs::create(objectAttributes->pAttributeVList);
	attributeV = pAttributeV;

	AttributeVTs* pAttributeVT = AttributeVTs::create(objectAttributes->pAttributeVTList);
	attributeVT = pAttributeVT;

	AttributeVNs* pAttributeVN = AttributeVNs::create(objectAttributes->pAttributeVNList);
	attributeVN = pAttributeVN;

	UseMaterials* pUseMaterial = UseMaterials::create(objectAttributes->pAttributeUseMaterialList);
	useMaterial = pUseMaterial;
}

/* ‚±‚ê‚Í”pŽ~
WavefrontObjFile::Material* WavefrontObjFile::Material::create(struct MATERIAL* material) {
	WavefrontObjFile::Material* pMaterial = new WavefrontObjFile::Material();
	pMaterial->createImpl(material);
	return pMaterial;
}
*/
/*
void WavefrontObjFile::Material::createImpl(struct MATERIAL* material) {

	materialName = material->pMaterialName;

	ns = material->pAttributeNs->ns;

	ka = Vectorf3(material->pAttributeKa->x,
		material->pAttributeKa->y,
		material->pAttributeKa->z);

	kd = Vectorf3(material->pAttributeKd->x,
		material->pAttributeKd->y,
		material->pAttributeKd->z);

	ks = Vectorf3(material->pAttributeKs->x,
		material->pAttributeKs->y,
		material->pAttributeKs->z);

	ke = Vectorf3(material->pAttributeKe->x,
		material->pAttributeKe->y,
		material->pAttributeKe->z);

	ni = material->pAttributeNi->ni;

	d = material->pAttributeD->d;

	illum = material->pAttributeIllum->illum;

	mapKd = material->pMapKd;

	dirname = material->pDirname;

}
*/

WavefrontObjFile* WavefrontObjFile::create(struct obj_reader_work* objReaderWork) {

	WavefrontObjFile* objFile = new WavefrontObjFile();
	objFile->createImpl(objReaderWork);
	return objFile;
}

vector<WavefrontObjFile::Material*> WavefrontObjFile::Material::create(const struct MATERIAL mtltab[]) {

	vector<WavefrontObjFile::Material*> materials;

	for (int i = 0; i < NHASH; i++) {
		if (mtltab[i].pMaterialName) {

			WavefrontObjFile::Material* pMaterial = new WavefrontObjFile::Material();
			materials.push_back(pMaterial);

			pMaterial->materialName = mtltab[i].pMaterialName;

			pMaterial->ns = mtltab[i].pAttributeNs != nullptr ? mtltab[i].pAttributeNs->ns : 0.0f;

			pMaterial->ka = mtltab[i].pAttributeKa != nullptr ? Vectorf3(mtltab[i].pAttributeKa->x, mtltab[i].pAttributeKa->y, mtltab[i].pAttributeKa->z) : Vectorf3{ 0.0f, 0.0f, 0.0f };
			pMaterial->kd = mtltab[i].pAttributeKd != nullptr ? Vectorf3(mtltab[i].pAttributeKd->x, mtltab[i].pAttributeKd->y, mtltab[i].pAttributeKd->z) : Vectorf3{ 0.0f, 0.0f, 0.0f };
			pMaterial->ks = mtltab[i].pAttributeKs != nullptr ? Vectorf3(mtltab[i].pAttributeKs->x, mtltab[i].pAttributeKs->y, mtltab[i].pAttributeKs->z) : Vectorf3{ 0.0f, 0.0f, 0.0f };
			pMaterial->ke = mtltab[i].pAttributeKe != nullptr ? Vectorf3(mtltab[i].pAttributeKe->x, mtltab[i].pAttributeKe->y, mtltab[i].pAttributeKe->z) : Vectorf3{ 0.0f, 0.0f, 0.0f };

			pMaterial->ni = mtltab[i].pAttributeNi != nullptr ? mtltab[i].pAttributeNi->ni : 0.0f;
			pMaterial->d = mtltab[i].pAttributeD != nullptr ? mtltab[i].pAttributeD->d : 0.0f;
			pMaterial->illum = mtltab[i].pAttributeIllum != nullptr ? mtltab[i].pAttributeIllum->illum : 0.0f;
			pMaterial->mapKd = mtltab[i].pMapKd != nullptr ? mtltab[i].pMapKd : "";
			pMaterial->dirname = mtltab[i].pDirname != nullptr ? mtltab[i].pDirname : "";

		}
	}

	return materials;
}


void WavefrontObjFile::createImpl(struct obj_reader_work* objReaderWork) {

	WavefrontObjFile::ObjectAttributes* pObjectAttributes = pObjectAttributes->create(objReaderWork->currentObj);
	objectAttributes = pObjectAttributes;

	/* ‚±‚ê‚Í”pŽ~
	WavefrontObjFile::Material* pMaterial = pMaterial->create(objReaderWork->currentMtl);
	material = pMaterial;
	*/

	materials = WavefrontObjFile::Material::create(objReaderWork->mtltab);

}

int WavefrontObjFile::encodeAxis( Axis inFromForward, Axis inFromUp, Axis inToForward, Axis inToUp){

	int fromForward = static_cast<int>(inFromForward);
	int fromUp = static_cast<int>(inFromUp);
	int toForward = static_cast<int>(inToForward);
	int toUp = static_cast<int>(inToUp);

	int fromForwardEncoded = fromForward;
	int fromUpEncoded = fromUp << (int)powf(2.0f, 3.0f);
	int toForwardEncoded = toForward << (int)powf(2.0f, 6.0f);
	int toUpEncoded = toUp << (int)powf(2.0f, 9.0f);

	int encodedAxis = fromForwardEncoded | fromUpEncoded | toForwardEncoded | toUpEncoded;

	return encodedAxis;

}

void WavefrontObjFile::convertAxis(Axis inFromForward, Axis inFromUp, Axis inToForward, Axis inToUp) {

	int encodedAxis = encodeAxis( inFromForward, inFromUp, inToForward, inToUp );









}