#pragma once

#ifndef _WAVEFRONTOBJFILE_
#define _WAVEFRONTOBJFILE_


#include <obj_reader/obj_reader.h>
#include <vector>
#include <string>
#include "mymath.h"
//#include "CConverter.h"

#define CLIPBOARDFORMAT _T("my3DObjViewer")

using namespace mymath;

using std::vector;
using std::string;

typedef struct structOneSetOfIndecies {
	enum TYPE type;
	Vectori3 indecies;
}OneSetOfIndecies;

typedef struct structFace {
//	int numOfOneSetOfIndecies;
//	OneSetOfIndecies* pOneSetOfIndecies;
	vector<OneSetOfIndecies*> pOneSetOfIndecies;
}Face;

typedef struct structUseMaterial {
	string useMaterialName;
	vector<Face>* pFaces;
}UseMaterial;

class WavefrontObjFile
{
public:
	enum class Axis { 
		X = 0, 
		Y, 
		Z, 
		minusX, 
		minusY, 
		minusZ 
	};

	//	class UseMaterials;

//	friend CConverter;

private:
	int encodeAxis(Axis inFromForward, Axis inFromUp, Axis inToForward, Axis inToUp);

public:
	void convertAxis(Axis inFromForward, Axis inFromUp, Axis inToForward, Axis inToUp);

	static WavefrontObjFile* create(struct obj_reader_work* objReaderWork);

	size_t getNumOfAttributeUseMaterials() const;

//	const UseMaterials* getUseMaterials() { return useMaterials; }
	
	size_t getNumOfUseMaterials() const {
		return objectAttributes->getUseMaterials()->getNumOfUseMaterials();
	}

	size_t getNumOfVertices() const {
		return objectAttributes->getNumOfVertices();
	}

	size_t getNumOfTexCoords() const {
		return objectAttributes->getNumOfTexCoords();
	}

	const vector<Vectorf3>& getAttributeVs() const { return objectAttributes->getAttributeVs(); }
	const vector<Vectorf2>& getAttributeVTs() const { return objectAttributes->getAttributeVTs(); }
	const vector<Vectorf3>& getAttributeVNs() const { return objectAttributes->getAttributeVNs(); }

	/*
	注意:まだ複数オブジェクト読み込みに対応してません。
	*/
	const string getObjectNames() const {
		return objectAttributes->getObjectName();
	}

protected:
	void createImpl(struct obj_reader_work* objReaderWork);




	class AttributeVs {
	public:
		static AttributeVs* create(struct ATTRIBUTE_V_LIST* pAttributeVList);
		size_t getNumOfVertices() { return attributeVs.size(); }
		const vector<Vectorf3>& getAttributeVs() { return attributeVs;  }
	protected:
		void createImpl(struct ATTRIBUTE_V_LIST* pAttributeVList);

		vector<Vectorf3> attributeVs;
	};

	class AttributeVTs {
	public:
		static AttributeVTs* create(struct ATTRIBUTE_VT_LIST* pAttributeVTList);
		size_t getNumOfTexCoords() { return attributeVTs.size(); }
		const vector<Vectorf2>& getAttributeVTs() { return attributeVTs; }

	protected:
		void createImpl(struct ATTRIBUTE_VT_LIST* pAttributeVTList);

		vector<Vectorf2> attributeVTs;

	};

	class AttributeVNs {
	public:
		static AttributeVNs* create(struct ATTRIBUTE_VN_LIST* pAttributeVNList);
		const vector<Vectorf3>& getAttributeVNs() { return attributeVNs; }

	protected:
		void createImpl(struct ATTRIBUTE_VN_LIST* pAttributeVNList);

		vector<Vectorf3> attributeVNs;

	};

	class UseMaterials {

	public:

		static UseMaterials* create(struct ATTRIBUTE_USE_MATERIAL_LIST* pAttributeUseMaterialList);
		size_t getNumOfUseMaterials() const { return useMaterials.size(); }
		const vector<UseMaterial>& getUseMaterials() const { return useMaterials; }

	protected:
		void createImpl(struct ATTRIBUTE_USE_MATERIAL_LIST* pAttributeUseMaterialList);
		vector<Face>* createAttributeF(struct ATTRIBUTE_F_LIST* attributeFList);

		vector<UseMaterial> useMaterials;

//		string useMaterialName;
//		vector<vectori3> attributeFs;

		/*これらの値の持ち場所はここではない。正しくはnewmtlに対応する場所に持つこと
		float ns;
		vectorf3 ka;
		vectorf3 kd;
		vectorf3 ks;
		vectorf3 ke;
		float ni;
		float d;
		float illum;
		string mapKd;
*/


	};

public:
	const vector<UseMaterial>& getUseMaterials() const { return objectAttributes->getUseMaterials()->getUseMaterials(); }

private:
	class ObjectAttributes{

	public:
		static ObjectAttributes* create(struct OBJECT_ATTRIBUTES* currentObj);
		size_t getNumOfVertices() { return attributeV->getNumOfVertices(); }
		size_t getNumOfTexCoords() { return attributeVT->getNumOfTexCoords(); }
		const vector<Vectorf3>& getAttributeVs() { return attributeV->getAttributeVs(); }
		const vector<Vectorf2>& getAttributeVTs() { return attributeVT->getAttributeVTs(); }
		const vector<Vectorf3>& getAttributeVNs() { return attributeVN->getAttributeVNs(); }
		const UseMaterials* getUseMaterials() const { return useMaterial; }
		string getObjectName() { return objectName;  }
	protected:
		void createImpl(struct OBJECT_ATTRIBUTES* currentObj);

	private:
		string objectName;

		AttributeVs* attributeV;

		AttributeVTs* attributeVT;

		AttributeVNs* attributeVN;

		UseMaterials* useMaterial;

	};

public:
	class Material {

	public:
//		static Material* create(struct MATERIAL* currentMtl);
		static vector<class Material*> create( const struct MATERIAL mtltab[]);


	protected:
		void createImpl(struct MATERIAL* currentMtl);

	public:
		string materialName;

		float ns;
		Vectorf3 ka;
		Vectorf3 kd;
		Vectorf3 ks;
		Vectorf3 ke;
		float ni;
		float d;
		float illum;
		string mapKd;
		string dirname;
	};

private:
	ObjectAttributes* objectAttributes;
	/* 廃止
	Material* material;		// ←こっち使っちゃダメ
	*/
	vector<WavefrontObjFile::Material*> materials;

	// UseMaterials* useMaterials; ObjectAttributesの中のuseMaterialを使用すること。

public:
	//
	// Get material.
	//
	WavefrontObjFile::Material* getMaterial(string materialName) const {

		for (WavefrontObjFile::Material* pMaterial : materials) {
			if (pMaterial->materialName == materialName) {
				return pMaterial;
			}
		}

		return nullptr;
	};
};

#endif
