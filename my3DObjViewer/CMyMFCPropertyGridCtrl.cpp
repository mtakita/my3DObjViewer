#include "pch.h"
#include "CMyMFCPropertyGridCtrl.h"

void CMyMFCPropertyGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const{

	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	// Notify properties have been updated and need reflect to corresponding actor.
	*m_pPropertyUpdatedFlag = true;

	return CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}
