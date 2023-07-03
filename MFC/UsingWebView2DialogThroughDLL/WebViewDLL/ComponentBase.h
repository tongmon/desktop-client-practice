#pragma once

#include <wil/com.h>
#include <wrl.h>
#include <Windows.h>

#include "WebView2EnvironmentOptions.h"
#include "WebView2.h"

class ComponentBase
{
public:
	// *result defaults to 0
	virtual bool HandleWindowMessage(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam,
		LRESULT* result)
	{
		return false;
	}
	virtual ~ComponentBase() {}
};
