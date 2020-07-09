#pragma once

#include <GL/gl3w.h>

class CDeviceContext
{
public:
	void SetGLContext(CDC* pDC, HGLRC hrc);
	HGLRC GetHGLRC();
	CDC* GetDC();

private:
	HGLRC	 m_hrc;
	CDC* m_dc;
};

