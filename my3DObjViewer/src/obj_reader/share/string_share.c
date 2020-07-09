
#ifdef _MSC_VER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_share.h"

/*
 if this function return not NULL, it must be freed.
*/
char* strcpy_win(char* dest, const char* src){

	while (*src != '\0' ) {
		*dest++ = *src++;
	}

	*dest++ = '\0';

	return dest;

}

#endif
