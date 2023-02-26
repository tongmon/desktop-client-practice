
// CEF_multiprocess.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "CEF_multiprocess.h"
#include "MainFrm.h"

#include "CEF_multiprocessDoc.h"
#include "CEF_multiprocessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCEFmultiprocessApp

BEGIN_MESSAGE_MAP(CCEFmultiprocessApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CCEFmultiprocessApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CCEFmultiprocessApp 생성

CCEFmultiprocessApp::CCEFmultiprocessApp() noexcept
{

	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("CEFmultiprocess.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CCEFmultiprocessApp 개체입니다.

CCEFmultiprocessApp theApp;


// CCEFmultiprocessApp 초기화

BOOL CCEFmultiprocessApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	// 애플리케이션의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCEFmultiprocessDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CCEFmultiprocessView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// CCEFmultiprocessApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

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

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


BOOL DoExternalModal(HWND hwnd, LPCSTR ApplicationName)
{
	EnableWindow(hwnd, FALSE);

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL ret = FALSE;
	DWORD flags = CREATE_NO_WINDOW;

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = NULL;
	si.hStdError = NULL;
	si.hStdOutput = NULL;

	CString cmd = _T("cmd /c ") + CString(ApplicationName);
	BOOL inherits = FALSE;

	ret = CreateProcess(NULL,
		cmd.GetBuffer(),
		NULL,
		NULL,
		inherits,
		flags,
		NULL,
		NULL,
		&si,
		&pi);

	if (ret)
	{
		CloseHandle(pi.hThread);

		MSG msg;

		while (1)
		{
			switch (MsgWaitForMultipleObjectsEx(1, &pi.hProcess, INFINITE, QS_ALLINPUT, 0))
			{
			case WAIT_OBJECT_0:
				CloseHandle(pi.hProcess);
				EnableWindow(hwnd, TRUE);
				::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); // HWND_TOP HWND_TOPMOST
				::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				// SetActiveWindow(hwnd);
				// SetFocus(hwnd);
				return TRUE;
			case WAIT_OBJECT_0 + 1:
				while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				continue;
			default: __debugbreak();
			}
		}
	}

	return FALSE;
}

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CCEFmultiprocessApp::OnAppAbout()
{
	//CAboutDlg aboutDlg;
	//aboutDlg.DoModal();

	/*
	EnableWindow(m_pMainWnd->m_hWnd, FALSE);

	SHELLEXECUTEINFO sei;
	::ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));

	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.lpFile = "C:\\Users\\DP91-HSK\\source\\repos\\CEF_multiprocess\\CEF_EXE\\cefsimple.exe";
	sei.lpParameters = "";
	sei.nShow = SW_SHOW;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = "open";

	DWORD result = ::ShellExecuteEx(&sei);

	if (sei.hProcess != NULL)
	{
		DWORD nResult;
		// while ((nResult = WaitForSingleObject(sei.hProcess, 0)) == WAIT_TIMEOUT);
		::WaitForSingleObject(sei.hProcess, INFINITE); //여기서 해당 프로세스가 종료될때까지 대기하게됨
	}

	EnableWindow(m_pMainWnd->m_hWnd, TRUE);
	*/

	/*
	EnableWindow(m_pMainWnd->m_hWnd, FALSE);

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL ret = FALSE;
	DWORD flags = CREATE_NO_WINDOW;

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = NULL;
	si.hStdError = NULL;
	si.hStdOutput = NULL;

	CString cmd = _T("cmd /c ") + CString(_T("C:\\Users\\DP91-HSK\\source\\repos\\CEF_multiprocess\\CEF_EXE\\cefsimple.exe"));
	BOOL inherits = FALSE;

	ret = CreateProcess(NULL,
		cmd.GetBuffer(),
		NULL,
		NULL,
		inherits,
		flags,
		NULL,
		NULL,
		&si,
		&pi);

	if (ret)
	{
		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		EnableWindow(m_pMainWnd->m_hWnd, TRUE);
	
		
		DWORD exit_code;
		if (FALSE == GetExitCodeProcess(pi.hProcess, &exit_code))
		{
			GetLastError();
		}
		else if (STILL_ACTIVE == exit_code)
		{
			// 실행중...
		}
		else
		{
			// 종료됨
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			EnableWindow(m_pMainWnd->m_hWnd, TRUE);
		}
		
	}
	*/

	// 경로 알맞게 바꾸셈
	DoExternalModal(m_pMainWnd->m_hWnd, "C:\\Users\\tongm\\OneDrive\\Documents\\GitHub\\Cef_Project\\Codes\\CEF_Multiprocess\\CEF_EXE\\cefsimple.exe");
}

// CCEFmultiprocessApp 메시지 처리기



