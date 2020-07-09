
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
#include "WavefrontObjFile.h"

#include "CConverter.h"
#include "logGLError.h"
#include "CDeviceContext.h"

#include "mymath.h"
//#include "Winuser.h"

using namespace mymath;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CMy3DObjViewerView, CView)

BEGIN_MESSAGE_MAP(CMy3DObjViewerView, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy3DObjViewerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_WM_NCMOUSEHOVER()
	ON_WM_NCMOUSELEAVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


CMy3DObjViewerView::CMy3DObjViewerView() noexcept
	: m_eyeSpace { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, 
	m_moveHoveringAlongXYPlaneFlg{false},
	m_moveHoveringAlongXZPlaneFlg{ false },
	m_moveRotateAlongXYnYZPlaneFlg{ false },
	virtualKey{false}
{
	// TODO: 構築コードをここに追加します。


	

}

CMy3DObjViewerView::~CMy3DObjViewerView()
{
}

BOOL CMy3DObjViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}


void CMy3DObjViewerView::OnDraw(CDC* pDC /*pDC*/)
{
	CMy3DObjViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
	std::unique_ptr<CDeviceContext> pDevCon{ new CDeviceContext() };
	pDevCon->SetGLContext(pDC, m_hrc);

	wglMakeCurrent(pDevCon->GetDC()->m_hDC, pDevCon->GetHGLRC() );

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); logGlError();

	glEnable(GL_DEPTH_TEST); logGlError();
	glDepthFunc(GL_LEQUAL); logGlError();

	glDepthMask(GL_TRUE); logGlError();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); logGlError();

	float cyan[] = { 165,220,228,0 };
	glClearBufferfv(GL_COLOR, 0, cyan); logGlError();

	//
	// Perspective matrix
	//
	Matrixf4x4 projectionMatrix = m_eyeSpace.getOnlyProjectionMatrix(pDoc, pDevCon.get());
	Matrixf4x4 translationScalongMatrix = m_eyeSpace.getOnlyTranslationRotationScalingMatrix(pDoc, pDevCon.get());

	//
	// Start render.
	//

	m_glGrid2.Render(*pDevCon, projectionMatrix, translationScalongMatrix, m_eyeSpace);

	for (CActor* pActor : m_actors ) {
		pActor->Render2(*pDevCon, projectionMatrix, translationScalongMatrix);
	}

	bool ret = SwapBuffers(pDC->m_hDC);
	ASSERT(ret == true);

	wglMakeCurrent(NULL, NULL);
}




void CMy3DObjViewerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy3DObjViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CMy3DObjViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CMy3DObjViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CMy3DObjViewerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	return;
}

void CMy3DObjViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}



#ifdef _DEBUG
void CMy3DObjViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CMy3DObjViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy3DObjViewerDoc* CMy3DObjViewerView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy3DObjViewerDoc)));
	return (CMy3DObjViewerDoc*)m_pDocument;
}
#endif //_DEBUG




int CMy3DObjViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1) {
		MessageBox(_T("int CMy3DObjViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct)"), _T("Debug Message"), MB_OK);

		return -1;
	}

	// TODO: ここに特定な作成コードを追加してください。

	CMy3DObjViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return TRUE;

	CDC* pDC = GetDC();

	CreateGLContext(pDC);

	std::unique_ptr<CDeviceContext> pDevCon{ new CDeviceContext() };
	pDevCon->SetGLContext(pDC, m_hrc);

	InitializeGL3W(*pDevCon);

	// Initialize shaders.
	m_glGrid.InitializeShaders(*pDevCon);
	m_glGrid.PrepareScene(*pDevCon);

	m_glGrid2.InitializeShaders(*pDevCon);
	m_glGrid2.PrepareScene(*pDevCon);

	m_debugDrawer.InitializeShaders(*pDevCon);
	m_debugDrawer.prepareDrawing2(*pDevCon);

	ReleaseDC(pDC);

	// Register drop target.
	m_dropTarget.Register(this);


	// Retrieve my engine property.
	CMyEnginePropertiesWnd* pMyEnginePropertiesWnd = pDoc->getMyEnginePropertiesWnd();

	m_eyeSpace.setMyEnginePropertiesWnd(pMyEnginePropertiesWnd);

	return 0;
}


void CMy3DObjViewerView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if (KillTimer(my_timer_id) == 0) {
		TRACE("KillTimer() failed");
	}

	CDC* pDC = GetDC();

	std::unique_ptr<CDeviceContext> pDevCon{ new CDeviceContext() };
	pDevCon->SetGLContext(pDC, m_hrc);

	m_glGrid.DestroyScene(*pDevCon);
	m_glGrid2.DestroyScene(*pDevCon);
	ReleaseDC(pDC);
}


BOOL CMy3DObjViewerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	return TRUE;
	//	return CView::OnEraseBkgnd(pDC);
}


void CMy3DObjViewerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_nType = nType;
	m_cx = cx;
	m_cy = cy;
	m_aspect = float(cy) / float(cx);


	CDC* pDC = GetDC();

	std::unique_ptr<CDeviceContext> pDevCon{ new CDeviceContext() };
	pDevCon->SetGLContext(pDC, m_hrc);

	m_glGrid.Reshape(*pDevCon, cx, cy);
	m_glGrid2.Reshape(*pDevCon, cx, cy);


	for (CActor* pActor : m_actors) {

		pActor->Reshape(*pDevCon, cx, cy);

	}
	   

	ReleaseDC(pDC);

}


void CMy3DObjViewerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (SetTimer(my_timer_id, 100, NULL) == 0) {
		TRACE("SetTimer() failed.");
	}

	CMy3DObjViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CViewportProperty viewportProperty = pDoc->getViewportProperty();
	float posX = viewportProperty.GetViewportTranslationX();
	float posY = viewportProperty.GetViewportTranslationY();
	float posZ = viewportProperty.GetViewportTranslationZ();
	m_eyeSpace.SetPosX(posX);
	m_eyeSpace.SetPosY(posY);
	m_eyeSpace.SetPosZ(posZ);

	float rotateX = viewportProperty.GetViewportRotationX();
	float rotateY = viewportProperty.GetViewportRotationY();
	float rotateZ = viewportProperty.GetViewportRotationZ();
	m_eyeSpace.SetRotateX(rotateX);
	m_eyeSpace.SetRotateY(rotateY);
	m_eyeSpace.SetRotateZ(rotateZ);

}




void CMy3DObjViewerView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	Invalidate();
	RedrawWindow();
	CView::OnTimer(nIDEvent);
}

void CMy3DObjViewerView::AddActor( WavefrontObjFile* pObjFile ) {

	CMy3DObjViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Device context setup.
	CDC* pDC = GetDC();
	std::unique_ptr<CDeviceContext> pDevCon{ new CDeviceContext() };
	pDevCon->SetGLContext(pDC, m_hrc);

	// Retrieve actor property.
	CActorPropertiesWnd* pActorPropertiesWnd = pDoc->getActorPropertiesWnd();

	// Calculate location for new actor.
	Vectorf4 Pe{ 0.0f, 0.0f, 70.0f, 1.0f };

	Matrixf4x4 negateZAxisRot;
	negateZAxisRot.identityNegateZAxis();

	Matrixf4x4 combMat = 
		  negateZAxisRot
		* m_eyeSpace.GetRotateMatrix4x4Rev() 
		* m_eyeSpace.GetScaleMatrix4x4() 
		* m_eyeSpace.GetTranslateMatrix4x4_2();

	Vectorf4 Pw = (Pe * combMat).homogeniousDivide();

	// Create and add an actor from WavefrontObjecjtFile.
	CActor* pActor = CConverter::convertWavefrontObjFileToActor2(*pObjFile);
	pActor->InitializeShaders(*pDevCon);
	pActor->prepareDrawing2(*pDevCon);
	pActor->Reshape(*pDevCon, m_cx, m_cy);

	pActor->SetPosX(Pw[0]);
	pActor->SetPosY(Pw[1]);
	pActor->SetPosZ(Pw[2]);
	pActor->SetScale(1.0f);
	pActor->SetRotateX(0);
	pActor->SetRotateY(0);
	pActor->SetRotateZ(0);

	pActor->SetTessellationOnOff(false);
	pActor->SetTessellationDomain(2);
	pActor->SetTessellationLevelFactorInner0(1);
	pActor->SetTessellationLevelFactorInner1(2);
	pActor->SetTessellationLevelFactorOuter0(3);
	pActor->SetTessellationLevelFactorOuter1(4);
	pActor->SetTessellationLevelFactorOuter2(5);
	pActor->SetTessellationLevelFactorOuter3(6);

	pActor->setActorPropertiesWnd(pActorPropertiesWnd);
	AddActor(pActor);

	// Add the actor to world outliner.
	pDoc->addActorID(pActor);

	// Create and add an actor to the list of actorAABBs make it selectable by user mouse control.
	CActorAABB* pAabb = pActor->createActorAABB();
	m_ActorSelector.AddActor(pAabb);

	// Create and add an AABB from actor's AABB.
	CAABB* pAABB = pActor->createAABB(*pAabb);
	pAABB->InitializeShaders(*pDevCon);
	pAABB->prepareDrawing2(*pDevCon);
	AddAABB(pAABB);

	m_ActorSelector.SelectActor(*pAabb, pDoc);

	ReleaseDC(pDC);
}

BOOL CMy3DObjViewerView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	// TODO: ここに実装コードを追加します.

	UINT cfFormat = RegisterClipboardFormatW(CLIPBOARDFORMAT);

	HGLOBAL hGlobal = pDataObject->GetGlobalData( cfFormat );

	// ObjFileが格納されているアドレスを取得する。
	WavefrontObjFile** ppObjFile = static_cast<WavefrontObjFile**>(::GlobalLock(hGlobal));
	WavefrontObjFile* pObjFile = *ppObjFile;
	::GlobalUnlock(hGlobal);

	AddActor( pObjFile );

	return TRUE;
}

void CMy3DObjViewerView::SelectActor(int actorId, CMy3DObjViewerDoc* pDoc) {

	m_ActorSelector.SelectActor(actorId, pDoc);

}

void CMy3DObjViewerView::DeleteActor(int actorId, CMy3DObjViewerDoc* pDoc) {

	//
	// delete the actor from m_aabbs.
	//

	vector<CAABB*>::iterator itorAABB = m_aabbs.begin();

	while (itorAABB != m_aabbs.end()) {

		if ((*itorAABB)->getActorId() == actorId) {
			m_aabbs.erase(itorAABB);
			break;
		}

		itorAABB++;
	}

	//
	// delete the actor from m_ActorSelector.
	//

	m_ActorSelector.DeleteActor(actorId );

	//
	// delete the actor from m_actors.
	//

	vector<CActor*>::iterator itorActor = m_actors.begin();

	while (itorActor != m_actors.end()) {

		if ((*itorActor)->getActorId() == actorId) {
			m_actors.erase(itorActor);
			break;
		}

		itorActor++;
	}



}


DROPEFFECT CMy3DObjViewerView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: ここに実装コードを追加します.
	return DROPEFFECT_COPY;
}


DROPEFFECT CMy3DObjViewerView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: ここに実装コードを追加します.
	return DROPEFFECT_COPY;
}


void CMy3DObjViewerView::OnDragLeave()
{
	// TODO: ここに実装コードを追加します.
	return;
}


// add an actor to render group.
void CMy3DObjViewerView::AddActor(CActor* pActor)
{
	// TODO: ここに実装コードを追加します.
	m_actors.push_back(pActor);
}

// add an AABB to render group.
void CMy3DObjViewerView::AddAABB(CAABB* pAABB)
{
	// TODO: ここに実装コードを追加します.
	m_aabbs.push_back(pAABB);
}

bool CMy3DObjViewerView::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

int CMy3DObjViewerView::InitializeGL3W(CDeviceContext& devCon)
{
	// TODO: ここに実装コードを追加します.
	wglMakeCurrent(devCon.GetDC()->m_hDC, devCon.GetHGLRC());

	// initialize gl3w
	if (gl3wInit()) {

		GLenum ret = glGetError();

		fprintf(stderr, "failed to initialize OpenGL\n");

		return FALSE;
	}

	if (!gl3wIsSupported(4, 4)) {

		fprintf(stderr, "OpenGL 4.4 not supported\n");

		return FALSE;
	}

	wglMakeCurrent(NULL, NULL);

	return 0;
}



void CMy3DObjViewerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	CMy3DObjViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC* pDC = GetDC();

	std::unique_ptr<CDeviceContext> pDevCon{ new CDeviceContext() };
	pDevCon->SetGLContext(pDC, m_hrc);

	CViewportProperty viewportTranslation = pDoc->getViewportProperty();

	RECT windowRect;
	GetWindowRect(&windowRect);

	bool recordRay = false;
	if (virtualKey && VK_SHIFT) {
		recordRay = true;
	}

	m_ActorSelector.Select(
		point.x, point.y,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		viewportTranslation.GetPerspectiveFrustumLeft(),
		viewportTranslation.GetPerspectiveFrustumRight(),
		viewportTranslation.GetPerspectiveFrustumBottom(),
		viewportTranslation.GetPerspectiveFrustumTop(),
		viewportTranslation.GetPerspectiveFrustumNear(),
		*pDevCon,
		viewportTranslation,
		m_debugDrawer,
		pDoc,
		m_eyeSpace,
		recordRay
	);

	ReleaseDC(pDC);

	CView::OnLButtonUp(nFlags, point);
}

void CMy3DObjViewerView::getStartTrackMouseEvent( TRACKMOUSEEVENT& event ) {

	memset(&event, 0, sizeof(event));
	event.cbSize = sizeof(event);
	event.dwFlags = TME_HOVER | TME_LEAVE;
	event.hwndTrack = m_hWnd;
	event.dwHoverTime = 10;

}

void CMy3DObjViewerView::getCancelTrackMouseEvent(TRACKMOUSEEVENT& event) {

	getStartTrackMouseEvent(event);
	event.dwFlags |= TME_CANCEL;

}



void CMy3DObjViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	CMy3DObjViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CViewportProperty viewportTranslation = pDoc->getViewportProperty();

	float left = viewportTranslation.GetPerspectiveFrustumLeft();
	float right = viewportTranslation.GetPerspectiveFrustumRight();
	float top = viewportTranslation.GetPerspectiveFrustumTop();
	float bottom = viewportTranslation.GetPerspectiveFrustumBottom();
	float n = viewportTranslation.GetPerspectiveFrustumNear();
	float f = viewportTranslation.GetPerspectiveFrustumFar();

	RECT windowRect;
	GetWindowRect(&windowRect);
	float screenWidth = windowRect.right - windowRect.left;
	float screenHeight = windowRect.bottom - windowRect.top;

	float eyeSpaceSpeed = 0.3f;
	float eyeSpecaAngleSpeed = 0.05f;

	if (m_moveHoveringAlongXYPlaneFlg == true) {

		// 
		// Move the eye space.
		//

		float moveX = + ( m_prevMousePoint.x - point.x );
		float moveY = - ( m_prevMousePoint.y - point.y );
		Vectorf3 move{ moveX * eyeSpaceSpeed, moveY * eyeSpaceSpeed, 0.0f };

		moveEyeSpaceRevNegZ(move);
	}

	if (m_moveHoveringAlongXZPlaneFlg == true) {

		// 
		// Move the eye space.
		//

		float moveX = - (m_prevMousePoint.x - point.x);
		float moveZ = + (m_prevMousePoint.y - point.y);
		Vectorf3 move{ 0.0f, 0.0f, moveZ * eyeSpaceSpeed };

		moveEyeSpaceRevNegZ(move);

		m_eyeSpace.AddRotate(
			0.0f, 
			moveX * eyeSpecaAngleSpeed,
			0.0f);
	}

	if (m_moveRotateAlongXYnYZPlaneFlg == true) {


		// 
		// Move the eye space.
		//

		float moveX = -(m_prevMousePoint.x - point.x);
		float moveZ = -(m_prevMousePoint.y - point.y);

		m_eyeSpace.AddRotate( 
			moveZ * eyeSpecaAngleSpeed, 
			moveX * eyeSpecaAngleSpeed, 
			0.0f );

	}


	TRACKMOUSEEVENT event;

	if(m_moveHoveringAlongXYPlaneFlg == false &&
		m_moveHoveringAlongXZPlaneFlg == false &&
		m_moveRotateAlongXYnYZPlaneFlg == false ){

		if (nFlags & MK_LBUTTON && nFlags & MK_RBUTTON) {

			getStartTrackMouseEvent( event );

			BOOL ret;
			ret = TrackMouseEvent(&event);
			ASSERT(ret != 0);

			// Mouse hovoring start.
			m_moveHoveringAlongXYPlaneFlg = true;

			// Record mouse position.
			m_prevMousePoint = point;
		}
		else if (nFlags & MK_LBUTTON ) {

			getStartTrackMouseEvent(event);

			BOOL ret;
			ret = TrackMouseEvent(&event);
			ASSERT(ret != 0);

			// Mouse hovoring start.
			m_moveHoveringAlongXZPlaneFlg = true;

			// Record mouse position.
			m_prevMousePoint = point;

		}
		else if (nFlags & MK_RBUTTON) {

			getStartTrackMouseEvent(event);

			BOOL ret;
			ret = TrackMouseEvent(&event);
			ASSERT(ret != 0);

			// Mouse hovoring start.
			m_moveRotateAlongXYnYZPlaneFlg = true;

			// Record mouse position.
			m_prevMousePoint = point;

		}
	}


	CView::OnMouseMove(nFlags, point);
}


void CMy3DObjViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CView::OnLButtonDown(nFlags, point);
}


void CMy3DObjViewerView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CView::OnRButtonDown(nFlags, point);
}


void CMy3DObjViewerView::OnMouseLeave()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	
	// Mouse hovering end.
	m_moveHoveringAlongXYPlaneFlg = false;
	m_moveHoveringAlongXZPlaneFlg = false;
	m_moveRotateAlongXYnYZPlaneFlg = false;

	CMy3DObjViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//
	// Update viewport properties.
	//
	CViewportProperty viewportProperty = pDoc->getViewportProperty();
	viewportProperty.SetViewportTranslationX(m_eyeSpace.GetPosX());
	viewportProperty.SetViewportTranslationY(m_eyeSpace.GetPosY());
	viewportProperty.SetViewportTranslationZ(m_eyeSpace.GetPosZ());

	viewportProperty.SetViewportRotationX(m_eyeSpace.GetRotateX());
	viewportProperty.SetViewportRotationY(m_eyeSpace.GetRotateY());
	viewportProperty.SetViewportRotationZ(m_eyeSpace.GetRotateZ());

	pDoc->updateViewportProperty(viewportProperty);

	CView::OnMouseLeave();
}


void CMy3DObjViewerView::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CMy3DObjViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	TRACKMOUSEEVENT event;

	if (nFlags & ( MK_LBUTTON | MK_RBUTTON ) ) {

		//
		// Continue mouse tracking.
		//
		getStartTrackMouseEvent(event);

		BOOL ret;
		ret = TrackMouseEvent(&event);
		ASSERT(ret != 0);

		// Record mouse position.
		m_prevMousePoint = point;

	}else{

		// Mouse hovering end.
		m_moveHoveringAlongXYPlaneFlg = false;
		m_moveHoveringAlongXZPlaneFlg = false;
		m_moveRotateAlongXYnYZPlaneFlg = false;

		CMy3DObjViewerDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		//
		// Update viewport properties.
		//
		CViewportProperty viewportProperty = pDoc->getViewportProperty();
		viewportProperty.SetViewportTranslationX(m_eyeSpace.GetPosX());
		viewportProperty.SetViewportTranslationY(m_eyeSpace.GetPosY());
		viewportProperty.SetViewportTranslationZ(m_eyeSpace.GetPosZ());

		viewportProperty.SetViewportRotationX(m_eyeSpace.GetRotateX());
		viewportProperty.SetViewportRotationY(m_eyeSpace.GetRotateY());
		viewportProperty.SetViewportRotationZ(m_eyeSpace.GetRotateZ());

		pDoc->updateViewportProperty(viewportProperty);

	}

	CView::OnMouseHover(nFlags, point);
}


void CMy3DObjViewerView::OnNcMouseHover(UINT nFlags, CPoint point)
{
	// この機能は Windows 2000 かそれ以降のバージョンを必要とします。
	// シンボル _WIN32_WINNTと WINVER は >= 0x0500 にならなければなりません。
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CView::OnNcMouseHover(nFlags, point);
}


void CMy3DObjViewerView::OnNcMouseLeave()
{
	// この機能は Windows 2000 かそれ以降のバージョンを必要とします。
	// シンボル _WIN32_WINNTと WINVER は >= 0x0500 にならなければなりません。
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CView::OnNcMouseLeave();
}


void CMy3DObjViewerView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
}


void CMy3DObjViewerView::moveEyeSpaceRev( Vectorf3& move )
{
	float moveXInW;
	float moveYInW;
	float moveZInW;

	m_eyeSpace.GetDeltaComponentInW(move, m_eyeSpace.GetRotateMatrix3x3Rev(), &moveXInW, &moveYInW, &moveZInW);
	m_eyeSpace.AddPos(moveXInW, moveYInW, moveZInW);
}

void CMy3DObjViewerView::moveEyeSpaceRevNegZ(Vectorf3& move)
{
	float moveXInW;
	float moveYInW;
	float moveZInW;

	Matrixf4x4 rotMatBase = m_eyeSpace.GetRotateMatrix3x3Rev();

	Matrixf4x4 negZMat;
	negZMat.identityNegateZAxis();

	Matrixf4x4 rotMat = negZMat * rotMatBase;

	m_eyeSpace.GetDeltaComponentInW(move, rotMat.homogeniousDivide(), &moveXInW, &moveYInW, &moveZInW);
	m_eyeSpace.AddPos(moveXInW, moveYInW, moveZInW);
}


void CMy3DObjViewerView::moveEyeSpace(Vectorf3& move)
{
	float moveXInW;
	float moveYInW;
	float moveZInW;

	m_eyeSpace.GetDeltaComponentInW(move, m_eyeSpace.GetRotateMatrix3x3(), &moveXInW, &moveYInW, &moveZInW);
	m_eyeSpace.AddPos(moveXInW, moveYInW, moveZInW);
}


void CMy3DObjViewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// Record pressed key information.
	virtualKey = nChar;

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMy3DObjViewerView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// Clear pressed key information.
	virtualKey = 0;

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

