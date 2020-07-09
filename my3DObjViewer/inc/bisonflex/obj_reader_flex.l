%option nounistd noyywrap nodefault yylineno case-insensitive

%x MTL

%{

#include "obj_reader_bison.tab.h"
#include "obj_reader.h"
#include <stdio.h>

#include "libgen_share.h"
#include "string_share.h"
#include "stdio_share.h"

#ifdef _MSC_VER

#define FREE_WHEN_MSC_VER_DEFINED( p ) free( p );

#else

#define FREE_WHEN_MSC_VER_DEFINED( p )

#endif

char* dirnameForMaterial;

struct bufstack {
		struct bufstack* prev;
		YY_BUFFER_STATE bs;
		int lineno;
		char* filename;
		FILE* f;
		char* realpath;
		char* basename;
		char* dirname;
}* curbs = 0;

char* curfilename = NULL;

char* getRelativeFileName( const char* dirname, const char* yytext ){

	if( dirname == NULL || yytext == NULL )
		return NULL;

	int lenDirname = strlen( dirname );
	int len = lenDirname
			+ 1 /* for '/' */
			+ strlen( yytext )
			+ 1 /* for \0 */
			;

	char* pRelativeFileName = malloc( len );
	strcpy( pRelativeFileName, dirname );
	*(pRelativeFileName+lenDirname) = '/';
	strcpy( pRelativeFileName + lenDirname + 1, yytext );

	return pRelativeFileName;

}

int popfile(){

	struct bufstack* bs = curbs;
	struct bufstack* prevbs;

	if(!bs) return 0;

	fclose( bs->f );

	free( bs->filename );

	if( bs->realpath != NULL ){
		free( bs->realpath );
	}

	if( bs->basename != NULL ){
		free( bs->basename );
	}

	if( bs->dirname != NULL ){
		free( bs->dirname );
	}

	yy_delete_buffer( bs->bs );

	prevbs = bs->prev;
	free(bs);

	if(!prevbs){
		curbs = 0;
		return 0;
	}

	yy_switch_to_buffer( prevbs->bs );

	curbs = prevbs;

	yylineno = curbs->lineno;

	curfilename = curbs->filename;
	return 1;
}

int newfile( const char* fn ){

//	FILE* f = fopen( fn, "r" );
	MYFOPEN(f, fn, "r")

	struct bufstack* bs = malloc( sizeof(struct bufstack) );
	
	if(!f){	perror(fn); return 0; }
	if(!bs){ perror("malloc"); exit(1); }

	// Getting real path of the file.
	char* pRealPath = realpath( fn, NULL );
	if( pRealPath != NULL ){
		bs->realpath = strdup( pRealPath );
	}else{
		bs->realpath = NULL;
	}
	FREE_WHEN_MSC_VER_DEFINED( pRealPath )

	// Getting basename of the file.
	char* pBaseName = basename( bs->realpath );
	bs->basename = strdup( pBaseName );
	FREE_WHEN_MSC_VER_DEFINED( pBaseName )

	// Getting dirname of the file.
	char* pDirName = dirname( bs->realpath );
	bs->dirname = strdup( pDirName );

    // Tossing dir path to bison.
    dirnameForMaterial = strdup(pDirName);

	FREE_WHEN_MSC_VER_DEFINED( pDirName )

	

	if(curbs){ curbs->lineno = yylineno; }

	bs->prev = curbs;
	bs->bs = yy_create_buffer(f, YY_BUF_SIZE);
	bs->f = f;
	bs->filename = strdup(fn);

	yy_switch_to_buffer(bs->bs);

	curbs = bs;
	yylineno = 1;
	curfilename = bs->filename;

	return 1;
}

int startParse( const char* fileName, struct obj_reader_work* pObjReaderWork  ){

	// Initialize all global variables before start parsing.
	pSymtab = pObjReaderWork->symtab;
	pLibtab = pObjReaderWork->libtab;
	pMtltab = pObjReaderWork->mtltab;
	pUmttab = pObjReaderWork->umttab;

	ppCurrentObj = &pObjReaderWork->currentObj;
	ppCurrentMtl = &pObjReaderWork->currentMtl;
	ppCurrentUseMaterial = &pObjReaderWork->currentUseMaterial;

	newfile( fileName );
        yyparse();
}

struct obj_reader_work* createObjReaderWork(){
	struct obj_reader_work* pObjReaderWork = (struct obj_reader_work*)malloc( sizeof( struct obj_reader_work ) );
	memset( pObjReaderWork, 0, sizeof( struct obj_reader_work ) );
	return pObjReaderWork;
}

void destroyObjReaderWork( struct obj_reader_work* pObjReaderWork ){
	free( pObjReaderWork );
}

struct obj_reader_work* readObj( const char* objFileName ){

	struct obj_reader_work* pObjReaderWork = createObjReaderWork();
	startParse( objFileName, pObjReaderWork );

	return pObjReaderWork;

}

%}

/* float exponent */
EXP ([Ee][-+]?[0-9]+)
SYMBOL ([:.\_\\\-a-zA-Z0-9])+
%%

 /* keywords for *.mtl file */
"newmtl"		{ yylog("newmtl has found.\n"); return NEWMTL;}
"ns"			{ return NS;}
"ka"			{ return KA;}
"kd"			{ return KD;}
"ks"			{ return KS;}
"ke"			{ return KE;}
"ni"			{ return NI;}
"d"			{ return D;}
"illum"			{ return ILLUM;}
"map_kd"		{ return MAP_KD;}


 /* keywords */

"o"			{ yylog( "o has been found.\n"); return O;}
"v"			{ return V;}
"vn"			{ return VN;}
"vt"			{ return VT;}
"s"			{ return S;}
"f"			{ return F;}
"g"                     { return G;}

"mtllib"[ \t]+		{ yylog("mtllib has been found.\n" ); BEGIN MTL; } 
<MTL>[^ \t\n]+		{
				{
					int c;
					while(( c = input() ) && c != '\n' );
				}
				yylineno++;

				char* pRelativeFilename = getRelativeFileName( curbs->dirname, yytext );
				char* pMaterialFileName = strdup(yytext);
/*				if(!newfile(yytext)){ */
				if(!newfile(pRelativeFilename)){
					yyterminate();
				}
				else{
					createMaterialLibrary( pMaterialFileName );
				}

				free( pRelativeFilename ); pRelativeFilename = 0;
				free( pMaterialFileName ); pMaterialFileName = 0;

				BEGIN INITIAL;
			}

"usemtl"		{ return USEMTL;}

"on" 			{ return ON; }
"off" 			{ return OFF; }

"/"			{ return SLASH; }


[-+]?[0-9]+"."[0-9]*{EXP}? | 
"."?[0-9]+{EXP}?		{ yylval.d = atof(yytext); return NUMBER; }

{SYMBOL}			{ yylval.s = yytext; return NAME; }
 # /* ([._\-a-zA-Z0-9])+\.[mM][tT][lL] 	{ yylval.s = yytext; return FILENAME; } */

 # /* ([a-zA-Z0-9:\\])+\.[jJ][pP][gG] { yylval.s = yytext; return FILEPATH; } */

 # /* ([a-zA-Z]:[/\\])?(([a-zA-Z0-9_]+)?[/\\])*[a-zA-Z0-9_]+\.[a-zA-Z0-9_]{1,3} { yylval.s = yytext; return FILEPATH; } */

 # /* {SYMBOL}			{ yylval.s = yytext; return FILEPATH; } */

"#".*
[ \t\n]  	/* ignore spaces */
^[ \t]*$	/* ignore blank lines */

 /* \n			{ return EOL; } */
 /* \n			{ } */
 /* <<EOF>>		{ yylog("flex has reached the EOF.\n"); if(!popfile()) yyterminate();  ?? return EOF;  } */
<<EOF>>			{ yylog("flex has reached the EOF.\n"); if(!popfile()) yyterminate(); /* return EOF; */ }

.			{ yyerror("Mystery character %c\n", *yytext); }
%%

