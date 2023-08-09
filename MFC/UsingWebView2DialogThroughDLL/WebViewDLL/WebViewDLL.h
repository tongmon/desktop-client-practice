﻿// Visual Studio에서 MFC 동적 연결 라이브러리 템플릿을 사용해 생성함
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
	~CWebViewDLLApp();

	WebViewDialog* webview_dlg;

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

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

#ifdef __cplusplus
extern "C"
{
#endif 

	void RunWebViewDialog(WebViewParam* wvp);

	bool ExecuteScript(const universal_string& script, HRESULT(*callback)(HRESULT, const WCHAR*) = nullptr);

#ifdef __cplusplus 
}
#endif 