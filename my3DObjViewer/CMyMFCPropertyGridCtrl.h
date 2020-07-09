#pragma once
#include <afxpropertygridctrl.h>

class CMyMFCPropertyGridCtrl :
	public CMFCPropertyGridCtrl
{
public:
	CMyMFCPropertyGridCtrl(bool* pPropertyUpdatedFlag) : m_pPropertyUpdatedFlag{ pPropertyUpdatedFlag } {};
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
/*
	bool getNeedPropertyUpdateFlag() {
		return m_needPropertyUpdateFlag;
	 }
	void clearNeedPropertyUpdateFlag() {
		m_needPropertyUpdateFlag = false;
	}
	*/
private:
	bool* m_pPropertyUpdatedFlag;
};

