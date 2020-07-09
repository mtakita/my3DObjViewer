#include "pch.h"

#include <GL/gl3w.h>

#ifdef _WIN32
ULONGLONG       m_appStartTime;
#else
struct timeval  m_appStartTime;
#endif

unsigned int app_time()
{
#ifdef _WIN32
	ULONGLONG currentTime = ::GetTickCount64();

	return (unsigned int)(currentTime - m_appStartTime);
#else
	return 0;
#endif
}

