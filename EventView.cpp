
// EventView.cpp: CEventView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Event.h"
#endif

#include "EventDoc.h"
#include "EventView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEventView

IMPLEMENT_DYNCREATE(CEventView, CView)

BEGIN_MESSAGE_MAP(CEventView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CEventView 생성/소멸
HANDLE hEvent30;
HANDLE hCalc;
CEventView::CEventView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	hEvent30 = CreateEvent(NULL, FALSE, FALSE, NULL); // 자동리셋 이벤트
	hCalc = CreateEvent(NULL, TRUE, FALSE, NULL); // 수동
}

CEventView::~CEventView()
{
	CloseHandle(hEvent30);
}

BOOL CEventView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CEventView 그리기

void CEventView::OnDraw(CDC* /*pDC*/)
{
	CEventDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CEventView 인쇄

BOOL CEventView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CEventView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CEventView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CEventView 진단

#ifdef _DEBUG
void CEventView::AssertValid() const
{
	CView::AssertValid();
}

void CEventView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEventDoc* CEventView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEventDoc)));
	return (CEventDoc*)m_pDocument;
}
#endif //_DEBUG


// CEventView 메시지 처리기

int buf[100];
DWORD WINAPI ThreadFunc(LPVOID p)
{
	for (int i = 0; i < 100; i++)
	{
		Sleep(30);
		buf[i] = i;
		if (i == 30) SetEvent(hEvent30);
	}
	MessageBeep(0);
	AfxMessageBox(_T("100만원 모았따 받아라"));
	return 0;
}

void CEventView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	DWORD dwID;
	HANDLE hTh = CreateThread(NULL, 0, ThreadFunc, this, 0, &dwID);

	/////////////////////////////////////////////////////
	//기다려
	//WaitForSingleObject(hTh, INFINITE);
	WaitForSingleObject(hEvent30, INFINITE);
	////////////////////////////////////////
	CDC* pDC = GetDC();
	CString str;
	for (int i = 0; i <= 30; i++)
	{
		str.Format(_T("%d 라인은 %d입니다."), i, buf[i]);
		pDC->TextOut(10, i * 20, str);
	}
	ReleaseDC(pDC);
	CloseHandle(hTh);

	CView::OnLButtonDown(nFlags, point);
}

DWORD WINAPI ThreadCalc(LPVOID p)
{
	CEventView* pView = (CEventView*)p;
	CDC* pDC = pView->GetDC();

	for (int i = 0; i < 10; i++)
	{
		pDC->TextOut(500, 100, _T("계산중"));
		GdiFlush();
		Sleep(300);
		pDC->TextOut(500, 100, _T("기다려"));
		GdiFlush();
		Sleep(300);
	}
	MessageBeep(0);
	pDC->TextOut(500, 100, _T("계산완료"));
	pView->ReleaseDC(pDC);

	SetEvent(hCalc);
	return 0;
}
DWORD WINAPI ThreadSend(LPVOID p)
{
	CEventView* pView = (CEventView*)p;
	CDC* pDC = pView->GetDC();

	WaitForSingleObject(hCalc, INFINITE);
	pDC->TextOut(500, 200, _T("Send : 전송완료"));
	pView->ReleaseDC(pDC);

	return 0;
}

DWORD WINAPI ThreadSave(LPVOID p)
{
	CEventView* pView = (CEventView*)p;
	CDC* pDC = pView->GetDC();

	WaitForSingleObject(hCalc, INFINITE);
	pDC->TextOut(500, 300, _T("Save : 전송완료"));
	pView->ReleaseDC(pDC);
	
	return 0;
}

DWORD WINAPI ThreadPrint(LPVOID p)
{
	CEventView* pView = (CEventView*)p;
	CDC* pDC = pView->GetDC();

	WaitForSingleObject(hCalc, INFINITE);
	pDC->TextOut(500, 400, _T("Print : 전송완료"));
	pView->ReleaseDC(pDC);
	
	return 0;
}

void CEventView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	DWORD dwID;
	ResetEvent(hCalc);

	CloseHandle(CreateThread(NULL, 0, ThreadCalc, this, 0, &dwID));
	CloseHandle(CreateThread(NULL, 0, ThreadSend, this, 0, &dwID));
	CloseHandle(CreateThread(NULL, 0, ThreadSave, this, 0, &dwID));
	CloseHandle(CreateThread(NULL, 0, ThreadPrint, this, 0, &dwID));
	

	CView::OnRButtonDown(nFlags, point);
}
