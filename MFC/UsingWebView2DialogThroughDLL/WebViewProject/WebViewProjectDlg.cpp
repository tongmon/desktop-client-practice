
// WebViewProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "WebViewProject.h"
#include "WebViewProjectDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <unordered_map>
#include <array>
#include <functional>
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
	enum CallBackType
	{
		OnDialogClose,
		OnCloseMessageReceived,
		OnNavigationCompleteMessageReceived,
		OnNavigationStartMessageReceived,
		OnMessageReceived,
		CallBackCnt
	};

	LPCTSTR url = nullptr;
	LPCTSTR title = nullptr;
	HWND hwnd = nullptr;
	SIZE size = { 500,500 };
	std::array<std::function<void(LPCWSTR)>, CallBackCnt> callbacks;
};

void CWebViewProjectDlg::OnWebViewButtonClicked()
{
	using RunWebViewDlg = void(*)(WebViewParam*);

	using ExecuteScript = bool(*)(
#if defined(UNICODE) || defined(_UNICODE)
		const std::wstring& script,
#else
		const std::string& script,
#endif
		HRESULT(*)(HRESULT, const WCHAR*));

	HINSTANCE h_instance = LoadLibrary(_T("WebViewDLL.dll"));
	if (!h_instance)
		return;

	// 웹 뷰 다이얼로그 수행하는 함수
	RunWebViewDlg fn_runwebview = reinterpret_cast<RunWebViewDlg>(GetProcAddress(h_instance, "RunWebViewDialog"));

	// 스크립트 수행 함수
	ExecuteScript fn_executescript = reinterpret_cast<ExecuteScript>(GetProcAddress(h_instance, "ExecuteScript"));

	if (!fn_runwebview || !fn_executescript)
	{
		FreeLibrary(h_instance);
		return;
	}

	WebViewParam wvp
	{
		_T("https://www.google.com/"),
		_T("google"),
		m_hWnd,
		{900, 700}
	};

	wvp.callbacks[WebViewParam::OnNavigationCompleteMessageReceived] = [&](LPCWSTR str)->void
	{
		// url 탐색이 완료되면 해당 함수가 실행되면서 html을 읽는 스크립트를 실행하게 만듦
		fn_executescript(LR"(document.documentElement.innerHTML)",
						 [](HRESULT err, const WCHAR* str)->HRESULT
						 {
							 // str에 html 정보가 담기게 됨.
							 return S_OK;
						 });

		/*
		// 밑과 같이 특정 태그에 대한 정보를 가져올 수도 있음.
		ExecuteScript fn_executescript = reinterpret_cast<ExecuteScript>(GetProcAddress(h_instance, "ExecuteScript"));
		fn_executescript(LR"(document.getElementById("Specific_ID").innerText)",
						 [](HRESULT err, const WCHAR* str)->HRESULT
						 {
						 	 // str에 Specific_ID에 대한 내용이 담겨져있음.
						 	 return S_OK;
						 });
		*/
	};

	fn_runwebview(&wvp);

	FreeLibrary(h_instance);
}

