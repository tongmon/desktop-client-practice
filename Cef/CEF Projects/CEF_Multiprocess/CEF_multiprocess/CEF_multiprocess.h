
// CEF_multiprocess.h: CEF_multiprocess 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// CCEFmultiprocessApp:
// 이 클래스의 구현에 대해서는 CEF_multiprocess.cpp을(를) 참조하세요.
//

class CCEFmultiprocessApp : public CWinApp
{
public:
	CCEFmultiprocessApp() noexcept;


// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCEFmultiprocessApp theApp;

/*
BOOL DoExternalModal(HWND hwnd, PCWSTR ApplicationName)
{
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    WCHAR CommandLine[32];
    swprintf(CommandLine, L"*%p", hwnd);

    if (CreateProcess(ApplicationName, CommandLine, 0, 0, 0, 0, 0, 0, &si, &pi))
    {
        CloseHandle(pi.hThread);

        MSG msg;

        for (;;)
        {
            switch (MsgWaitForMultipleObjectsEx(1, &pi.hProcess, INFINITE, QS_ALLINPUT, 0))
            {
            case WAIT_OBJECT_0:
                CloseHandle(pi.hProcess);
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
*/

BOOL DoExternalModal(HWND hwnd, LPCSTR ApplicationName);