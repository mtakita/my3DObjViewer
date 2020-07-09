#include "pch.h"
#include "CMyEnginePropertiesWnd.h"

BEGIN_MESSAGE_MAP(CMyEnginePropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()


int CMyEnginePropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B
//	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndViewportPropList.Create(dwViewStyle, rectDummy, this, 1)) {
		TRACE0("MyEngine Viewport�v���p�e�B �O���b�h���쐬�ł��܂���ł���\n");
		return -1;
	}

	if (!m_wndPerspectiveTransformPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2)) {
		TRACE0("MyEngine Perspective Transformation�v���p�e�B �O���b�h���쐬�ł��܂���ł���\n");
		return -1;
	}

	InitViewportPropList();
	InitPerspectiveTransformPropList();

	AdjustLayout();

	return 0;
}

void CMyEnginePropertiesWnd::InitPerspectiveTransformPropList()
{
	// TODO: �����Ɏ����R�[�h��ǉ����܂�.
	SetPropListFont();

	m_wndPerspectiveTransformPropList.EnableHeaderCtrl(FALSE);
	m_wndPerspectiveTransformPropList.EnableDescriptionArea();
	m_wndPerspectiveTransformPropList.SetVSDotNetLook();
	m_wndPerspectiveTransformPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pPerspectiveTransform = new CMFCPropertyGridProperty(_T("Perspective Transform"));
	CMFCPropertyGridProperty* pFov = new CMFCPropertyGridProperty(_T("FOV"));
	CMFCPropertyGridProperty* pFrustum = new CMFCPropertyGridProperty(_T("Frustum"));

	int index = 0;

	pPerspectiveTransform->AddSubItem(pFov);
	idxFov = index;
	index++;

	pPerspectiveTransform->AddSubItem(pFrustum);
	idxFrustum = index;
	index++;

	index = 0;
	pFov->AddSubItem(new CMFCPropertyGridProperty(_T("Horizontal FOV"), (_variant_t)45.0f, _T("Horizontal FOV��ݒ肵�܂�")));
	idxHorizontalFov = index;
	index++;
	pFov->AddSubItem(new CMFCPropertyGridProperty(_T("(���g�p)Vertical FOV"), (_variant_t)45.0f, _T("(���g�p)Vertical FOV��ݒ肵�܂�")));
	idxVerticalFov = index;
	index++;

	float L = 5.0f;

	index = 0;
	pFrustum->AddSubItem(new CMFCPropertyGridProperty(_T("Left"), (_variant_t)-L, _T("Frustum Left��ݒ肵�܂�")));
	idxFrustumLeft = index;
	index++;
	pFrustum->AddSubItem(new CMFCPropertyGridProperty(_T("Right"), (_variant_t)+L, _T("Frustum Right��ݒ肵�܂�")));
	idxFrustumRight = index;
	index++;
	pFrustum->AddSubItem(new CMFCPropertyGridProperty(_T("Top"), (_variant_t)+L, _T("Frustum Top��ݒ肵�܂�")));
	idxFrustumTop = index;
	index++;
	pFrustum->AddSubItem(new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)-L, _T("Frustum Bottom��ݒ肵�܂�")));
	idxFrustumBottom = index;
	index++;
	pFrustum->AddSubItem(new CMFCPropertyGridProperty(_T("Near"), (_variant_t)5.0f, _T("Frustum Near��ݒ肵�܂�")));
	idxFrustumNear = index;
	index++;
	pFrustum->AddSubItem(new CMFCPropertyGridProperty(_T("Far"), (_variant_t)5000.0f, _T("Frustum Far��ݒ肵�܂�")));
	idxFrustumFar = index;
	index++;

	index = 0;
	m_wndPerspectiveTransformPropList.AddProperty(pPerspectiveTransform);
	idxPerspectiveTransform = index;
	index++;


}


void CMyEnginePropertiesWnd::InitViewportPropList()
{
	// TODO: �����Ɏ����R�[�h��ǉ����܂�.
	SetPropListFont();

	int index = 0;

	m_wndViewportPropList.EnableHeaderCtrl(FALSE);
	m_wndViewportPropList.EnableDescriptionArea();
	m_wndViewportPropList.SetVSDotNetLook();
	m_wndViewportPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pViewport = new CMFCPropertyGridProperty(_T("Viewport"));
	CMFCPropertyGridProperty* pTranslation = new CMFCPropertyGridProperty(_T("Axis Translation"));
	CMFCPropertyGridProperty* pRotation = new CMFCPropertyGridProperty(_T("Axis Rotation"));
	index = 0;
	pViewport->AddSubItem(pTranslation);
	idxTranslation = index;
	index++;
	pViewport->AddSubItem(pRotation);
	idxRotation = index;
	index++;

	index = 0;
	pTranslation->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t)0.0f, _T("Viewport��Word Origin�����X���W�����ړ��l��ݒ肵�܂�")));
	idxTranslationX = index;
	index++;
	pTranslation->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t)300.0f, _T("Viewport��Word Origin�����Y���W�����ړ��l��ݒ肵�܂�")));
	idxTranslationY = index;
	index++;
	pTranslation->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t)100.0f, _T("Viewport��Word Origin�����Z���W�����ړ��l��ݒ肵�܂�")));
	idxTranslationZ = index;
	index++;

	index = 0;
	pRotation->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t)0.0f, _T("Viewport��X-Axis��]���W��ݒ肵�܂�")));
	idxXRotation = index;
	index++;
	pRotation->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t)0.0f, _T("Viewport��Y-Axis��]���W��ݒ肵�܂�")));
	idxYRotation = index;
	index++;
	pRotation->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t)0.0f, _T("Viewport��Z-Axis��]���W��ݒ肵�܂�")));
	idxZRotation = index;
	index++;

	idxViewport = m_wndViewportPropList.AddProperty(pViewport);

}

void CMyEnginePropertiesWnd::SetPropListFont()
{
	// TODO: �����Ɏ����R�[�h��ǉ����܂�.
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndViewportPropList.SetFont(&m_fntPropList);
	m_wndPerspectiveTransformPropList.SetFont(&m_fntPropList);

}


void CMyEnginePropertiesWnd::AdjustLayout()
{
	// TODO: �����Ɏ����R�[�h��ǉ����܂�.
	if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic())) {
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	CRect viewportPropListClientRect;
	m_wndViewportPropList.CalcWindowRect(viewportPropListClientRect);
	int viewportPropListHeight = 500;

	m_wndViewportPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), viewportPropListHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPerspectiveTransformPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + viewportPropListHeight, rectClient.Width(), rectClient.Height() - viewportPropListHeight, SWP_NOACTIVATE | SWP_NOZORDER);


}


void CMyEnginePropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}


void CMyEnginePropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndViewportPropList.SetFocus();
	m_wndPerspectiveTransformPropList.SetFocus();
}


void CMyEnginePropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CMyEnginePropertiesWnd::updateViewportProperty(CViewportProperty viewportProperty) {

	float transX = viewportProperty.GetViewportTranslationX();
	float transY = viewportProperty.GetViewportTranslationY();
	float transZ = viewportProperty.GetViewportTranslationZ();

	float rotationX = viewportProperty.GetViewportRotationX();
	float rotationY = viewportProperty.GetViewportRotationY();
	float rotationZ = viewportProperty.GetViewportRotationZ();

	float horizontalFov = viewportProperty.GetHorizontalFov();
	float verticalFov = viewportProperty.GetVerticalFov();

	float perspectiveFrustumLeft = viewportProperty.GetPerspectiveFrustumLeft();
	float perspectiveFrustumRight = viewportProperty.GetPerspectiveFrustumRight();
	float perspectiveFrustumTop = viewportProperty.GetPerspectiveFrustumTop();
	float perspectiveFrustumBottom = viewportProperty.GetPerspectiveFrustumBottom();
	float perspectiveFrustumNear = viewportProperty.GetPerspectiveFrustumNear();
	float perspectiveFrustumFar = viewportProperty.GetPerspectiveFrustumFar();

	CMFCPropertyGridProperty* pViewportProperty = (CMFCPropertyGridProperty*)m_wndViewportPropList.GetProperty(idxViewport);

	// Obtain property for translation.
	CMFCPropertyGridProperty* pViewportTranslation = (CMFCPropertyGridProperty*)pViewportProperty->GetSubItem(idxTranslation);

	// Obtain propeties for viewport translation.
	CMFCPropertyGridProperty* pViewportTranslationX = (CMFCPropertyGridProperty*)pViewportTranslation->GetSubItem(idxTranslationX);
	pViewportTranslationX->SetValue( transX );
	CMFCPropertyGridProperty* pViewportTranslationY = (CMFCPropertyGridProperty*)pViewportTranslation->GetSubItem(idxTranslationY);
	pViewportTranslationY->SetValue(transY);
	CMFCPropertyGridProperty* pViewportTranslationZ = (CMFCPropertyGridProperty*)pViewportTranslation->GetSubItem(idxTranslationZ);
	pViewportTranslationZ->SetValue(transZ);

	// Obtain property for rotation.
	CMFCPropertyGridProperty* pViewportRotation = (CMFCPropertyGridProperty*)pViewportProperty->GetSubItem(idxRotation);

	// Obtain propeties for X rotation.
	CMFCPropertyGridProperty* pViewportXRotation = (CMFCPropertyGridProperty*)pViewportRotation->GetSubItem(idxXRotation);
	pViewportXRotation->SetValue(rotationX);

	// Obtain propeties for Y rotation.
	CMFCPropertyGridProperty* pViewportYRotation = (CMFCPropertyGridProperty*)pViewportRotation->GetSubItem(idxYRotation);
	pViewportYRotation->SetValue(rotationY);

	// Obtain propeties for Z rotation.
	CMFCPropertyGridProperty* pViewportZRotation = (CMFCPropertyGridProperty*)pViewportRotation->GetSubItem(idxZRotation);
	pViewportZRotation->SetValue(rotationZ);


	// Perspective Transform.
	CMFCPropertyGridProperty* pPerspectiveTransformProperty = (CMFCPropertyGridProperty*)m_wndPerspectiveTransformPropList.GetProperty(idxPerspectiveTransform);

	// Obtain FOV for perspective transform.
	CMFCPropertyGridProperty* pPerspectiveFovProperty = (CMFCPropertyGridProperty*)pPerspectiveTransformProperty->GetSubItem(idxFov);

	CMFCPropertyGridProperty* pHoriontalFov = (CMFCPropertyGridProperty*)pPerspectiveFovProperty->GetSubItem(idxHorizontalFov);
	pHoriontalFov->SetValue(horizontalFov);
	CMFCPropertyGridProperty* pVerticalFov = (CMFCPropertyGridProperty*)pPerspectiveFovProperty->GetSubItem(idxVerticalFov);
	pVerticalFov->SetValue(verticalFov);


	// Frustum
	CMFCPropertyGridProperty* pPerspectiveFrustumProperty = (CMFCPropertyGridProperty*)pPerspectiveTransformProperty->GetSubItem(idxFrustum);

	// Obtain propeties for Perspective Frustum.
	CMFCPropertyGridProperty* pPerspectiveFrustumLeftProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumLeft);
	pPerspectiveFrustumLeftProperty->SetValue(perspectiveFrustumLeft);
	CMFCPropertyGridProperty* pPerspectiveFrustumRightProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumRight);
	pPerspectiveFrustumRightProperty->SetValue(perspectiveFrustumRight);
	CMFCPropertyGridProperty* pPerspectiveFrustumTopProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumTop);
	pPerspectiveFrustumTopProperty->SetValue(perspectiveFrustumTop);
	CMFCPropertyGridProperty* pPerspectiveFrustumBottomProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumBottom);
	pPerspectiveFrustumBottomProperty->SetValue(perspectiveFrustumBottom);
	CMFCPropertyGridProperty* pPerspectiveFrustumNearProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumNear);
	pPerspectiveFrustumNearProperty->SetValue(perspectiveFrustumNear);
	CMFCPropertyGridProperty* pPerspectiveFrustumFarProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumFar);
	pPerspectiveFrustumFarProperty->SetValue(perspectiveFrustumFar);

}

CViewportProperty CMyEnginePropertiesWnd::getViewportTransltion()
{
	// TODO: �����Ɏ����R�[�h��ǉ����܂�.

	CViewportProperty viewportProp;

	CMFCPropertyGridProperty* pViewportProperty = (CMFCPropertyGridProperty * )m_wndViewportPropList.GetProperty( idxViewport );

	// Obtain property for translation.
	CMFCPropertyGridProperty* pViewportTranslation = (CMFCPropertyGridProperty*)pViewportProperty->GetSubItem(idxTranslation);

	// Obtain propeties for viewport translation.
	CMFCPropertyGridProperty* pViewportTranslationX = (CMFCPropertyGridProperty*)pViewportTranslation->GetSubItem(idxTranslationX);
	float transX = pViewportTranslationX->GetValue().fltVal;
	CMFCPropertyGridProperty* pViewportTranslationY = (CMFCPropertyGridProperty*)pViewportTranslation->GetSubItem(idxTranslationY);
	float transY = pViewportTranslationY->GetValue().fltVal;
	CMFCPropertyGridProperty* pViewportTranslationZ = (CMFCPropertyGridProperty*)pViewportTranslation->GetSubItem(idxTranslationZ);
	float transZ = pViewportTranslationZ->GetValue().fltVal;

	viewportProp.SetViewportTranslationX(transX);
	viewportProp.SetViewportTranslationY(transY);
	viewportProp.SetViewportTranslationZ(transZ);

	// Obtain property for rotation.
	CMFCPropertyGridProperty* pViewportRotation = (CMFCPropertyGridProperty*)pViewportProperty->GetSubItem(idxRotation);

	// Obtain propeties for X rotation.
	CMFCPropertyGridProperty* pViewportXRotation = (CMFCPropertyGridProperty*)pViewportRotation->GetSubItem(idxXRotation);
	float rotationX = pViewportXRotation->GetValue().fltVal;
	viewportProp.SetViewportRotationX(rotationX);

	// Obtain propeties for Y Rotation.
	CMFCPropertyGridProperty* pViewportYRotation = (CMFCPropertyGridProperty*)pViewportRotation->GetSubItem(idxYRotation);
	float rotationY = pViewportYRotation->GetValue().fltVal;
	viewportProp.SetViewportRotationY(rotationY);

	// Obtain propeties for Z rotation.
	CMFCPropertyGridProperty* pViewportZRotation = (CMFCPropertyGridProperty*)pViewportRotation->GetSubItem(idxZRotation);
	float rotationZ = pViewportZRotation->GetValue().fltVal;
	viewportProp.SetViewportRotationZ(rotationZ);

	// Perspective Transform.
	CMFCPropertyGridProperty* pPerspectiveTransformProperty = (CMFCPropertyGridProperty*)m_wndPerspectiveTransformPropList.GetProperty(idxPerspectiveTransform);

	// Obtain FOV for perspective transform.
	CMFCPropertyGridProperty* pPerspectiveFovProperty = (CMFCPropertyGridProperty*)pPerspectiveTransformProperty->GetSubItem(idxFov);

	CMFCPropertyGridProperty* pHoriontalFov = (CMFCPropertyGridProperty*)pPerspectiveFovProperty->GetSubItem(idxHorizontalFov);
	float horizontalFov = pHoriontalFov->GetValue().fltVal;
	CMFCPropertyGridProperty* pVerticalFov = (CMFCPropertyGridProperty*)pPerspectiveFovProperty->GetSubItem(idxVerticalFov);
	float verticalFov = pVerticalFov->GetValue().fltVal;

	viewportProp.SetHorizontalFov(horizontalFov);
	viewportProp.SetVerticalFov(verticalFov);


	// Frustum
	CMFCPropertyGridProperty* pPerspectiveFrustumProperty = (CMFCPropertyGridProperty*)pPerspectiveTransformProperty->GetSubItem(idxFrustum);

	// Obtain propeties for Perspective Frustum.
	CMFCPropertyGridProperty* pPerspectiveFrustumLeftProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumLeft);
	float perspectiveFrustumLeft = pPerspectiveFrustumLeftProperty->GetValue().fltVal;
	CMFCPropertyGridProperty* pPerspectiveFrustumRightProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumRight);
	float perspectiveFrustumRight = pPerspectiveFrustumRightProperty->GetValue().fltVal;
	CMFCPropertyGridProperty* pPerspectiveFrustumTopProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumTop);
	float perspectiveFrustumTop = pPerspectiveFrustumTopProperty->GetValue().fltVal;
	CMFCPropertyGridProperty* pPerspectiveFrustumBottomProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumBottom);
	float perspectiveFrustumBottom = pPerspectiveFrustumBottomProperty->GetValue().fltVal;
	CMFCPropertyGridProperty* pPerspectiveFrustumNearProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumNear);
	float perspectiveFrustumNear = pPerspectiveFrustumNearProperty->GetValue().fltVal;
	CMFCPropertyGridProperty* pPerspectiveFrustumFarProperty = (CMFCPropertyGridProperty*)pPerspectiveFrustumProperty->GetSubItem(idxFrustumFar);
	float perspectiveFrustumFar = pPerspectiveFrustumFarProperty->GetValue().fltVal;

	viewportProp.SetPerspectiveFrustumLeft(perspectiveFrustumLeft);
	viewportProp.SetPerspectiveFrustumRight(perspectiveFrustumRight);
	viewportProp.SetPerspectiveFrustumTop(perspectiveFrustumTop);
	viewportProp.SetPerspectiveFrustumBottom(perspectiveFrustumBottom);
	viewportProp.SetPerspectiveFrustumNear(perspectiveFrustumNear);
	viewportProp.SetPerspectiveFrustumFar(perspectiveFrustumFar);



	return viewportProp;
}


void CMyEnginePropertiesWnd::DoDataExchange(CDataExchange* pDX)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	CDockablePane::DoDataExchange(pDX);
}
