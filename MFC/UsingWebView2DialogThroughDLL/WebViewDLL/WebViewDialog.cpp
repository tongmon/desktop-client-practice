#include "pch.h"
#include "WebViewDialog.h"
#include "WebViewDLL.h"
#include "winuser.h"
#include "ViewComponent.h"
#include "resource.h"
#include <ShObjIdl_core.h>
#include <Shellapi.h>
#include <ShlObj_core.h>
#include <regex>
#include <algorithm>

#ifdef __windows__
#undef __windows__
#endif

using Microsoft::WRL::Callback;
static constexpr UINT WM_WEBVIEW2_APP_CALLBACK = WM_APP + 123;

WebViewDialog::WebViewDialog(const universal_string& url, HWND parent, const universal_string& title, SIZE size, std::function<void(LPCWSTR)>* callbacks)
	: CDialog(IDD_WEBVIEW2_DIALOG, CWnd::FromHandle(parent)), m_main_window{ nullptr },
	m_url(NormalizeUrl(url)),
	m_title{ title },
	window_width{ size.cx },
	window_height{ size.cy },
	m_creation_mode_id{ 0 }
{
	for (size_t i = 0; i < WebViewParam::CallBackCnt; i++)
		m_callbacks.push_back(callbacks[i]);
}

void WebViewDialog::DoDataExchange(CDataExchange* pdx)
{
	CDialog::DoDataExchange(pdx);
}

BEGIN_MESSAGE_MAP(WebViewDialog, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

std::wstring WebViewDialog::NormalizeUrl(const universal_string& url)
{
#if defined(UNICODE) || defined(_UNICODE)
	return Utf8ToWStr(WStrToUtf8(url));
#else
	return Utf8ToWStr(StrToUtf8(url));
#endif
}

void WebViewDialog::OnSize(UINT opt, int width, int height)
{
	static bool first_load = true;
	if (first_load)
	{
		MoveWindow(0, 0, window_width, window_height);
		first_load = false;
	}
	else
	{
		window_width = width;
		window_height = height;
		ResizeEverything();
	}
	CDialog::OnSize(opt, window_width, window_height);
}

BOOL WebViewDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	HRESULT hresult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	SetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA, (LONG_PTR)this);

	// 아이콘 설정
	SetIcon(m_icon, TRUE);	// 큰 아이콘, FALSE면 작은 아이콘

	// 윈도우 텍스트 설정
	SetWindowText(m_title.c_str());

	// WebView2 초기화
	InitializeWebView();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL WebViewDialog::DestroyWindow()
{
	CloseWebView(true);
	return CDialog::DestroyWindow();
}

afx_msg void WebViewDialog::OnClose()
{
	if (m_callbacks[WebViewParam::OnDialogClose])
		m_callbacks[WebViewParam::OnDialogClose](L"close");

	CDialog::OnClose();
}

void WebViewDialog::OnSysCommand(UINT n_id, LPARAM lparam)
{
	CDialog::OnSysCommand(n_id, lparam);
}

void WebViewDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cx_icon = GetSystemMetrics(SM_CXICON);
		int cy_icon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cx_icon + 1) / 2;
		int y = (rect.Height() - cy_icon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_icon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR WebViewDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_icon);
}

void WebViewDialog::ResizeEverything()
{
	RECT availableBounds = { 0 };
	GetClientRect(&availableBounds);

	if (auto view = GetComponent<ViewComponent>())
	{
		view->SetBounds(availableBounds);
	}
}

void WebViewDialog::RunAsync(std::function<void()> callback)
{
	auto* task = new std::function<void()>(callback);
	PostMessage(WM_WEBVIEW2_APP_CALLBACK, reinterpret_cast<WPARAM>(task), 0);
}

void WebViewDialog::InitializeWebView()
{
	CloseWebView();
	m_dcomp_device = nullptr;

#ifdef USE_WEBVIEW2_WIN10
	m_wincompCompositor = nullptr;
#endif

	auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
	options->put_AllowSingleSignOnUsingOSPrimaryAccount(FALSE);

	HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, options.Get(), Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &WebViewDialog::OnCreateEnvironmentCompleted).Get());

	if (!SUCCEEDED(hr))
	{
		if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			// 웹뷰 설치가 안되어 있는 경우 웹뷰 설치

			// 1. 온라인 연결이 가능하여 웹뷰를 설치할 수 있는 경우 편하게 MicrosoftEdgeWebview2Setup를 이용
			// MicrosoftEdgeWebview2Setup.exe는 경로에 따라 달라져야됨.
			// std::system("MicrosoftEdgeWebview2Setup.exe /silent /install");

			// 2. 온라인 연결 없이 웹뷰를 설치해야 하는 경우 불편하고 용량이 큰 MicrosoftEdgeWebView2RuntimeInstaller를 이용
			LONG result, is_webview_installed = 0;
			HKEY hkey;
			DWORD type;
			DWORD bytes = MAX_PATH;
			TCHAR str_regkey[MAX_PATH] = { 0, };

			// 레지스트리에 웹뷰가 등록되어 있는지 확인하고 안되어 있으면 설치
			// MicrosoftEdgeWebview2Setup을 이용하면 알아서 확인해주지만 오프라인 전용 인스톨러는 그런거 없음
			for (int i = 0; i < 4 && !is_webview_installed; i++)
			{
				// Regstry Key 탐색
				result = RegOpenKeyEx((i % 2 ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE),
									  (i ? _T("SOFTWARE\\Microsoft\\EdgeUpdate\\Clients\\{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}") : _T("SOFTWARE\\WOW6432Node\\Microsoft\\EdgeUpdate\\Clients\\{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}")),
									  0, KEY_READ, &hkey);
				if (result != ERROR_SUCCESS)
					continue;

				// Regstry Key 값 획득
				result = RegQueryValueEx(hkey, _T("pv"), 0, &type, (LPBYTE)str_regkey, &bytes);

				if (result == ERROR_SUCCESS && str_regkey[0] != 0 && lstrcmp(str_regkey, _T("0.0.0.0")))
					is_webview_installed = 1;
			}

			// MicrosoftEdgeWebView2RuntimeInstaller의 이름은 배포될 아키텍쳐에 따라 달라져야됨.
			if (!is_webview_installed)
				std::system("MicrosoftEdgeWebView2RuntimeInstallerX64.exe /silent /install");

			// AfxMessageBox(_T("WebView2를 설치해야 합니다."));
		}
		else
			AfxMessageBox(_T("웹뷰 환경 구성에 실패하였습니다."));
	}
}

HRESULT WebViewDialog::DCompositionCreateDevice2(IUnknown* rendering_device, REFIID riid, void** ppv)
{
	HRESULT hr = E_FAIL;
	static decltype(::DCompositionCreateDevice2)* fn_create_dcomp_device2 = nullptr;
	if (!fn_create_dcomp_device2)
	{
		HMODULE hmod = ::LoadLibraryEx(_T("dcomp.dll"), nullptr, 0);
		if (hmod)
		{
			fn_create_dcomp_device2 = reinterpret_cast<decltype(::DCompositionCreateDevice2)*>(
				::GetProcAddress(hmod, "DCompositionCreateDevice2"));
		}
	}
	if (fn_create_dcomp_device2)
	{
		hr = fn_create_dcomp_device2(rendering_device, riid, ppv);
	}
	return hr;
}

void WebViewDialog::CloseWebView(bool cleanup_user_data_folder)
{
	if (m_controller)
	{
		m_controller->Close();
		m_controller = nullptr;
		m_webview = nullptr;
		m_webview_2 = nullptr;
		// m_webview_16 = nullptr;
		m_web_settings = nullptr;
	}

	m_webview_environment = nullptr;
	m_webview_environment_2 = nullptr;

	if (cleanup_user_data_folder)
	{
		//Clean user data        
	}
}

HRESULT WebViewDialog::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
	environment->QueryInterface(IID_PPV_ARGS(&m_webview_environment));
	environment->QueryInterface(IID_PPV_ARGS(&m_webview_environment_2));

	m_webview_environment->CreateCoreWebView2Controller(this->GetSafeHwnd(),
														Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this, &WebViewDialog::OnCreateCoreWebView2ControllerCompleted).Get());

	return S_OK;
}

HRESULT WebViewDialog::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller)
{
	if (result == S_OK)
	{
		m_controller = controller;
		wil::com_ptr<ICoreWebView2> coreWebView2;
		m_controller->get_CoreWebView2(&coreWebView2);
		coreWebView2.query_to(&m_webview);

		NewComponent<ViewComponent>(
			this, m_dcomp_device.get(),
#ifdef USE_WEBVIEW2_WIN10
			m_wincompCompositor,
#endif
			m_creation_mode_id == 195); // IDM_CREATION_MODE_TARGET_DCOMP

		m_webview->QueryInterface(IID_PPV_ARGS(&m_webview_2));
		// m_webview->QueryInterface(IID_PPV_ARGS(&m_webview_16));

		m_webview->get_Settings(&m_web_settings);

		// 웹 이벤트 등록
		ConnectEventHandlers();

		HRESULT hresult = m_webview->Navigate(m_url.c_str());

		if (hresult == S_OK)
		{
			TRACE("Web Page Opened Successfully");
			ResizeEverything();
		}
	}
	else
	{
		TRACE("Failed to create webview");
	}
	return S_OK;
}

void WebViewDialog::ConnectEventHandlers()
{
	EventRegistrationToken token;

	// 웹 닫기 메시지 리시버
	m_webview->add_WindowCloseRequested(Callback<ICoreWebView2WindowCloseRequestedEventHandler>(this, &WebViewDialog::WebCloseMessageReceived).Get(), &token);

	// 웹 주소 변환 시작시 호출되는 리시버
	m_webview->add_NavigationStarting(Callback<ICoreWebView2NavigationStartingEventHandler>(this, &WebViewDialog::WebNavigationStartMessageReceived).Get(), &token);

	// 웹 주소 변환 성공시 호출되는 리시버
	m_webview->add_NavigationCompleted(Callback<ICoreWebView2NavigationCompletedEventHandler>(this, &WebViewDialog::WebNavigationCompleteMessageReceived).Get(), &token);

	// 웹에서 뿌린 메시지 받는 리시버
	m_webview->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(this, &WebViewDialog::WebMessageReceived).Get(), &token);

	// 이 외에도 많은 이벤트 종류가 있음
}

// 웹 주소 탐색
void WebViewDialog::Navigate(const universal_string& url)
{
	if (!m_webview)
		return;

	m_url = NormalizeUrl(url);
	m_webview->Navigate(m_url.c_str());
}

// Post 방식으로 웹 주소 탐색
void WebViewDialog::NavigatePost(const universal_string& url, const universal_string& content, const universal_string& headers)
{
	if (!m_webview)
		return;

	m_url = NormalizeUrl(url);

	wil::com_ptr<ICoreWebView2WebResourceRequest> web_resource_request;
	wil::com_ptr<IStream> post_data_stream = SHCreateMemStream(reinterpret_cast<const BYTE*>(content.c_str()),
															   static_cast<UINT>(content.length() + 1));

	m_webview_environment_2->CreateWebResourceRequest(
		m_url.c_str(),
		L"POST",
		post_data_stream.get(),
#if defined(UNICODE) || defined(_UNICODE)
		headers.c_str(),
#else
		StrToWStr(headers).c_str(),
#endif
		& web_resource_request);

	m_webview_2->NavigateWithWebResourceRequest(web_resource_request.get());
}

void WebViewDialog::Stop()
{
	if (!m_webview)
		return;

	m_webview->Stop();
}

void WebViewDialog::Reload()
{
	if (!m_webview)
		return;

	m_webview->Reload();
}

void WebViewDialog::GoBack()
{
	if (!m_webview)
		return;

	BOOL can_go_back = FALSE;
	m_webview->get_CanGoBack(&can_go_back);

	if (can_go_back)
		m_webview->GoBack();
}

void WebViewDialog::GoForward()
{
	if (!m_webview)
		return;

	BOOL can_go_forward = FALSE;
	m_webview->get_CanGoForward(&can_go_forward);

	if (can_go_forward)
		m_webview->GoForward();
}

void WebViewDialog::DisablePopups()
{
	if (!m_web_settings)
		return;

	m_web_settings->put_AreDefaultScriptDialogsEnabled(FALSE);
}

void WebViewDialog::ExecuteScript(const universal_string& code, std::function<HRESULT(HRESULT, PCWSTR)> callback)
{
	if (!m_webview)
		return;

	m_webview->ExecuteScript(
#if defined(UNICODE) || defined(_UNICODE)
		code.c_str(),
#else
		StrToWStr(code).c_str(),
#endif
		Callback<ICoreWebView2ExecuteScriptCompletedHandler>(callback).Get());
}

// 웹에서 준 메시지를 받는 함수
// 페이지 로직에서 window.chrome.webview.postMessage('some message');를 수행하면 클라이언트에서 밑 함수가 수행됨
// some message 메시지는 TryGetWebMessageAsString를 통해 획득할 수 있음
HRESULT WebViewDialog::WebMessageReceived(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args)
{
	LPWSTR web_msg = nullptr;
	if (args->TryGetWebMessageAsString(&web_msg) != E_INVALIDARG && m_callbacks[WebViewParam::OnMessageReceived])
		m_callbacks[WebViewParam::OnMessageReceived](web_msg);

	CoTaskMemFree(web_msg);
	return S_OK;
}

// 웹을 닫으려 하는 경우 이벤트 핸들러
// window.close(); 등의 스크립트가 수행되면 작동해야 하나... 트리거가 잘 안되는 듯 함
HRESULT WebViewDialog::WebCloseMessageReceived(ICoreWebView2* sender, IUnknown* args)
{
	if (m_callbacks[WebViewParam::OnCloseMessageReceived])
		m_callbacks[WebViewParam::OnCloseMessageReceived](L"close");

	EndDialog(0);
	return S_OK;
}

HRESULT WebViewDialog::WebNavigationCompleteMessageReceived(ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args)
{
	BOOL is_success = FALSE;
	args->get_IsSuccess(&is_success);

	if (m_callbacks[WebViewParam::OnNavigationCompleteMessageReceived])
		m_callbacks[WebViewParam::OnNavigationCompleteMessageReceived](is_success ? L"success" : L"fail");

	return S_OK;
}

HRESULT WebViewDialog::WebNavigationStartMessageReceived(ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args)
{
	wil::unique_cotaskmem_string uri;
	args->get_Uri(&uri);

	if (m_callbacks[WebViewParam::OnNavigationStartMessageReceived])
		m_callbacks[WebViewParam::OnNavigationStartMessageReceived](uri.get());

	return S_OK;
}

BOOL WebViewDialog::PreTranslateMessage(MSG* msg)
{
	switch (msg->message)
	{
	case WM_WEBVIEW2_APP_CALLBACK: {
		auto* task = reinterpret_cast<std::function<void()>*>(msg->wParam);
		(*task)();
		delete task;
		break;
	}
	default:
		break;
	}

	return CDialog::PreTranslateMessage(msg);
}

/*
https://mariusbancila.ro/blog/2021/01/27/using-microsoft-edge-in-a-native-windows-desktop-app-part-4/
void WebViewDialog::PrintToPDF(bool const landscape, std::function<void(bool, CString)> callback)
{
   WCHAR defaultName[MAX_PATH] = L"default.pdf";

   OPENFILENAME openFileName = {};
   openFileName.lStructSize   = sizeof(openFileName);
   openFileName.hwndOwner     = nullptr;
   openFileName.hInstance     = nullptr;
   openFileName.lpstrFile     = defaultName;
   openFileName.lpstrFilter   = L"PDF File\0*.pdf\0";
   openFileName.nMaxFile      = MAX_PATH;
   openFileName.Flags         = OFN_OVERWRITEPROMPT;

   if (::GetSaveFileName(&openFileName))
   {
	  wil::com_ptr<ICoreWebView2PrintSettings> printSettings = nullptr;
	  wil::com_ptr<ICoreWebView2Environment6> webviewEnvironment6;
	  CHECK_FAILURE(m_pImpl->m_webViewEnvironment->QueryInterface(IID_PPV_ARGS(&webviewEnvironment6)));

	  if (webviewEnvironment6)
	  {
		 CHECK_FAILURE(webviewEnvironment6->CreatePrintSettings(&printSettings));
		 CHECK_FAILURE(printSettings->put_Orientation(landscape ? COREWEBVIEW2_PRINT_ORIENTATION_LANDSCAPE :
																  COREWEBVIEW2_PRINT_ORIENTATION_PORTRAIT));
	  }

	  wil::com_ptr<ICoreWebView2_7> webview2_7;
	  CHECK_FAILURE(m_pImpl->m_webView->QueryInterface(IID_PPV_ARGS(&webview2_7)));
	  if (webview2_7)
	  {
		 m_printToPdfInProgress = true;

		 CHECK_FAILURE(webview2_7->PrintToPdf(
			openFileName.lpstrFile,
			printSettings.get(),
			Callback<ICoreWebView2PrintToPdfCompletedHandler>(
			   [this, callback, &openFileName](HRESULT errorCode, BOOL isSuccessful) -> HRESULT {
				  CHECK_FAILURE(errorCode);

				  m_printToPdfInProgress = false;

				  callback(isSuccessful, openFileName.lpstrFile);

				  return S_OK;
			   })
			.Get()));
	  }
   }
}
*/