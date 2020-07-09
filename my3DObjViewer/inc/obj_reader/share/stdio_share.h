
#ifndef _STDIO_WIN_H_
#define _STDIO_WIN_H_


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER

#define fileno _fileno

#define MYFOPEN( f, filename, mode ) \
	FILE* f; \
	errno_t err = fopen_s( &f, filename, mode );

#else

#define MYFOPEN( f, filename, mode ) \
	FILE* f = fopen( filename, mode );
#endif

#ifdef __cplusplus
}
#endif

#endif
