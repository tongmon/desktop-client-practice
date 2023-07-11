#pragma once
#include "ComponentBase.h"
#include <dcomp.h>
#include <functional>
#include <memory>
#include <ole2.h>
#include <string>
#include <vector>
#include <string>
#include <unordered_map>
#include <winnt.h>
#include <afxwin.h>

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
private:

	wil::com_ptr<ICoreWebView2Environment> m_webview_environment;
	wil::com_ptr<ICoreWebView2Controller> m_controller;
	wil::com_ptr<ICoreWebView2> m_webview;
	wil::com_ptr<IDCompositionDevice> m_dcomp_device;
	std::vector<std::unique_ptr<ComponentBase>> m_components;
	HWND m_main_window = nullptr;
	HINSTANCE g_hinstance;
	static constexpr size_t s_max_laod_strLoadString = 100;
	template <class ComponentType, class... Args> void NewComponent(Args&&... args);

	HICON m_icon;
	DWORD m_creation_mode_id = 0;
	universal_string m_title;
	universal_string m_url;
	std::wstring m_message_from_web; // 스크립트 수행으로 전달되는 결과값
	int m_window_width;
	int m_window_height;
	std::unordered_map<std::wstring, std::wstring>& m_html_result; // 태그 ID별로 결과값 저장, key = element id, value = id에 저장된 값

public:
	bool is_base64_encoded;
	bool is_direct_close; // 직접 다이얼로그 프레임의 X 버튼을 눌러 껐는지 여부, 반환값이라고 보면 된다.
	bool should_get_msg_when_closed;

	// 웹에서 전달받을 element id들을 해쉬맵에 미리 넣어준다.  
	// 예를 들어 Element_ID에 해당하는 값을 웹에서 던져주고 싶다면 웹 페이지 로직은 document.getElementById('Element_ID').textContent = str; 와 같을 것이다.
	WebViewDialog(universal_string url, 
				  HWND parent, 
				  const universal_string& title, 
				  int width, 
				  int height, 
				  std::unordered_map<std::wstring, std::wstring>& element_ids);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEBVIEW2_DIALOG };
#endif

	void InitializeWebView();
	void CloseWebView(bool cleanup_user_data_folder = false);
	HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
	HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
	void RunAsync(std::function<void(void)> callback);
	void ResizeEverything();
	HRESULT DCompositionCreateDevice2(IUnknown* renderingDevice, REFIID riid, void** ppv);
	HRESULT WebNavigationCompleteMessageReceived(ICoreWebView2* sender, IUnknown* args);
	HRESULT WebCloseMessageReceived(ICoreWebView2* sender, IUnknown* args);
	HRESULT WebMessageReceived(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args);

	void OnSize(UINT opt, int width, int height);

	ICoreWebView2Controller* GetWebViewController() { return m_controller.get(); }
	ICoreWebView2* GetWebView() { return m_webview.get(); }
	ICoreWebView2Environment* GetWebViewEnvironment() { return m_webview_environment.get(); }
	HWND GetMainWindow() { return this->GetSafeHwnd(); }

	inline void SetSize(int width, int height) { m_window_width = width; m_window_height = height; }

	inline void SetUrl(LPCTSTR url) { m_url = url; }

	inline auto GetFullWebResult() { return m_html_result; }

	std::wstring GetElementValue(const std::wstring& key);

private:
	template <class ComponentType> ComponentType* GetComponent();

	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
	void DoDataExchange(CDataExchange* pDX);
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