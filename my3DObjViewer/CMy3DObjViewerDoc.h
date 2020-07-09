


#pragma once

#ifndef _CMY3DOBJVIEWERDOC_H_
#define _CMY3DOBJVIEWERDOC_H_

#include "mymath.h"
#include "CViewportProperty.h"
#include "CActorPropertiesWnd.h"
#include "CMyEnginePropertiesWnd.h"
#include "CActor.h"

using mymath::Vectorf4;


class CMy3DObjViewerDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CMy3DObjViewerDoc() noexcept;
	DECLARE_DYNCREATE(CMy3DObjViewerDoc)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 実装
public:
	virtual ~CMy3DObjViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 検索ハンドラーの検索コンテンツを設定するヘルパー関数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	CViewportProperty getViewportProperty();
	void updateViewportProperty(CViewportProperty viewportProperty );
	void updateActorProperty(CActorProperty actorProperty);
	void showActorProperty();
	void hideActorProperty();
	CActorPropertiesWnd* getActorPropertiesWnd();
	CMyEnginePropertiesWnd* getMyEnginePropertiesWnd();
	void addActorID(CActor* pActor);
	BOOL selectItem(HTREEITEM inItem);


};

#endif
