#include "pch.h"
#include "CMyTreeView.h"
#include "CWorldOutlinerWnd.h"
#include "MainFrm.h"
#include "CMy3DObjViewerDoc.h"
#include "CMy3DObjViewerView.h"

BEGIN_MESSAGE_MAP(CMyTreeView, CViewTree)
	ON_NOTIFY_REFLECT(TVN_ITEMCHANGING, &CMyTreeView::OnTvnItemChanging)
	ON_NOTIFY_REFLECT(TVN_ITEMCHANGED, &CMyTreeView::OnTvnItemChanged)
END_MESSAGE_MAP()


void CMyTreeView::OnTvnItemChanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVITEMCHANGE* pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
}


void CMyTreeView::OnTvnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVITEMCHANGE* pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;

	if (pNMTVItemChange->uStateNew & TVIS_SELECTED) {

		//	HTREEITEM hTreeItem = GetSelectedItem();
		HTREEITEM hTreeItem = pNMTVItemChange->hItem;

		int actorID = GetItemData(hTreeItem);

		CWorldOutlinerWnd* pWnd = dynamic_cast<CWorldOutlinerWnd*>(GetParent());
		ASSERT(pWnd != nullptr);

		CMainFrame* frame = (CMainFrame*)pWnd->GetParentFrame();
		CMy3DObjViewerDoc* pDoc = static_cast<CMy3DObjViewerDoc*>(frame->GetActiveDocument());

		POSITION pos = pDoc->GetFirstViewPosition();
		CMy3DObjViewerView* p = (CMy3DObjViewerView*)pDoc->GetNextView(pos);
		p->SelectActor(actorID, pDoc);

		// Signal that the document is updated.
//		pDoc->SetModifiedFlag();

	}
}

BOOL CMyTreeView::selectItem(HTREEITEM inItem) {

	return CViewTree::SelectItem(inItem);

}
