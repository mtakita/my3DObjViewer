/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_OBJ_READER_BISON_TAB_H_INCLUDED
# define YY_YY_OBJ_READER_BISON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMBER = 258,
    NAME = 259,
    EOL = 260,
    O = 261,
    V = 262,
    VT = 263,
    VN = 264,
    S = 265,
    F = 266,
    G = 267,
    MTLLIB = 268,
    USEMTL = 269,
    ON = 270,
    OFF = 271,
    NEWMTL = 272,
    NS = 273,
    KA = 274,
    KD = 275,
    KS = 276,
    KE = 277,
    NI = 278,
    D = 279,
    ILLUM = 280,
    MAP_KD = 281,
    SLASH = 282
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "obj_reader_bison.y" /* yacc.c:1909  */

	struct OBJECT_ATTRIBUTES *pObjectAttributes;
	struct ATTRIBUTE_F *pAttributeF;
	struct ATTRIBUTE_V *pAttributeV;
	struct ATTRIBUTE_VN *pAttributeVN;
	struct ATTRIBUTE_VT *pAttributeVT;
	struct FACENUM* pFaceNum;
	struct VERTNUM* pVertNum;
	struct VERT2NUM* pVert2Num;
	double d;
	char* s;

	struct MATERIAL *pMaterial;

	struct ATTRIBUTE_USE_MATERIAL_LIST* pAttributeUseMaterialList;

	struct ATTRIBUTE_NS *pAttributeNs;
	struct ATTRIBUTE_KA *pAttributeKa;
	struct ATTRIBUTE_KD *pAttributeKd;
	struct ATTRIBUTE_KS *pAttributeKs;
	struct ATTRIBUTE_KE *pAttributeKe;
	struct ATTRIBUTE_NI *pAttributeNi;
	struct ATTRIBUTE_D  *pAttributeD;
	struct ATTRIBUTE_ILLUM *pAttributeIllum;
	struct ATTRIBUTE_MAPKD *pAttributeMapKd;

#line 109 "obj_reader_bison.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_OBJ_READER_BISON_TAB_H_INCLUDED  */
