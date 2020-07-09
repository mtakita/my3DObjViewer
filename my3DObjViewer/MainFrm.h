
// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once

#ifndef _MAINFRM_H_
#define _MAINFRM_H_

#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "CAssetWnd.h"
#include "CMyEnginePropertiesWnd.h"
#include "CActorPropertiesWnd.h"
#include "CWorldOutlinerWnd.h"

#include "mymath.h"

using mymath::Vectorf4;


class CMainFrame : public CFrameWndEx
{
	
protected: // シリアル化からのみ作成します。
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバー
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
/*
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
*/
//	COutputWnd        m_wndOutput;
//	CPropertiesWnd    m_wndProperties;
	CAssetWnd        m_wndAsset;
	CWorldOutlinerWnd m_wndWorldOutliner;

	CMyEnginePropertiesWnd m_wndMyEngineProperties;
	CActorPropertiesWnd m_wndActorProperties;

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	CViewportProperty getViewpotTranslation();
	void showActorProperty();
	void hideActorProperty();
	void updateActorProperty(CActorProperty actorProperty);
	void updateViewportProperty(CViewportProperty viewportProperty);
	void addActorID(CActor* pActor);
	BOOL selectItem(HTREEITEM inItem);

	/*
	bool getNeedPropertyUpdateFlag();
	void clearNeedPropertyUpdateFlag();
*/
	CActorPropertiesWnd* getActorPropertiesWnd();
	CMyEnginePropertiesWnd* getMyEnginePropertiesWnd();

};


#endif
