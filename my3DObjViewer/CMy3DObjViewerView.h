
//

#pragma once

#include "GLRenderer.h"
#include "COleDropTargetEx.h"

#include "vector"
#include "CActor.h"
#include "CGLGrid.h"
#include "CGLGrid2.h"
#include "CActorSelector.h"
#include "CAABB.h"

#include "mymath.h"

#include "CDebugDrawer.h"
#include "CEyeSpace.h"
#include "WavefrontObjFile.h"
#include "etc.h"

using namespace mymath;
using std::vector;

class CMy3DObjViewerView : public CView
{
protected: // シリアル化からのみ作成します。
	CMy3DObjViewerView() noexcept;
	DECLARE_DYNCREATE(CMy3DObjViewerView)

// 属性
public:
	CMy3DObjViewerDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void SelectActor(int actorId, CMy3DObjViewerDoc* pDoc);
	void DeleteActor(int actorId, CMy3DObjViewerDoc* pDoc);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	bool CreateGLContext(CDC* pDC);

	void AddActor(CActor* pActor);
	void AddAABB(CAABB* pAABB);
	void AddActor(WavefrontObjFile* pObjFile);


	Matrixf4x4 constructProjectionMatrix(CMy3DObjViewerDoc* pDoc, CDeviceContext* pDevCon);

protected:
//	GLRenderer m_glRenderer;
	COleDropTargetEx m_dropTarget;

	CGLGrid m_glGrid;
	CGLGrid2 m_glGrid2;

	vector<CActor*> m_actors;
	vector<CAABB*> m_aabbs;

	CDebugDrawer m_debugDrawer;
	CActorSelector m_ActorSelector;

	HGLRC m_hrc = 0;

	int m_nType = 0;
	int m_cx = 0;
	int m_cy = 0;
	float m_aspect = 0;

	CEyeSpace m_eyeSpace;

	CPoint m_prevMousePoint;
	bool m_moveHoveringAlongXYPlaneFlg;
	bool m_moveHoveringAlongXZPlaneFlg;
	bool m_moveRotateAlongXYnYZPlaneFlg;

	int virtualKey;

	// 実装
public:
	virtual ~CMy3DObjViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

protected:

// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	
	//	int InitializeGL3W(CDC* pDC );
	int InitializeGL3W(CDeviceContext& devCon);
	int InitializeShaders(CDeviceContext& devCon);
	void attachShaders(GLuint programId, ShaderInfo shaders[], int size);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();

	enum timerId { my_timer_id = 1 };

protected:
	void formatErrorMessage();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnNcMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnNcMouseLeave();

private:
	void getStartTrackMouseEvent(TRACKMOUSEEVENT& event);
	void getCancelTrackMouseEvent(TRACKMOUSEEVENT& event);

	void moveEyeSpace(Vectorf3& move);
	void moveEyeSpaceRev(Vectorf3& move);
	void moveEyeSpaceRevNegZ(Vectorf3& move);

	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  
inline CMy3DObjViewerDoc* CMy3DObjViewerView::GetDocument() const
   { return reinterpret_cast<CMy3DObjViewerDoc*>(m_pDocument); }
#endif

