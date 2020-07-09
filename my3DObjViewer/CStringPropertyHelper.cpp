#include "pch.h"
#include "CStringPropertyHelper.h"

CStringPropertyHelper::CStringPropertyHelper(WCHAR* inPropertyNames[], int inNumOfPropertyNames ) {
	
	for (int i= 0; i < inNumOfPropertyNames; i++) {
		m_propertyNames.push_back(wstring(inPropertyNames[i]));
	}
}

int CStringPropertyHelper::getIndex(wstring inPropertyName) {

	for (int i = 0; i < m_propertyNames.size(); i++) {
		if (m_propertyNames[i] == inPropertyName) {
			return i;
		}
	}

	return -1;
}

wstring CStringPropertyHelper::getPropertyName(int inIndex) {
	return m_propertyNames[inIndex];
}
