#include "pch.h"
#include "CDeviceContext.h"


void CDeviceContext::SetGLContext(CDC* pDC, HGLRC hrc)
{
	m_hrc = hrc;
	m_dc = pDC;
}

HGLRC CDeviceContext::GetHGLRC() {
	return m_hrc;
}

CDC* CDeviceContext::GetDC() {
	return m_dc;
}
