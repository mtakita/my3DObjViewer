#pragma once

#include <list>
#include <vector>
#include <string>

using namespace std;

class CStringPropertyHelper
{
	vector<wstring> m_propertyNames;

public:
	CStringPropertyHelper( WCHAR* inPropertyNames[], int inNumOfPropertyNames );
	int getIndex(wstring inPropertyName);
	wstring getPropertyName(int inIndex);

};

