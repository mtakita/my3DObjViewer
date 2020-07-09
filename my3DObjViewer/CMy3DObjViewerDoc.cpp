
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "CMy3DObjViewerApp.h"
#endif

#include "CMy3DObjViewerDoc.h"
#include "CMy3DObjViewerView.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CMy3DObjViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CMy3DObjViewerDoc, CDocument)
END_MESSAGE_MAP()



CMy3DObjViewerDoc::CMy3DObjViewerDoc() noexcept
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CMy3DObjViewerDoc::~CMy3DObjViewerDoc()
{
}

BOOL CMy3DObjViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。

	return TRUE;
}





void CMy3DObjViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		// TODO: 読み込むコードをここに追加してください。
	}
}

#ifdef SHARED_HANDLERS

//縮小版のサポート
void CMy3DObjViewerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// このコードを変更してドキュメントのデータを描画します
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 検索ハンドラーのサポート
void CMy3DObjViewerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ドキュメントのデータから検索コンテンツを設定します。
	// コンテンツの各部分は ";" で区切る必要があります

	// 例:      strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMy3DObjViewerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS


#ifdef _DEBUG
void CMy3DObjViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMy3DObjViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG




CViewportProperty CMy3DObjViewerDoc::getViewportProperty()
{
	// TODO: ここに実装コードを追加します.
	POSITION pos = GetFirstViewPosition();
	CMy3DObjViewerView* p = (CMy3DObjViewerView*)this->GetNextView(pos);
	CMainFrame* frame = (CMainFrame*)p->GetParentFrame();
	CViewportProperty viewportTranslation = frame->getViewpotTranslation();
	return viewportTranslation;
}

void CMy3DObjViewerDoc::showActorProperty() {
	POSITION pos = GetFirstViewPosition();
	CMy3DObjViewerView* p = (CMy3DObjViewerView*)this->GetNextView(pos);
	CMainFrame* frame = (CMainFrame*)p->GetParentFrame();
	frame->showActorProperty();
}

void CMy3DObjViewerDoc::hideActorProperty() {
	POSITION pos = GetFirstViewPosition();
	CMy3DObjViewerView* p = (CMy3DObjViewerView*)this->GetNextView(pos);
	CMainFrame* frame = (CMainFrame*)p->GetParentFrame();
	frame->hideActorProperty();
}

BOOL CMy3DObjViewerDoc::selectItem(HTREEITEM inItem) {
	POSITION pos = GetFirstViewPosition();
	CMy3DObjViewerView* p = (CMy3DObjViewerView*)this->GetNextView(pos);
	CMainFrame* frame = (CMainFrame*)p->GetParentFrame();
	return frame->selectItem(inItem);
}
void CMy3DObjViewerDoc::updateActorProperty(CActorProperty actorProperty) {

	POSITION pos = GetFirstViewPosition();
	CMy3DObjViewerView* p = (CMy3DObjViewerView*)this->GetNextView(pos);
	CMainFrame* frame = (CMainFrame*)p->GetParentFrame();
	frame->updateActorProperty(actorProperty);
}

CActorPropertiesWnd* CMy3DObjViewerDoc::getActorPropertiesWnd() {
	POSITION pos = GetFirstViewPosition();
	CMy3DObjViewerView* p = (CMy3DObjViewerView*)this->GetNextView(pos);
	CMainFrame* frame = (CMainFrame*)p->GetParentFrame();
	return frame->getActorPropertiesWnd();
}

void CMy3DObjViewerDoc::updateViewportProperty(CViewportProperty viewportProperty) {

	POSITION pos = GetFirstViewPosition();
	CMy3DObjViewerView* p = (CMy3DObjViewerView*)this->GetNextView(pos);
	CMainFrame* frame = (CMainFrame*)p->GetParentFrame();
	frame->updateViewportProperty(viewportProperty);
}

CMyEnginePropertiesWnd* CMy3DObjViewerDoc::getMyEnginePropertiesWnd() {
	POSITION pos = GetFirstViewPosition();
	CMy3DObjViewerView* p = (CMy3DObjViewerView*)this->GetNextView(pos);
	CMainFrame* frame = (CMainFrame*)p->GetParentFrame();
	return frame->getMyEnginePropertiesWnd();
}

void CMy3DObjViewerDoc::addActorID(CActor* pActor) {
	POSITION pos = GetFirstViewPosition();
	CMy3DObjViewerView* p = (CMy3DObjViewerView*)this->GetNextView(pos);
	CMainFrame* frame = (CMainFrame*)p->GetParentFrame();
	return frame->addActorID( pActor );

}
