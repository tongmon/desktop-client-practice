#include "pch.h"
#include "WebViewDialog.h"
#include "winuser.h"
#include "ViewComponent.h"
#include "resource.h"
#include <ShObjIdl_core.h>
#include <Shellapi.h>
#include <ShlObj_core.h>
#include <regex>

#ifdef __windows__
#undef __windows__
#endif

using Microsoft::WRL::Callback;
static constexpr UINT s_runAsyncWindowMessage = WM_APP;

WebViewDialog::WebViewDialog(universal_string url, CWnd* parent, const universal_string& title, int width, int height, const std::vector<std::wstring>& element_ids)
	: CDialog(IDD_WEBVIEW2_DIALOG, parent), m_url{ url }, m_title{ title }, m_window_width{ width }, m_window_height{height}
{
	is_direct_close = is_base64_encoded = should_get_msg_when_closed = false;
	m_message_from_web = L"";

	for (const auto& element : element_ids)
		m_html_result[element] = L"";
}

void WebViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
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

void WebViewDialog::OnSize(UINT opt, int width, int height)
{
	MoveWindow(0, 0, m_window_width, m_window_height);
	ResizeEverything();
}

BOOL WebViewDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	HRESULT hresult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	SetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA, (LONG_PTR)this);

	// ������ ����
	SetIcon(m_icon, TRUE);	// ū ������, FALSE�� ���� ������

	// ������ �ؽ�Ʈ ����
	SetWindowText(m_title.c_str());

	// WebView2 �ʱ�ȭ
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
	is_direct_close = true;

	// �ݴ� ��� ���� �� �޽��� �±� ID�� ���� �����ؾ� �Ѵ�.
	// �ϴ� �ӽ÷� Result�� �����س���.
	std::wstring element_id = L"Result", script_content = L"document.getElementById(\"%ELEMENT_ID%\").innerText";
	script_content = std::regex_replace(script_content, std::wregex(L"%ELEMENT_ID%"), element_id.c_str());

	// ���� �ݴ� ��쿡�� ������ ������ ������� �������� ���� ������ Ȱ��ȭ�ϸ� �ȴ�.  
	if (should_get_msg_when_closed)
	{
		m_webview->ExecuteScript(script_content.c_str(),
								 Callback<ICoreWebView2ExecuteScriptCompletedHandler>([this](HRESULT errorCode, LPCWSTR result)-> HRESULT
								 {
									 m_message_from_web = result;
									 m_message_from_web = std::regex_replace(m_message_from_web, std::wregex(L"\\n\\r"), L"\n\r");
									 m_message_from_web = std::regex_replace(m_message_from_web, std::wregex(L"\\n"), L"\n");
									 m_message_from_web = std::move(m_message_from_web.substr(1, m_message_from_web.length() - 2));
									 return S_OK;
								 }).Get());
	}

	CDialog::OnClose();
}

void WebViewDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

void WebViewDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

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
	PostMessage(s_runAsyncWindowMessage, reinterpret_cast<WPARAM>(task), 0);
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
			AfxMessageBox(_T("WebView2�� ��ġ�ؾ� �մϴ�."));
		else
			AfxMessageBox(_T("���� ȯ�� ������ �����Ͽ����ϴ�."));		
	}
}

HRESULT WebViewDialog::DCompositionCreateDevice2(IUnknown* renderingDevice, REFIID riid, void** ppv)
{
	HRESULT hr = E_FAIL;
	static decltype(::DCompositionCreateDevice2)* fnCreateDCompDevice2 = nullptr;
	if (fnCreateDCompDevice2 == nullptr)
	{
		HMODULE hmod = ::LoadLibraryEx(_T("dcomp.dll"), nullptr, 0);
		if (hmod != nullptr)
		{
			fnCreateDCompDevice2 = reinterpret_cast<decltype(::DCompositionCreateDevice2)*>(
				::GetProcAddress(hmod, "DCompositionCreateDevice2"));
		}
	}
	if (fnCreateDCompDevice2 != nullptr)
	{
		hr = fnCreateDCompDevice2(renderingDevice, riid, ppv);
	}
	return hr;
}

void WebViewDialog::CloseWebView(bool cleanupUserDataFolder)
{
	if (m_controller)
	{
		m_controller->Close();
		m_controller = nullptr;
		m_webview = nullptr;
	}
	m_webview_environment = nullptr;
	if (cleanupUserDataFolder)
	{
		//Clean user data        
	}
}

HRESULT WebViewDialog::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
	m_webview_environment = environment;
	m_webview_environment->CreateCoreWebView2Controller(this->GetSafeHwnd(), Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this, &WebViewDialog::OnCreateCoreWebView2ControllerCompleted).Get());

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

		EventRegistrationToken token;

		// �� �ݱ� �޽��� ���ù�
		m_webview->add_WindowCloseRequested(Callback<ICoreWebView2WindowCloseRequestedEventHandler>(this, &WebViewDialog::WebCloseMessageReceived).Get(), &token);

		// �� �ּ� �ٲ� �� ȣ��Ǵ� ���ù�
		m_webview->add_NavigationCompleted(Callback<ICoreWebView2NavigationCompletedEventHandler>(this, &WebViewDialog::WebNavigationCompleteMessageReceived).Get(), &token);

		// ������ �Ѹ� �޽��� �޴� ���ù�
		m_webview->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(this, &WebViewDialog::WebMessageReceived).Get(), &token);

		HRESULT hresult = m_webview->Navigate(
#if defined(UNICODE) || defined(_UNICODE)
			m_url.c_str()
#else
			StrToWStr(m_url).c_str()
#endif
		);

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

// ������ �� �޽����� �޴� �Լ�
// ������ �������� window.chrome.webview.postMessage('some message');�� �����ϸ� Ŭ���̾�Ʈ���� �� �Լ��� �����
// some message �޽����� TryGetWebMessageAsString�� ���� ȹ���� �� ����
HRESULT WebViewDialog::WebMessageReceived(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args)
{
	LPWSTR web_msg;
	args->TryGetWebMessageAsString(&web_msg);
	std::wstring received_message = web_msg;

	// ���� some message��� ������ ������ �޽����� �޴� ���� ����
	if (received_message == L"some message")
	{
		if (!m_html_result.empty())
		{
			std::wstring base_script = L"document.getElementById(\"%ELEMENT_ID%\").innerText";

			for (auto& content : m_html_result)
			{
				std::wstring script = std::regex_replace(base_script, std::wregex(L"%ELEMENT_ID%"), content.first);

				m_webview->ExecuteScript(script.c_str(),
										 Callback<ICoreWebView2ExecuteScriptCompletedHandler>([this](HRESULT errorCode, LPCWSTR result)-> HRESULT
										 {
											 m_message_from_web = result;
											 m_message_from_web = std::regex_replace(m_message_from_web, std::wregex(L"\\n\\r"), L"\n\r");
											 m_message_from_web = std::regex_replace(m_message_from_web, std::wregex(L"\\n"), L"\n");
											 m_message_from_web = std::move(m_message_from_web.substr(1, m_message_from_web.length() - 2));
											 return S_OK;
										 }).Get());

				m_html_result[content.first] = m_message_from_web;
			}
		}
	}
	return S_OK;
}

// ���� ������ �ϴ� ��� �̺�Ʈ �ڵ鷯
// window.close(); ���� ��ũ��Ʈ�� ����Ǹ� �۵��ؾ� �ϳ�... Ʈ���Ű� �� �ȵǴ� �� ��
HRESULT WebViewDialog::WebCloseMessageReceived(ICoreWebView2* sender, IUnknown* args)
{
	EndDialog(0);
	return S_OK;
}

// �� �ּ� �ٲ�� ��� �̺�Ʈ �ڵ鷯
HRESULT WebViewDialog::WebNavigationCompleteMessageReceived(ICoreWebView2* sender, IUnknown* args)
{

	return S_OK;
}

std::wstring WebViewDialog::GetElementValue(const std::wstring& key)
{
	if (m_html_result[key] == L"null")
		return L"";
	if (is_base64_encoded)
		m_message_from_web = std::move(StrToWStr(DecodeBase64(WStrToStr(m_message_from_web))));
	return m_html_result[key];
}