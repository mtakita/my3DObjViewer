
#ifdef _MSC_VER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libgen_share.h"

extern char* dirnameForMaterial;

/*
 if this function return not NULL, it must be freed.
*/

void killLastDelimiter( char* dir, int len ){

	if( len == 0 )
		return;

	if( dir[len-1] == '\\' ){
		dir[len-1] = '\0';
	}

	return;
}

char* dirname_win(char* path) {

    if (path == NULL)
        return NULL;

    char drive[256];
    char dir[256];
    char fname[256];
    char ext[256];

    errno_t err = _splitpath_s(
        path,
        drive,
        sizeof(drive),
        dir,
        sizeof(dir),
        fname,
        sizeof(fname),
        ext,
        sizeof(ext)
    );

    if (err != 0) {

        // an error occurred.
        return NULL;
    }

    int len = strlen( dir );
    killLastDelimiter( dir, len );
    len = strlen( dir );

    char* pDir = malloc(len+1L);
    err = strcpy_s(pDir, len + 1L, dir);
    if (err != 0) {
        return NULL;
    }

    return pDir;
}

/*
 if this function return not NULL, it must be freed.
*/
char* basename_win(char* path) {

    if (path == NULL)
        return NULL;

    char drive[256];
    char dir[256];
    char fname[256];
    char ext[256];

    errno_t err = _splitpath_s(
        path,
        drive,
        sizeof(drive),
        dir,
        sizeof(dir),
        fname,
        sizeof(fname),
        ext,
        sizeof(ext)
    );

    if (err != 0) {

        // an error occurred.
        return NULL;
    }

    int len = strlen(fname);
    char* pFname = malloc(len + 1L);
    err = strcpy_s(pFname, len + 1L, fname);
    if (err != 0) {
        return NULL;
    }

    return pFname;
}

/*
 if this function return not NULL, it must be freed.
*/
char* realpath_win(const char* path, char* resolved_path) {

    char fullPath[256];
    char* pFullPath = _fullpath(
                        fullPath,
                        path,
                        sizeof(fullPath)
                    );
    if (pFullPath == NULL) {
        return NULL;
    }

    int len = strlen(pFullPath);
    char* pFullPathRet = malloc(len + 1L);
    errno_t err = strcpy_s(pFullPathRet, len + 1L, pFullPath);
    if (err != 0) {
        return NULL;
    }

    return pFullPathRet;
}

#endif
