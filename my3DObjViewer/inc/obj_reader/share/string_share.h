
#ifndef _STRING_WIN_H_
#define _STRING_WIN_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _MSC_VER

#define strdup( strSource ) _strdup( strSource )
#define strcpy( dest, src ) strcpy_win( dest, src )

#endif


#ifdef __cplusplus
}
#endif

#endif
