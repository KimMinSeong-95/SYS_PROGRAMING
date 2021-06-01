
// MultiTeskView.cpp: CMultiTeskView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MultiTesk.h"
#endif

#include "MultiTeskDoc.h"
#include "MultiTeskView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiTeskView

IMPLEMENT_DYNCREATE(CMultiTeskView, CView)

BEGIN_MESSAGE_MAP(CMultiTeskView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMultiTeskView 생성/소멸

CMultiTeskView::CMultiTeskView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMultiTeskView::~CMultiTeskView()
{
}

BOOL CMultiTeskView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMultiTeskView 그리기

int nBlue;
void CMultiTeskView::OnDraw(CDC* pDC)
{
	CMultiTeskDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

}


// CMultiTeskView 인쇄

BOOL CMultiTeskView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMultiTeskView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMultiTeskView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMultiTeskView 진단

#ifdef _DEBUG
void CMultiTeskView::AssertValid() const
{
	CView::AssertValid();
}

void CMultiTeskView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMultiTeskDoc* CMultiTeskView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMultiTeskDoc)));
	return (CMultiTeskDoc*)m_pDocument;
}
#endif //_DEBUG


// CMultiTeskView 메시지 처리기
CMultiTeskView* g_pView;

DWORD WINAPI ThreadDraw(LPVOID p)
{
	RECT rc = *(LPRECT)p;
	CDC* pDC = g_pView->GetDC();

	while (1)
	{
		nBlue += 5;
		if (nBlue > 255) nBlue = 0;

		CBrush BrBlue(RGB(0, 0, nBlue));
		CBrush* pOldBr = pDC->SelectObject(&BrBlue);
		//pDC->Rectangle(10, 10, 400, 200);
		pDC->Rectangle(rc.left, rc.top, rc.right, rc.bottom);
		pDC->SelectObject(pOldBr);
	}
	g_pView->ReleaseDC(pDC);
	return 0;
}

void CMultiTeskView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	g_pView = this;

	static RECT arRect[] = { {100, 100, 200, 200},
							 {300, 100, 400, 200},
							 {100, 300, 200, 400},
							 {300, 300, 400, 400} };

	DWORD dwID;
	for (int i = 0; i < 4; i++)
	{
		CloseHandle(CreateThread(NULL, 0, ThreadDraw, &arRect[i], 0, &dwID));
	}

	CView::OnLButtonDown(nFlags, point);
}
