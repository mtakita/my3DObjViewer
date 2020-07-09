#include "pch.h"
#include "CMyMFCPropertyGridCtrl.h"

void CMyMFCPropertyGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const{

	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	// Notify properties have been updated and need reflect to corresponding actor.
	*m_pPropertyUpdatedFlag = true;

	return CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}
