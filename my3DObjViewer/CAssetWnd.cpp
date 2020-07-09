#include "pch.h"

#include "framework.h"
#include "CAssetWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#include <memory>
#include <stdlib.h>
#include <string>

#include "obj_reader/obj_reader.h"
#include "WavefrontObjFile.h"

#include "CImporterWavefrontDialog.h"

using namespace std;



// extern "C" struct obj_reader_work* readObj(const char* objFileName);



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAssetWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
//	ON_MESSAGE(LVN_BEGINDRAG, &CAssetWnd::OnLvnBegindrag)
END_MESSAGE_MAP()


CAssetWnd::CAssetWnd() noexcept
{

}

CAssetWnd::~CAssetWnd()
{

}

BEGIN_MESSAGE_MAP(CAssetList, CListBox)
	ON_WM_CONTEXTMENU()
//	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_COMMAND(ID_LOAD_ASSET_OBJ, OnLoadAssetObj)
	ON_WM_WINDOWPOSCHANGING()

//	ON_MESSAGE(LVN_BEGINDRAG, &CAssetList::OnLvnBegindrag)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
END_MESSAGE_MAP()

CAssetList::CAssetList() noexcept {

}

CAssetList::~CAssetList() {

}

void CAssetWnd::FillBuildWindow() {


}

void CAssetWnd::UpdateFonts()
{
	m_wndAssetBuild.SetFont(&afxGlobalData.fontRegular);
}

int CAssetWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1 )) {
		TRACE0("タブ付きウィンドウを作成できませんでした\n");
		return -1;
	}

	//出力ペインの作成
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndAssetBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2)) {

		TRACE0("出力ウィンドウを作成できませんでした\n");
		return -1;

	}

	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	bNameValid = strTabName.LoadString(IDS_ASSET_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndAssetBuild, strTabName, (UINT)0);

	// 出力タブにダミー テキストを入力します
	FillBuildWindow();

	return 0;
}


void CAssetWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_wndTabs.SetWindowPos(nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

}


void CAssetList::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。

	if (0 == m_assetLoadMenu.LoadMenu(IDR_ASSET_LOAD_MENU)) {
		return;
	}

	ASSERT(m_assetLoadMenu);

	POINT mousePos;
	GetCursorPos(&mousePos);

	CMenu* pMenu = m_assetLoadMenu.GetSubMenu(0);

	pMenu->TrackPopupMenuEx(
		TPM_LEFTALIGN,
		mousePos.x,
		mousePos.y,
		this,
		NULL);

	m_assetLoadMenu.DestroyMenu();

}

void CAssetList::AddNewObjFile( class WavefrontObjFile* pObjFile, const CString& fileName ) {

	// Add new objFile to map.
	TSequenceNumber seqNum = sequenceNumberGenerator.GetNextNumber();
	m_objFiles[seqNum] = pObjFile;
	numOfAsset++;

	// Add new item to List on window.
	int index = AddString(fileName);
	SetItemData(index, seqNum);

}

void CAssetList::OnLoadAssetObj()
{
	CFileDialog dlgFile(TRUE);
	OPENFILENAME& ofn = dlgFile.GetOFN();
	ofn.lpstrFilter = _T("Wavefront Object file\0*.obj\0\0");

	if (dlgFile.DoModal() == IDCANCEL) {
		// Did not make a choice.
		return;
	}

	// Convert string from generic string type to char*.
	CString fileName = dlgFile.GetPathName();

	int sizeOfString = (fileName.GetLength() + 1);
	unique_ptr<TCHAR[]> lpsz{ new TCHAR[sizeOfString] };
	_tcscpy_s(lpsz.get(), sizeOfString, fileName);

	USES_CONVERSION;
	char* pFilename = T2A((LPTSTR)lpsz.get());

	// Read the *.obj file chosen.
	struct obj_reader_work* pObjReaderWork = readObj(pFilename);

	// Convert internal representation to class.
	class WavefrontObjFile* pObjFile = WavefrontObjFile::create(pObjReaderWork);

	//
	// open import option dialog.
	//
	CImporterWavefrontDialog dialog;
	if (dialog.DoModal() == IDOK) {

		//
		// expecting below values.
		//
		// 0: (blank)
		// 1: +X
		// 2: +Y
		// 3: +Z
		// 4: -X
		// 5: -Y
		// 6: -Z

		WavefrontObjFile::Axis fromForward = static_cast<WavefrontObjFile::Axis>(dialog.m_forward - 1);
		WavefrontObjFile::Axis fromUp = static_cast<WavefrontObjFile::Axis>(dialog.m_up - 1);
		WavefrontObjFile::Axis toForward = WavefrontObjFile::Axis::X;
		WavefrontObjFile::Axis toUp = WavefrontObjFile::Axis::X;

		pObjFile->convertAxis(fromForward, fromUp, toForward, toUp);








	}

	AddNewObjFile(pObjFile, fileName );



	return;

}

void CAssetList::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// Make sure something is selected.
	int index = GetCurSel();
	if (index != LB_ERR) {

		COleDataSource*  pOleDataSource = CacheSelectedAsset( index );

		DROPEFFECT ret = pOleDataSource->DoDragDrop();
		if (ret == DROPEFFECT_MOVE) {

			// ドラッグが行われた。



		}

		pOleDataSource->InternalRelease();

	}

	CListBox::OnLButtonDown(nFlags, point);

	// これがないと画面がマウスになかなか反応しなくなる。
	PostMessage(WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));

}


int CAssetList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。

	return 0;
}


COleDataSource* CAssetList::CacheSelectedAsset(int inSelectedItemIndex)
{
	// TODO: ここに実装コードを追加します.

	// キャッシュする対象の選択状態のObjFileを取得する。
	TSequenceNumber seqNum = GetItemData(inSelectedItemIndex);
	WavefrontObjFile* pObjFile = m_objFiles[seqNum];

	// ドラッグするためヒープにObjFileのアドレスを格納する準備をする。

	COleDataSource* pOleDataSource = new COleDataSource();

	if (pOleDataSource != nullptr) {

		HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(pObjFile));
		ASSERT( hGlobal != NULL );

		// ObjFileが格納されているアドレスを格納する。
		WavefrontObjFile** ppObjFile = static_cast<WavefrontObjFile**>(::GlobalLock(hGlobal));
		*ppObjFile = pObjFile;
		::GlobalUnlock(hGlobal);

		UINT cfFormat = RegisterClipboardFormatW(CLIPBOARDFORMAT);

		pOleDataSource->CacheGlobalData( cfFormat, hGlobal);

		return pOleDataSource;
	}


	return nullptr;
}


