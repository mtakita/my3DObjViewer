#pragma once
#include <afxdockablepane.h>
#include "ViewTree.h"
#include "IncreasingNumber.h"
#include <map>
#include "CActor.h"
#include "etc.h"
#include "CMyTreeView.h"

using std::map;

class CWorldOutlinerToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};


class CWorldOutlinerWnd :
	public CDockablePane
{
	using TSequenceNumber = DWORD;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	
	void OnChangeVisualStyle();

	void FillClassView();
	void AdjustLayout();

	CWorldOutlinerToolBar m_wndToolBar;
	CMyTreeView m_wndWorldOutlinerView;
	CImageList m_WorldOutlinerViewImages;
	UINT m_nCurrSort;

private:
	IncreasingNumber sequenceNumberGenerator;
	int numOfActors = 0;

	map<TSequenceNumber, int> m_actorIDs;

public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	void AddActorID(class CActor* pActor);
	BOOL selectItem(HTREEITEM inItem);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
	afx_msg void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnWorldoutlinerDelete();
	virtual HRESULT accSelect(long flagsSelect, VARIANT varChild);
};

