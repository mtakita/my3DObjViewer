#include "pch.h"
#include "CFurBase.h"

bool CFurBase::GetFurGeometryOnOff() {
	return m_FurGeometryOnOff;
}

void CFurBase::SetFurGeometryOnOff(bool inFurGeometryOnOff) {
	m_FurGeometryOnOff = inFurGeometryOnOff;
}

void CFurBase::SetFurLayer(int inFurLayer) {
	m_FurLayer = inFurLayer;
}
void CFurBase::SetFurDepth(float inFurDepth) {
	m_FurDepth = inFurDepth;
}

int CFurBase::GetFurLayer() {
	return m_FurLayer;
}

float CFurBase::GetFurDepth() {
	return m_FurDepth;
}
