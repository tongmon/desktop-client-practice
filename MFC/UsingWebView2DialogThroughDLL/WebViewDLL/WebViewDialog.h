#pragma once
#include "ComponentBase.h"
#include <dcomp.h>
#include <functional>
#include <memory>
#include <ole2.h>
#include <string>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <winnt.h>
#include <afxwin.h>
#include <shlobj.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#ifdef __windows__
#undef __windows__
#endif

#ifdef USE_WEBVIEW2_WIN10
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.ViewManagement.h>
namespace winrtComp = winrt::Windows::UI::Composition;
#endif

using universal_string = std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>>;

class WebViewDialog : public CDialog
{
	HWND m_main_window;
	wil::com_ptr<ICoreWebView2> m_webview;
	wil::com_ptr<ICoreWebView2_2> m_webview_2;
	//wil::com_ptr<ICoreWebView2_16> m_webview_16;
	wil::com_ptr<ICoreWebView2Environment> m_webview_environment;
	wil::com_ptr<ICoreWebView2Environment2> m_webview_environment_2;
	wil::com_ptr<ICoreWebView2Controller> m_controller;
	wil::com_ptr<ICoreWebView2Settings> m_web_settings;
	wil::com_ptr<IDCompositionDevice> m_dcomp_device;
	std::vector<std::unique_ptr<ComponentBase>> m_components;
	template <class ComponentType, class... Args> void NewComponent(Args&&... args);

	HICON m_icon;
	DWORD m_creation_mode_id;
	universal_string m_title;
	std::wstring m_url;

	std::vector<std::function<void(LPCWSTR)>> m_callbacks;

public:
	int window_width;
	int window_height;

	WebViewDialog(const universal_string& url,
				  HWND parent, 
				  const universal_string& title, 
				  SIZE size,
				  std::function<void(LPCWSTR)> *callbacks);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEBVIEW2_DIALOG };
#endif

	void InitializeWebView();
	void CloseWebView(bool cleanup_user_data_folder = false);
	void RunAsync(std::function<void()> callback);
	void ResizeEverything();
	void ConnectEventHandlers();
	HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
	HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
	HRESULT DCompositionCreateDevice2(IUnknown* rendering_device, REFIID riid, void** ppv);
	HRESULT WebNavigationCompleteMessageReceived(ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args);
	HRESULT WebNavigationStartMessageReceived(ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args);
	HRESULT WebCloseMessageReceived(ICoreWebView2* sender, IUnknown* args);
	HRESULT WebMessageReceived(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args);

	ICoreWebView2Controller* GetWebViewController() { return m_controller.get(); }
	ICoreWebView2* GetWebView() { return m_webview.get(); }
	ICoreWebView2Environment* GetWebViewEnvironment() { return m_webview_environment.get(); }
	HWND GetMainWindow() { return this->GetSafeHwnd(); }

	void Navigate(const universal_string& url);
	void NavigatePost(const universal_string& url, const universal_string& content, const universal_string& headers);
	
	void Stop();
	void Reload();
	void GoBack();
	void GoForward();

	void DisablePopups();

	void ExecuteScript(const universal_string& code, std::function<HRESULT(HRESULT, PCWSTR)> callback = {});

private:
	template <class ComponentType> ComponentType* GetComponent();
	std::wstring NormalizeUrl(const universal_string& url);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* msg);
	afx_msg void OnSysCommand(UINT n_id, LPARAM lparam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT opt, int width, int height);
	afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
	void DoDataExchange(CDataExchange* pdx);
	DECLARE_MESSAGE_MAP()
};

template <class ComponentType, class... Args> 
void WebViewDialog::NewComponent(Args&&... args)
{
	m_components.emplace_back(new ComponentType(std::forward<Args>(args)...));
}

template <class ComponentType> 
ComponentType* WebViewDialog::GetComponent()
{
	for (auto& component : m_components)
	{
		if (auto wanted = dynamic_cast<ComponentType*>(component.get()))
		{
			return wanted;
		}
	}
	return nullptr;
}

inline std::wstring StrToWStr(const std::string& str)
{
	if (str.empty())
		return L"";

	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), nullptr, 0);
	if (len <= 0)
		return L"";

	std::wstring wstr(len + 1, 0);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), &wstr.at(0), len);
	return wstr;
}

inline std::string WStrToStr(const std::wstring& wstr)
{
	if (wstr.empty())
		return "";

	int len = WideCharToMultiByte(CP_ACP, 0, &wstr.at(0), (int)wstr.length(), nullptr, 0, nullptr, nullptr);
	if (len <= 0)
		return "";

	std::string str(len + 1, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstr.at(0), (int)wstr.length(), &str.at(0), len, nullptr, nullptr);
	return str;
}

inline std::string StrToUtf8(const std::string& str)
{
	std::wstring wstr = std::move(StrToWStr(str));
	if (wstr.empty())
		return "";

	int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
	if (len <= 0)
		return "";

	std::string utf_8(len + 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), &utf_8.at(0), len, NULL, NULL);
	return utf_8;
}

inline std::wstring Utf8ToWStr(const std::string& utf_8)
{
	if (utf_8.empty())
		return L"";

	int len = MultiByteToWideChar(CP_UTF8, 0, utf_8.c_str(), (int)utf_8.length(), NULL, NULL);
	if (len <= 0)
		return L"";

	std::wstring wstr(len + 1, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf_8.c_str(), (int)utf_8.length(), &wstr.at(0), len);
	return wstr;
}

inline std::string WStrToUtf8(const std::wstring& wstr)
{
	if (wstr.empty())
		return "";

	int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
	if (len <= 0)
		return "";

	std::string uft_8(len + 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), &uft_8.at(0), len, NULL, NULL);
	return uft_8;
}

// UTF-8 Base64 Encoding
inline std::string EncodeBase64(const std::string& str)
{
	std::string ret;
	unsigned int val = 0;
	int valb = -6;

	for (unsigned char c : str)
	{
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0)
		{
			ret.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) 
		ret.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
	while (ret.size() % 4)
		ret.push_back('=');
	return ret;
}

// UTF-8 Base64 Decoding
inline std::string DecodeBase64(const std::string& str_encoded)
{
	std::string ret;
	std::vector<int> T(256, -1);
	unsigned int val = 0;
	int valb = -8;

	for (int i = 0; i < 64; i++) 
		T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
	for (unsigned char c : str_encoded)
	{
		if (T[c] == -1) 
			break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0)
		{
			ret.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return ret;
}