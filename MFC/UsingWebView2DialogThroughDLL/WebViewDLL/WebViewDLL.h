// Visual Studio에서 MFC 동적 연결 라이브러리 템플릿을 사용해 생성함
// Nuget Package를 이용하는 방식과 vcpkg를 이용하는 방식이 있음
// 어느 쪽이던 WebView2와 wil(Windows Implementation Library)는 설치되어야 함

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "WebViewDialog.h"
#include "resource.h"		// 주 기호입니다.

class CWebViewDLLApp : public CWinApp
{
public:
	CWebViewDLLApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

void RunWebViewTest()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WebViewDialog wvd(_T("https://www.google.com/"));

	wvd.DoModal();
}

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

void RunWebView(WebViewParam* wvp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WebViewDialog wvd(wvp->url, wvp->hwnd, wvp->title, wvp->size.cx, wvp->size.cy, wvp->element_ids);

	wvd.is_base64_encoded = wvp->is_base64_encoded;
	wvd.should_get_msg_when_closed = wvp->should_get_msg_when_closed;

	wvd.DoModal();

	wvp->is_direct_close = wvd.is_direct_close;
	wvp->html_result = std::move(wvd.GetFullWebResult());
}