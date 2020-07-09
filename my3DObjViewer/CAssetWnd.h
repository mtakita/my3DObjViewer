#pragma once
#include <afxdockablepane.h>
#include "IncreasingNumber.h"
#include <map>

using namespace std;

class CAssetList :
	public CListBox
{
	using TSequenceNumber = DWORD;

public:
	CAssetList() noexcept;
protected:
	void AddNewObjFile( class WavefrontObjFile* pObjFile, const CString& fileName);

public:
	virtual ~CAssetList();

protected:
	CMenu m_assetLoadMenu;

private:
	IncreasingNumber sequenceNumberGenerator;
	int numOfAsset = 0;

	map<TSequenceNumber, WavefrontObjFile*> m_objFiles;

public:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLoadAssetObj();
	afx_msg void OnTest();
	//	afx_msg void OnViewOutput();
protected:
	afx_msg LRESULT OnLvnBegindrag(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	COleDataSource* CacheSelectedAsset(int inSelectedItemIndex);
};



class CAssetWnd :
	public CDockablePane
{
public:
	CAssetWnd() noexcept;

public:
	virtual ~CAssetWnd();

protected:
	CMFCTabCtrl m_wndTabs;
	CAssetList m_wndAssetBuild;

	void UpdateFonts();

protected:
	void FillBuildWindow();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	afx_msg LRESULT OnLvnBegindrag(WPARAM wParam, LPARAM lParam);
};

