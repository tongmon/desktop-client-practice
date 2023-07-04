﻿
// WebViewProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "WebViewProject.h"
#include "WebViewProjectDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <unordered_map>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWebViewProjectDlg 대화 상자



CWebViewProjectDlg::CWebViewProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WEBVIEWPROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebViewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWebViewProjectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_WEBVIEW_BUTTON, OnWebViewButtonClicked)
END_MESSAGE_MAP()


// CWebViewProjectDlg 메시지 처리기

BOOL CWebViewProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CWebViewProjectDlg::OnPaint()
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
HCURSOR CWebViewProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// WebView Dialog에 넘길 파라메터 구조체
// WebViewDLL의 구조체와 동일해야 한다.
struct WebViewParam
{
	bool is_base64_encoded;
	bool should_get_msg_when_closed;
	bool is_direct_close;
	LPCTSTR url;
	LPCTSTR title;
	HWND hwnd;
	SIZE size;
	std::vector<std::wstring> element_ids;
	std::unordered_map<std::wstring, std::wstring> html_result;
};

void CWebViewProjectDlg::OnWebViewButtonClicked()
{
	using WebViewFunc = void(*)(WebViewParam*);

	HINSTANCE h_instance = LoadLibrary(_T("WebViewDLL.dll"));
	if (!h_instance)
		return;

	WebViewFunc webview_func = reinterpret_cast<WebViewFunc>(GetProcAddress(h_instance, "RunWebView"));

	if (webview_func)
	{
		WebViewParam wvp
		{
			false,
			false,
			false,
			_T("https://www.google.com/"),
			_T("google"),
			this->m_hWnd,
			{900, 700},
			{},
			{} 
		};

		webview_func(&wvp);
	}

	FreeLibrary(h_instance);
}

