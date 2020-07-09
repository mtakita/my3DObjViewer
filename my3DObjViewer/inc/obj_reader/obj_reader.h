#ifndef _OBJ_READER_H_
#define _OBJ_READER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "obj_reader_bison.tab.h"

	extern char* dirnameForMaterial;	/* Warning that this points to a directory path name and only available during flex is pursing material file( *.mtl ). */

	extern int yylineno;

	extern int yyparse();
	extern int yylex();

	/* type definition */
	enum TYPE {
		enumFACENUM = 1,
		enumFACENUMS,
		enumV,
		enumVN,
		enumVT,
		enumS,
		enumF
	};



	/* object table */
	struct symbol {
		char* name;
		struct FACENUM* pFaceNum;
	};

  //#define NHASH 9999
#define NHASH 9999

	extern struct OBJECT_ATTRIBUTES* lookupObject(const char*);
	extern struct MATERIAL_LIBRARY* materialLibraryLookup(const char*);
	extern struct MATERIAL_LIBRARY* createMaterialLibrary(const char*);
	extern struct ATTRIBUTE_USE_MATERIAL_LIST* createUseMaterial(char* pUseMaterialName);

	struct symlist {
		struct symbol* sym;
		struct symlist* next;
	};

	struct VERTNUM {
		float x;
		float y;
		float z;
	};

	struct VERT2NUM {
		float u;
		float v;
	};

	extern struct ATTRIBUTE_VN* createAttributeVN(struct VERTNUM* pVertNum);
	extern struct ATTRIBUTE_VT* createAttributeVT(struct VERT2NUM* pVert2Num);

	extern struct VERT* createVert(float x, float y, float z);

	struct VERTLIST {
		struct VERT* vert;
		struct VERTLIST* next;
	};

	struct VT {
		//	float x;
		float u;
		//	float y;
		float v;
	};

	extern struct VT* createVt(float x, float y);

	struct VTLIST {
		struct VT* vt;
		struct VTLIST* next;
	};

	struct VN {
		float x;
		float y;
		float z;
	};

	extern struct VN* createVn(float x, float y, float z);

	struct VNLIST {
		struct VN* vn;
		struct VNLIST* next;
	};

	struct FACENUM {
		enum TYPE type;
		int vertnum;
		int texnum;
		int normnum;
		struct FACENUM* pNext;
	};

	enum FaceNumType {
		enumFaceNumType1 = 1,
		enumFaceNumType2,
		enumFaceNumType3,
		enumFaceNumType4
	};


	extern struct FACENUM* createFaceNum(enum FaceNumType type, int vertnum, int texnum, int normnum);
	extern struct VERTNUM* createVertNum(float x, float y, float z);
	extern struct VERT2NUM* createVert2Num(float u, float v);

	struct ATTRIBUTE_VN {
		enum TYPE type;
		struct VERTNUM* pVertNum;
	};

	struct ATTRIBUTE_VT {
		enum TYPE type;
		struct VERT2NUM* pVert2Num;
	};

	struct ATTRIBUTE_VN_LIST {
		struct ATTRIBUTE_VN* pAttributeVN;
		struct ATTRIBUTE_VN_LIST* pNext;
	};

	struct ATTRIBUTE_VT_LIST {
		struct ATTRIBUTE_VT* pAttributeVT;
		struct ATTRIBUTE_VT_LIST* pNext;
	};

	struct ATTRIBUTE_V {
		enum TYPE type;
		struct VERTNUM* pVertNum;
	};

	struct ATTRIBUTE_V_LIST {
		struct ATTRIBUTE_V* pAttributeV;
		struct ATTRIBUTE_V_LIST* pNext;
	};

	struct ATTRIBUTE_F {
		enum TYPE type;
		struct FACENUM* pFaceNum;
	};

	struct ATTRIBUTE_F_LIST {
		struct ATTRIBUTE_F* pAttributeF;
		struct ATTRIBUTE_F_LIST* pNext;
	};

	struct ATTRIBUTE_USE_MATERIAL_LIST {
		struct USE_MATERIAL* pUseMaterial;

		struct ATTRIBUTE_USE_MATERIAL_LIST* pNext;
	};

	extern struct ATTRIBUTE_V* createAttributeV(struct VERTNUM* pVertNum);
	extern struct ATTRIBUTE_F* createAttributeF(struct FACENUM* pFaceNum);

	struct OBJECT_ATTRIBUTES {
		const char* pObjectName;

		struct ATTRIBUTE_V_LIST* pAttributeVList;
		struct ATTRIBUTE_VT_LIST* pAttributeVTList;
		struct ATTRIBUTE_VN_LIST* pAttributeVNList;

		struct ATTRIBUTE_USE_MATERIAL_LIST* pAttributeUseMaterialList;
	};

	struct ATTRIBUTE_NS {
		float ns;
	};

	struct ATTRIBUTE_KA {
		float x;
		float y;
		float z;
	};

	struct ATTRIBUTE_KD {
		float x;
		float y;
		float z;
	};

	struct ATTRIBUTE_KS {
		float x;
		float y;
		float z;
	};

	struct ATTRIBUTE_KE {
		float x;
		float y;
		float z;
	};

	struct ATTRIBUTE_NI {
		float ni;
	};

	struct ATTRIBUTE_D {
		float d;
	};

	struct ATTRIBUTE_ILLUM {
		float illum;
	};

	struct MATERIAL {
		const char* pMaterialName;
		struct ATTRIBUTE_NS* pAttributeNs;
		struct ATTRIBUTE_KA* pAttributeKa;
		struct ATTRIBUTE_KD* pAttributeKd;
		struct ATTRIBUTE_KS* pAttributeKs;
		struct ATTRIBUTE_KE* pAttributeKe;
		struct ATTRIBUTE_NI* pAttributeNi;
		struct ATTRIBUTE_D* pAttributeD;
		struct ATTRIBUTE_ILLUM* pAttributeIllum;
		const char* pMapKd;
		const char* pDirname;
	};

	extern struct MATERIAL* createNewmtl(const char* pNewmtlName);

	struct USE_MATERIAL {
		char* pUseMaterialName;
		struct MATERIAL* pMaterial;

		struct ATTRIBUTE_F_LIST* pAttributeFList;
	};

	struct MATERIAL_LIST {
		struct MATERIAL* pMaterial;
		struct MATERIAL_LIST* pNext;
	};

	struct MATERIAL_LIBRARY {
		const char* pMaterialLibraryName;
		struct MATERIAL_LIST* pMaterialList;
	};

	extern void setAttribute(enum TYPE type, struct FACENUM* pFaceNum);


	struct OBJ {
		char* objName;
		struct VERTLIST* vertlist;
		struct VTLIST* vtlist;
		struct VNLIST* vnlist;
	};

	extern void setCurrentObj(const char* objName);

	struct OBJLIST {
		struct OBJ* obj;
		struct OBJLIST* next;
	};

	extern struct OBJECT_ATTRIBUTES* createObjectAttributes(char* name);

        extern struct OBJECT_ATTRIBUTES* pSymtab;
        extern struct MATERIAL_LIBRARY* pLibtab;
        extern struct MATERIAL* pMtltab;
        extern struct MATERIAL* pUmttab;

	extern void dump_all_objects();

	struct MATERIAL* createMaterial(const char*);
	struct MATERIAL* findMaterial(const char*);

	extern struct ATTRIBUTE_NS* createAttributeNs(float ns);
	extern struct ATTRIBUTE_KA* createAttributeKa(float x, float y, float z);
	extern struct ATTRIBUTE_KD* createAttributeKd(float x, float y, float z);
	extern struct ATTRIBUTE_KS* createAttributeKs(float x, float y, float z);
	extern struct ATTRIBUTE_KE* createAttributeKe(float x, float y, float z);
	extern struct ATTRIBUTE_NI* createAttributeNi(float ni);
	extern struct ATTRIBUTE_D* createAttributeD(float d);
	extern struct ATTRIBUTE_ILLUM* createAttributeIllum(float illum);
	extern const char* createAttributeMapKd(const char* pMapKd);
	
	extern struct ATTRIBUTE_USE_MATERIAL_LIST* getAttributeUseMaterialList(struct OBJECT_ATTRIBUTES* pObject);
	// extern int getNumOfAttributeUseMaterials();
	//extern void getNormalsWithRearrange2(float* pNormalsSuzannu, struct USE_MATERIAL* pUseMaterial);
	//extern void getTexCoordsWithRearrange2(float* pTexCoords, struct USE_MATERIAL* pUseMaterial);


	extern void yylog(char* msg);

	extern void yyerror(char *s, ...);

  extern struct OBJECT_ATTRIBUTES** ppCurrentObj;
  extern struct MATERIAL** ppCurrentMtl;
  extern struct USE_MATERIAL** ppCurrentUseMaterial;
  
  struct obj_reader_work{
    struct OBJECT_ATTRIBUTES* currentObj;
    struct MATERIAL* currentMtl;
    struct USE_MATERIAL* currentUseMaterial;

    struct OBJECT_ATTRIBUTES symtab[NHASH];
    struct MATERIAL_LIBRARY libtab[NHASH];
    struct MATERIAL mtltab[NHASH];
    struct MATERIAL umttab[NHASH];


  };

  extern int startParse(const char* fileName, struct obj_reader_work* pObjReaderWork );

  extern struct obj_reader_work* createObjReaderWork();
  extern void destroyObjReaderWork( struct obj_reader_work* );

  extern char* getRelativeFileName( const char* dirname, const char* yytext );

  extern struct obj_reader_work* readObj(const char* objFileName);
  
#ifdef __cplusplus
}
#endif

#endif

