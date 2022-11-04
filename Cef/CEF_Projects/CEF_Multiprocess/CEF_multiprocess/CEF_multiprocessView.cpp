
// CEF_multiprocessView.cpp: CCEFmultiprocessView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "CEF_multiprocess.h"
#endif

#include "CEF_multiprocessDoc.h"
#include "CEF_multiprocessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCEFmultiprocessView

IMPLEMENT_DYNCREATE(CCEFmultiprocessView, CView)

BEGIN_MESSAGE_MAP(CCEFmultiprocessView, CView)
END_MESSAGE_MAP()

// CCEFmultiprocessView 생성/소멸

CCEFmultiprocessView::CCEFmultiprocessView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CCEFmultiprocessView::~CCEFmultiprocessView()
{
}

BOOL CCEFmultiprocessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CCEFmultiprocessView 그리기

void CCEFmultiprocessView::OnDraw(CDC* /*pDC*/)
{
	CCEFmultiprocessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CCEFmultiprocessView 진단

#ifdef _DEBUG
void CCEFmultiprocessView::AssertValid() const
{
	CView::AssertValid();
}

void CCEFmultiprocessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCEFmultiprocessDoc* CCEFmultiprocessView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCEFmultiprocessDoc)));
	return (CCEFmultiprocessDoc*)m_pDocument;
}
#endif //_DEBUG


// CCEFmultiprocessView 메시지 처리기
