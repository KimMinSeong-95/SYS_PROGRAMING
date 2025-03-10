﻿
// MyDevClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MyDevClient.h"
#include "MyDevClientDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyDevClientDlg 대화 상자



CMyDevClientDlg::CMyDevClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYDEVCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDevClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyDevClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CMyDevClientDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_SUSPEND, &CMyDevClientDlg::OnBnClickedButtonSuspend)
	ON_BN_CLICKED(IDC_BUTTON_END, &CMyDevClientDlg::OnBnClickedButtonEnd)
END_MESSAGE_MAP()


// CMyDevClientDlg 메시지 처리기

BOOL CMyDevClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMyDevClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMyDevClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMyDevClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#include "MyDevLib.h"
//#pragma comment(lib, "MyDevLib.lib")

typedef struct USERDATA
{
	int x;
	int y;
} USERDATA;

HANDLE		g_hMonitor;
USERDATA	g_arData[100];
int			g_nCount = 0;

DWORD WINAPI CalcThread(LPVOID p)
{
	USERDATA* arWork = (USERDATA*)p;
	CString strTemp;
	//1. 100개 자료 가공(*2)
	for (int i = 0; i < 100; i++)
	{
		arWork[i].x = arWork[i].x * 2;
		arWork[i].y = arWork[i].y * 2;
	}
	
	//2. 서버접속 시도
	HANDLE hPipe;
	for (;;)// 서버 무한 접속 시도
	{
		hPipe = CreateFile(_T("\\\\.\\pipe\\MyPipe"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		if (hPipe != INVALID_HANDLE_VALUE) break;
	}

	//3. 전송(단방향, 1회성)
	DWORD dwWritten;
	TCHAR szOutput[255];
	for (int i = 0; i < 100; i++)
	{
		strTemp.Format(_T("Client[%d] 보고: %d, %d\r\n"), ::GetCurrentProcessId, arWork[i].x, arWork[i].y);
		WriteFile(hPipe, strTemp, (lstrlen(strTemp) + 1) * sizeof(TCHAR), &dwWritten, NULL);
	}

	CloseHandle(hPipe);
	free(arWork);
	return 0;
}

DWORD WINAPI MonitorThread(LPVOID p)
{
	int x, y;
	CString strTemp;
	
	for (;;)
	{
		if (::GetMyDevPosition(&x, &y))
		{
			strTemp.Format(_T("x=%d, y=%d\n"), x, y);
			g_arData[g_nCount].x = x;
			g_arData[g_nCount].y = y;
		}
		else
		{
			strTemp.Format(_T("에러발생 : %d\n"), GetMyDevError());
		}
		::OutputDebugString(strTemp);
		Sleep(100);

		/////////////////////////////////////////////////////////////
		g_nCount++;
		if (g_nCount == 100)
		{
			int* arCopy = (int*)malloc(sizeof(g_arData));
			memcpy(arCopy, g_arData, sizeof(g_arData));

			CloseHandle(CreateThread(NULL, 0, CalcThread, arCopy, 0, 0));
			g_nCount = 0;
		}
	}
	return 0;
}


void CMyDevClientDlg::OnBnClickedButtonStart()
{
	InitMyDev(5);	// 장치 초기화

	g_hMonitor = CreateThread(NULL, 0, MonitorThread, 0, 0, 0);
}


void CMyDevClientDlg::OnBnClickedButtonSuspend()
{
	static BOOL bSuspend = FALSE;

	if (bSuspend != TRUE)
	{
		bSuspend = TRUE;
		SuspendThread(g_hMonitor);
		SetDlgItemText(IDC_BUTTON_SUSPEND, _T("정지"));
	}
	else
	{
		bSuspend = FALSE;
		ResumeThread(g_hMonitor);
		SetDlgItemText(IDC_BUTTON_SUSPEND, _T("시작"));
	}
}


void CMyDevClientDlg::OnBnClickedButtonEnd()
{
	ReleaseMyDev();

	TerminateThread(g_hMonitor, 0);
	CloseHandle(g_hMonitor);
}
