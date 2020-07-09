#include "pch.h"
#include "CTessellationBase.h"

void CTessellationBase::SetTessellationOnOff(bool inTessellationOnOff) {
	m_TessellationOnOff = inTessellationOnOff;
}
void CTessellationBase::SetTessellationDomain(int inTessellationDomain) {
	m_TessellationDomain = inTessellationDomain;
}
void CTessellationBase::SetTessellationLevelFactorOuter0(int inTessellationFactorOuter0) {
	m_TessellationFactorOuter0 = inTessellationFactorOuter0;
}
void CTessellationBase::SetTessellationLevelFactorOuter1(int inTessellationFactorOuter1) {
	m_TessellationFactorOuter1 = inTessellationFactorOuter1;
}
void CTessellationBase::SetTessellationLevelFactorOuter2(int inTessellationFactorOuter2) {
	m_TessellationFactorOuter2 = inTessellationFactorOuter2;
}
void CTessellationBase::SetTessellationLevelFactorOuter3(int inTessellationFactorOuter3) {
	m_TessellationFactorOuter3 = inTessellationFactorOuter3;
}
void CTessellationBase::SetTessellationLevelFactorInner0(int inTessellationFactorInner0) {
	m_TessellationFactorInner0 = inTessellationFactorInner0;
}
void CTessellationBase::SetTessellationLevelFactorInner1(int inTessellationFactorInner1) {
	m_TessellationFactorInner1 = inTessellationFactorInner1;
}
bool CTessellationBase::GetTessellationOnOff() {
	return m_TessellationOnOff;
}
int CTessellationBase::GetTessellationDomain() {
	return m_TessellationDomain;
}
int CTessellationBase::GetTessellationLevelFactorOuter0() {
	return m_TessellationFactorOuter0;
}
int CTessellationBase::GetTessellationLevelFactorOuter1() {
	return m_TessellationFactorOuter1;
}
int CTessellationBase::GetTessellationLevelFactorOuter2() {
	return m_TessellationFactorOuter2;
}
int CTessellationBase::GetTessellationLevelFactorOuter3() {
	return m_TessellationFactorOuter3;
}
int CTessellationBase::GetTessellationLevelFactorInner0() {
	return m_TessellationFactorInner0;
}
int CTessellationBase::GetTessellationLevelFactorInner1() {
	return m_TessellationFactorInner1;
}
