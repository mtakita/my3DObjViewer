#pragma once
#include "ViewTree.h"
class CMyTreeView :
	public CViewTree
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTvnItemChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);

public:
	BOOL selectItem(HTREEITEM inItem);

};

