#include "pch.h"
#include "framework.h"
#include "Resource.h"

#include "CWorldOutlinerWnd.h"
#include "CMy3DObjViewerApp.h"
#include "MainFrm.h"
#include "CMy3DObjViewerDoc.h"
#include "CMy3DObjViewerView.h"


class CWorldOutlinerViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CWorldOutlinerView;

	DECLARE_SERIAL(CWorldOutlinerViewMenuButton)

public:
	CWorldOutlinerViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

BEGIN_MESSAGE_MAP(CWorldOutlinerWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_DELETEITEM()
	ON_WM_SYSCHAR()
	ON_COMMAND(ID_WORLDOUTLINER_DELETE, &CWorldOutlinerWnd::OnWorldoutlinerDelete)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(CWorldOutlinerViewMenuButton, CMFCToolBarMenuButton, 1)


int CWorldOutlinerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �r���[�̍쐬:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndWorldOutlinerView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("�N���X �r���[���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	// �C���[�W�̓ǂݍ���:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* ���b�N����Ă��܂�*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// ���ׂẴR�}���h���A�e�t���[���o�R�ł͂Ȃ����̃R���g���[���o�R�œn����܂�:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CWorldOutlinerViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CWorldOutlinerViewMenuButton* pButton = DYNAMIC_DOWNCAST(CWorldOutlinerViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// �ÓI�c���[ �r���[ �f�[�^ (�_�~�[ �R�[�h) ����͂��܂�
	FillClassView();

	return 0;
}

void CWorldOutlinerWnd::OnChangeVisualStyle()
{
	m_WorldOutlinerViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�r�b�g�}�b�v��ǂݍ��߂܂���ł���: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_WorldOutlinerViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_WorldOutlinerViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndWorldOutlinerView.SetImageList(&m_WorldOutlinerViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* ���b�N����܂���*/);
}

void CWorldOutlinerWnd::FillClassView()
{
}



void CWorldOutlinerWnd::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndWorldOutlinerView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// �N���b�N���ꂽ���ڂ̑I��:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_WORLDOUTLINER_EDIT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

	if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
		return;

	((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
	UpdateDialogControls(this, FALSE);

}


void CWorldOutlinerWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	AdjustLayout();
}

void CWorldOutlinerWnd::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndWorldOutlinerView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}



BOOL CWorldOutlinerWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	return CDockablePane::PreTranslateMessage(pMsg);
}


void CWorldOutlinerWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
					   // �`�惁�b�Z�[�W�� CDockablePane::OnPaint() ���Ăяo���Ȃ��ł��������B

	CRect rectTree;
	m_wndWorldOutlinerView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));

}


void CWorldOutlinerWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B

	m_wndWorldOutlinerView.SetFocus();
}

BOOL CWorldOutlinerWnd::selectItem(HTREEITEM inItem) {

	return m_wndWorldOutlinerView.selectItem(inItem);
}

void CWorldOutlinerWnd::AddActorID( class CActor* pActor ) {

	// Add an entery to the map.
	TSequenceNumber seqNum = sequenceNumberGenerator.GetNextNumber();
	m_actorIDs[seqNum] = pActor->getActorId();
	numOfActors++;

	// Add item to List on window.

	CString objectName { pActor->getObjectName().c_str() };

	HTREEITEM hRoot = m_wndWorldOutlinerView.InsertItem(objectName, 0, 0);
	m_wndWorldOutlinerView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_wndWorldOutlinerView.SetItemData(hRoot, seqNum);

	// Select the item.
//	m_wndWorldOutlinerView.Select(hRoot, TVGN_CARET);
	m_wndWorldOutlinerView.SelectItem(hRoot);


}


void CWorldOutlinerWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndWorldOutlinerView;
	ASSERT_VALID(pWndTree);


	if (point != CPoint(-1, -1))
	{
		// �N���b�N���ꂽ���ڂ̑I��:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);
			int actorID = pWndTree->GetItemData(hTreeItem);

			CMainFrame* frame = (CMainFrame*)GetParentFrame();
			CMy3DObjViewerDoc* pDoc = static_cast<CMy3DObjViewerDoc*>(frame->GetActiveDocument());

			POSITION pos = pDoc->GetFirstViewPosition();
			CMy3DObjViewerView* p = (CMy3DObjViewerView*)pDoc->GetNextView(pos);
			p->SelectActor(actorID, pDoc );

			// Signal that the document is updated.
			pDoc->SetModifiedFlag();
		}
	}

	pWndTree->SetFocus();



	CDockablePane::OnLButtonUp(nFlags, point);
}


void CWorldOutlinerWnd::OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CDockablePane::OnDeleteItem(nIDCtl, lpDeleteItemStruct);
}


void CWorldOutlinerWnd::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CDockablePane::OnSysChar(nChar, nRepCnt, nFlags);
}


void CWorldOutlinerWnd::OnWorldoutlinerDelete()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B

	//
	// Delete the actor from world outliner.
	//

	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndWorldOutlinerView;
	ASSERT_VALID(pWndTree);

	HTREEITEM hTreeItem = pWndTree->GetSelectedItem();
	if (hTreeItem == NULL) {
		return;
	}

	int actorId = pWndTree->GetItemData(hTreeItem);

	pWndTree->DeleteItem(hTreeItem);

	//
	// Delete the actor from view
	//

	CMainFrame* frame = (CMainFrame*)GetParentFrame();
	CMy3DObjViewerDoc* pDoc = static_cast<CMy3DObjViewerDoc*>(frame->GetActiveDocument());

	POSITION pos = pDoc->GetFirstViewPosition();
	CMy3DObjViewerView* p = (CMy3DObjViewerView*)pDoc->GetNextView(pos);
	p->DeleteActor(actorId, pDoc);




}


HRESULT CWorldOutlinerWnd::accSelect(long flagsSelect, VARIANT varChild)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	return CDockablePane::accSelect(flagsSelect, varChild);
}
