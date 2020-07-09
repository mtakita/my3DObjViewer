#include "pch.h"
#include "CActorPropertiesWnd.h"
#include "CActorProperty.h"
#include "CStringPropertyHelper.h"

BEGIN_MESSAGE_MAP(CActorPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETTINGCHANGE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

void CActorPropertiesWnd::SetPropListFont()
{
	// TODO: ここに実装コードを追加します.
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

	m_wndActorPropList.SetFont(&m_fntPropList);

}

void CActorPropertiesWnd::InitActorPropList()
{
	// TODO: ここに実装コードを追加します.
	SetPropListFont();

	int index = 0;

	m_wndActorPropList.EnableHeaderCtrl(FALSE);
	m_wndActorPropList.EnableDescriptionArea();
	m_wndActorPropList.SetVSDotNetLook();
	m_wndActorPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pActor = new CMFCPropertyGridProperty(_T("Actor"));

	CMFCPropertyGridProperty* pTranslation = new CMFCPropertyGridProperty(_T("Translation"));
	CMFCPropertyGridProperty* pRotation = new CMFCPropertyGridProperty(_T("Rotation"));
	CMFCPropertyGridProperty* pScale = new CMFCPropertyGridProperty(_T("Scale"));
	CMFCPropertyGridProperty* pTessellation = new CMFCPropertyGridProperty(_T("Tessellation"));

	index = 0;
	pActor->AddSubItem(pTranslation);
	idxTranslation = index;
	index++;
	pActor->AddSubItem(pRotation);
	idxRotation = index;
	index++;
	pActor->AddSubItem(pScale);
	idxScale = index;
	index++;
	pActor->AddSubItem(pTessellation);
	idxTessellation = index;
	index++;

	index = 0;
	pTranslation->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t)0.0f, _T("ActorのX座標を設定します")));
	idxTranslationX = index;
	index++;
	pTranslation->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t)0.0f, _T("ActorのY座標を設定します")));
	idxTranslationY = index;
	index++;
	pTranslation->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t)0.0f, _T("ActorのZ座標を設定します")));
	idxTranslationZ = index;
	index++;


	index = 0;
	pRotation->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t)0.0f, _T("ActorのX軸の回転角度を設定します")));
	idxRotationX = index;
	index++;
	pRotation->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t)0.0f, _T("ActorのY軸の回転角度を設定します")));
	idxRotationY = index;
	index++;
	pRotation->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t)0.0f, _T("ActorのZ軸の回転角度を設定します")));
	idxRotationZ = index;
	index++;

	index = 0;
	pScale->AddSubItem(new CMFCPropertyGridProperty(_T("k"), (_variant_t)0.0f, _T("Actorのスケーリングを設定します")));
	idxScale_k = index;
	index++;

	idxActor = m_wndActorPropList.AddProperty(pActor);


	CMFCPropertyGridProperty* pTessellationFunc = new CMFCPropertyGridProperty(_T("Tessellation On/Off"));
	CMFCPropertyGridProperty* pTessellationDomain = new CMFCPropertyGridProperty(_T("Tessellation Domain"));
	CMFCPropertyGridProperty* pTessellationLevelFactors = new CMFCPropertyGridProperty(_T("Tessellation Level Factors"));

	index = 0;
	pTessellation->AddSubItem(pTessellationFunc);
	idxTessellationFunc = index;
	index++;
	pTessellation->AddSubItem(pTessellationDomain);
	idxTessellationDomain = index;
	index++;
	pTessellation->AddSubItem(pTessellationLevelFactors);
	idxTessellationLevelFactor = index;
	index++;

	index = 0;
	pTessellationFunc->AddSubItem(new CMFCPropertyGridProperty(_T("テセレーション"), (_variant_t)false, _T("テセレーションのOn/Offを切り替えます")));
	idxTessellationFuncOnOff = index;
	index++;

	index = 0;
	CMFCPropertyGridProperty* pTessellationDomainProp = new CMFCPropertyGridProperty(_T("テセレーションドメイン"), (_variant_t)(TESSELLATION_DOAMIN_TRIANGLE), _T("次のうちのどれかです : Quad、Isoline、Triangle"));
	pTessellationDomain->AddSubItem(pTessellationDomainProp);
	idxTessellationDomainQuadTriaLine = index;
	pTessellationDomainProp->AddOption(TESSELLATION_DOAMIN_QUAD);
	pTessellationDomainProp->AddOption(TESSELLATION_DOAMIN_ISOLINE);
	pTessellationDomainProp->AddOption(TESSELLATION_DOAMIN_TRIANGLE);
	pTessellationDomainProp->AllowEdit(FALSE);

	index = 0;
	pTessellationLevelFactors->AddSubItem(new CMFCPropertyGridProperty(_T("gl_TessLevelOuter[0]"), (_variant_t)((int)2), _T("テセレーションレベル0を指定します")));
	idxTessellationLevelFactorOuter0 = index;
	index++;
	pTessellationLevelFactors->AddSubItem(new CMFCPropertyGridProperty(_T("gl_TessLevelOuter[1]"), (_variant_t)((int)3), _T("テセレーションレベル1を指定します")));
	idxTessellationLevelFactorOuter1 = index;
	index++;
	pTessellationLevelFactors->AddSubItem(new CMFCPropertyGridProperty(_T("gl_TessLevelOuter[2]"), (_variant_t)((int)2), _T("テセレーションレベル2を指定します")));
	idxTessellationLevelFactorOuter2 = index;
	index++;
	pTessellationLevelFactors->AddSubItem(new CMFCPropertyGridProperty(_T("gl_TessLevelOuter[3]"), (_variant_t)((int)5), _T("テセレーションレベル3を指定します")));
	idxTessellationLevelFactorOuter3 = index;
	index++;
	pTessellationLevelFactors->AddSubItem(new CMFCPropertyGridProperty(_T("gl_TessLevelInner[0]"), (_variant_t)((int)3), _T("テセレーションレベル0を指定します")));
	idxTessellationLevelFactorInner0 = index;
	index++;
	pTessellationLevelFactors->AddSubItem(new CMFCPropertyGridProperty(_T("gl_TessLevelInner[1]"), (_variant_t)((int)4), _T("テセレーションレベル1を指定します")));
	idxTessellationLevelFactorInner1 = index;
	index++;


}


int CActorPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndActorPropList.Create(dwViewStyle, rectDummy, this, 1)) {
		TRACE0("Actorプロパティ グリッドを作成できませんでした\n");
		return -1;
	}

	InitActorPropList();

	AdjustLayout();
	return 0;
}


void CActorPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}


void CActorPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}


void CActorPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndActorPropList.SetFocus();
}

void CActorPropertiesWnd::AdjustLayout()
{
	// TODO: ここに実装コードを追加します.
	if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic())) {
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	CRect actorPropListClientRect;
	m_wndActorPropList.CalcWindowRect(actorPropListClientRect);
	int actorPropListHeight = 1000;

	m_wndActorPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), actorPropListHeight, SWP_NOACTIVATE | SWP_NOZORDER);


}


void CActorPropertiesWnd::updateActorProperty(CActorProperty actorProperty) {

	CActorProperty viewportProp;

	CMFCPropertyGridProperty* pActorProperty = (CMFCPropertyGridProperty*)m_wndActorPropList.GetProperty(idxActor);

	// Obtain property for translation.
	CMFCPropertyGridProperty* pActorTranslation = (CMFCPropertyGridProperty*)pActorProperty->GetSubItem(idxTranslation);

	// Update propeties for actor translation.
	CMFCPropertyGridProperty* pActorTranslationX = (CMFCPropertyGridProperty*)pActorTranslation->GetSubItem(idxTranslationX);
	pActorTranslationX->SetValue(actorProperty.GetActorTranslationX());

	CMFCPropertyGridProperty* pActorTranslationY = (CMFCPropertyGridProperty*)pActorTranslation->GetSubItem(idxTranslationY);
	pActorTranslationY->SetValue(actorProperty.GetActorTranslationY());

	CMFCPropertyGridProperty* pActorTranslationZ = (CMFCPropertyGridProperty*)pActorTranslation->GetSubItem(idxTranslationZ);
	pActorTranslationZ->SetValue(actorProperty.GetActorTranslationZ());

	// Obtain property for rotation.
	CMFCPropertyGridProperty* pActorRotation = (CMFCPropertyGridProperty*)pActorProperty->GetSubItem(idxRotation);

	// Update property for rotation.
	CMFCPropertyGridProperty* pActorRotationX = (CMFCPropertyGridProperty*)pActorRotation->GetSubItem(idxRotationX);
	pActorRotationX->SetValue(actorProperty.GetActorRotationX());

	CMFCPropertyGridProperty* pActorRotationY = (CMFCPropertyGridProperty*)pActorRotation->GetSubItem(idxRotationY);
	pActorRotationY->SetValue(actorProperty.GetActorRotationY());

	CMFCPropertyGridProperty* pActorRotationZ = (CMFCPropertyGridProperty*)pActorRotation->GetSubItem(idxRotationZ);
	pActorRotationZ->SetValue(actorProperty.GetActorRotationZ());

	// Obtain property for scale.
	CMFCPropertyGridProperty* pActorScale = (CMFCPropertyGridProperty*)pActorProperty->GetSubItem(idxScale);

	// Update property for scale.
	CMFCPropertyGridProperty* pActorScaleK = (CMFCPropertyGridProperty*)pActorScale->GetSubItem(idxScale_k);
	pActorScaleK->SetValue(actorProperty.GetActorScaleK());

	// Tessellation
	CMFCPropertyGridProperty* pActorTessellation = (CMFCPropertyGridProperty*)pActorProperty->GetSubItem(idxTessellation);
	int subItemCnt = pActorTessellation->GetSubItemsCount();

	// TessellationFunc
	CMFCPropertyGridProperty* pActorTessellationFunc = (CMFCPropertyGridProperty*)pActorTessellation->GetSubItem(idxTessellationFunc);

	// Tessellation On/Off
	CMFCPropertyGridProperty* pActorTessellationFuncOnOff = (CMFCPropertyGridProperty*)pActorTessellationFunc->GetSubItem(idxTessellationFuncOnOff);
	pActorTessellationFuncOnOff->SetValue(_variant_t(actorProperty.GetTessellationOnOff()));

	// TessellationDomain
	CMFCPropertyGridProperty* pActorTessellationDomain = (CMFCPropertyGridProperty*)pActorTessellation->GetSubItem(idxTessellationDomain);

	// TessellationDomainQuadTriaLine
	CMFCPropertyGridProperty* pActorTessellationDomainQuadTriaLine = (CMFCPropertyGridProperty*)pActorTessellationDomain->GetSubItem(idxTessellationDomainQuadTriaLine);
	pActorTessellationDomainQuadTriaLine->SetValue(COleVariant(m_tessellationDomainHelper.getPropertyName(actorProperty.GetTessellationDomain()).c_str()));

	// TessellationLevelFactor
	CMFCPropertyGridProperty* pActorTessellationLevelFactor = (CMFCPropertyGridProperty*)pActorTessellation->GetSubItem(idxTessellationLevelFactor);

	// Tessellation level factor inner0
	CMFCPropertyGridProperty* pActorTessellationLevelFactorInner0 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactor->GetSubItem(idxTessellationLevelFactorInner0);
	pActorTessellationLevelFactorInner0->SetOriginalValue(_variant_t((int)actorProperty.GetTessellationLevelFactorInner0()));

	// Tessellation level factor inner1
	CMFCPropertyGridProperty* pActorTessellationLevelFactorInner1 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactor->GetSubItem(idxTessellationLevelFactorInner1);
	pActorTessellationLevelFactorInner1->SetOriginalValue(_variant_t((int)actorProperty.GetTessellationLevelFactorInner1()));

	// Tessellation level factor outer0
	CMFCPropertyGridProperty* pActorTessellationLevelFactorOuter0 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactor->GetSubItem(idxTessellationLevelFactorOuter0);
	pActorTessellationLevelFactorOuter0->SetOriginalValue(_variant_t((int)actorProperty.GetTessellationLevelFactorOuter0()));

	// Tessellation level factor outer1
	CMFCPropertyGridProperty* pActorTessellationLevelFactorOuter1 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactor->GetSubItem(idxTessellationLevelFactorOuter1);
	pActorTessellationLevelFactorOuter1->SetOriginalValue(_variant_t((int)actorProperty.GetTessellationLevelFactorOuter1()));

	// Tessellation level factor outer2
	CMFCPropertyGridProperty* pActorTessellationLevelFactorOuter2 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactor->GetSubItem(idxTessellationLevelFactorOuter2);
	pActorTessellationLevelFactorOuter2->SetOriginalValue(_variant_t{ actorProperty.GetTessellationLevelFactorOuter2() });

	// Tessellation level factor outer3
	CMFCPropertyGridProperty* pActorTessellationLevelFactorOuter3 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactor->GetSubItem(idxTessellationLevelFactorOuter3);
	pActorTessellationLevelFactorOuter3->SetOriginalValue(_variant_t((int)actorProperty.GetTessellationLevelFactorOuter3()));

}


void CActorPropertiesWnd::DoDataExchange(CDataExchange* pDX)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CDockablePane::DoDataExchange(pDX);
}


void CActorPropertiesWnd::getActorProperty(CActorProperty& actorProperty) {

	CActorProperty viewportProp;

	CMFCPropertyGridProperty* pActorProperty = (CMFCPropertyGridProperty*)m_wndActorPropList.GetProperty(idxActor);

	// Obtain property for translation.
	CMFCPropertyGridProperty* pActorTranslation = (CMFCPropertyGridProperty*)pActorProperty->GetSubItem(idxTranslation);

	// Update propeties for actor translation.
	CMFCPropertyGridProperty* pActorTranslationX = (CMFCPropertyGridProperty*)pActorTranslation->GetSubItem(idxTranslationX);
	float transX = pActorTranslationX->GetValue().fltVal;

	CMFCPropertyGridProperty* pActorTranslationY = (CMFCPropertyGridProperty*)pActorTranslation->GetSubItem(idxTranslationY);
	float transY = pActorTranslationY->GetValue().fltVal;

	CMFCPropertyGridProperty* pActorTranslationZ = (CMFCPropertyGridProperty*)pActorTranslation->GetSubItem(idxTranslationZ);
	float transZ = pActorTranslationZ->GetValue().fltVal;

	// Obtain property for rotation.
	CMFCPropertyGridProperty* pActorRotation = (CMFCPropertyGridProperty*)pActorProperty->GetSubItem(idxRotation);

	// Update property for rotation.
	CMFCPropertyGridProperty* pActorRotationX = (CMFCPropertyGridProperty*)pActorRotation->GetSubItem(idxRotationX);
	float rotateX = pActorRotationX->GetValue().fltVal;

	CMFCPropertyGridProperty* pActorRotationY = (CMFCPropertyGridProperty*)pActorRotation->GetSubItem(idxRotationY);
	float rotateY = pActorRotationY->GetValue().fltVal;

	CMFCPropertyGridProperty* pActorRotationZ = (CMFCPropertyGridProperty*)pActorRotation->GetSubItem(idxRotationZ);
	float rotateZ = pActorRotationZ->GetValue().fltVal;

	// Obtain property for scale.
	CMFCPropertyGridProperty* pActorScale = (CMFCPropertyGridProperty*)pActorProperty->GetSubItem(idxScale);

	// Update property for scale.
	CMFCPropertyGridProperty* pActorScaleK = (CMFCPropertyGridProperty*)pActorScale->GetSubItem(idxScale_k);
	float scale_k = pActorScaleK->GetValue().fltVal;

	actorProperty.SetActorTranslationX(transX);
	actorProperty.SetActorTranslationY(transY);
	actorProperty.SetActorTranslationZ(transZ);

	actorProperty.SetActorRotationX(rotateX);
	actorProperty.SetActorRotationY(rotateY);
	actorProperty.SetActorRotationZ(rotateZ);

	actorProperty.SetActorScaleK(scale_k);


	CMFCPropertyGridProperty* pActorTessellation = (CMFCPropertyGridProperty*)pActorProperty->GetSubItem(idxTessellation);

	CMFCPropertyGridProperty* pActorTessellationFunc = (CMFCPropertyGridProperty*)pActorTessellation->GetSubItem(idxTessellationFunc);

	CMFCPropertyGridProperty* pActorTessellationOnOff = (CMFCPropertyGridProperty*)pActorTessellationFunc->GetSubItem(idxTessellationFuncOnOff);
	bool tessellationOnOff = pActorTessellationOnOff->GetValue().boolVal;

	CMFCPropertyGridProperty* pActorTessellationDomain = (CMFCPropertyGridProperty*)pActorTessellation->GetSubItem(idxTessellationDomain);

	CMFCPropertyGridProperty* pActorTessellationDomainQuadTriaLine = (CMFCPropertyGridProperty*)pActorTessellationDomain->GetSubItem(idxTessellationDomainQuadTriaLine);
	int tessellationDomain = m_tessellationDomainHelper.getIndex(pActorTessellationDomainQuadTriaLine->GetValue().bstrVal);

	CMFCPropertyGridProperty* pActorTessellationLevelFactors = (CMFCPropertyGridProperty*)pActorTessellation->GetSubItem(idxTessellationLevelFactor);

	CMFCPropertyGridProperty* pActorTessellationLevelFactorOuter0 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactors->GetSubItem(idxTessellationLevelFactorOuter0);
	int tessellationFactorOuter0 = pActorTessellationLevelFactorOuter0->GetValue().intVal;
	CMFCPropertyGridProperty* pActorTessellationLevelFactorOuter1 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactors->GetSubItem(idxTessellationLevelFactorOuter1);
	int tessellationFactorOuter1 = pActorTessellationLevelFactorOuter1->GetValue().intVal;
	CMFCPropertyGridProperty* pActorTessellationLevelFactorOuter2 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactors->GetSubItem(idxTessellationLevelFactorOuter2);
	int tessellationFactorOuter2 = pActorTessellationLevelFactorOuter2->GetValue().intVal;
	CMFCPropertyGridProperty* pActorTessellationLevelFactorOuter3 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactors->GetSubItem(idxTessellationLevelFactorOuter3);
	int tessellationFactorOuter3 = pActorTessellationLevelFactorOuter3->GetValue().intVal;
	CMFCPropertyGridProperty* pActorTessellationLevelFactorInner0 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactors->GetSubItem(idxTessellationLevelFactorInner0);
	int tessellationFactorInner0 = pActorTessellationLevelFactorInner0->GetValue().intVal;
	CMFCPropertyGridProperty* pActorTessellationLevelFactorInner1 = (CMFCPropertyGridProperty*)pActorTessellationLevelFactors->GetSubItem(idxTessellationLevelFactorInner1);
	int tessellationFactorInner1 = pActorTessellationLevelFactorInner1->GetValue().intVal;



	actorProperty.SetTessellationOnOff(tessellationOnOff);
	actorProperty.SetTessellationDomain(tessellationDomain);
	actorProperty.SetTessellationLevelFactorOuter0(tessellationFactorOuter0);
	actorProperty.SetTessellationLevelFactorOuter1(tessellationFactorOuter1);
	actorProperty.SetTessellationLevelFactorOuter2(tessellationFactorOuter2);
	actorProperty.SetTessellationLevelFactorOuter3(tessellationFactorOuter3);
	actorProperty.SetTessellationLevelFactorInner0(tessellationFactorInner0);
	actorProperty.SetTessellationLevelFactorInner1(tessellationFactorInner1);
}
