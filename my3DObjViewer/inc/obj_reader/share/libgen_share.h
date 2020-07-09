
#ifndef _LIBGEN_WIN_H_
#define _LIBGEN_WIN_H_

#ifdef __cplusplus
extern "C" {
#endif

char* dirname_win(char *path);
char* basename_win(char *path);
char* realpath_win(const char *path, char *resolved_path);

#ifdef _MSC_VER

#define dirname( path ) dirname_win( path )
#define basename( path ) basename_win( path )
#define realpath( path, resolved_path ) realpath_win( path, resolved_path )

#else

#include <libgen.h>

#endif


#ifdef __cplusplus
}
#endif

#endif
