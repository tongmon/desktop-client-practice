// Copyright (C) Microsoft Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#pragma once

#include "pch.h"
#include "ViewComponent.h"
#include <sstream>
#include <windowsx.h>
#include <WinUser.h>
#ifdef USE_WEBVIEW2_WIN10
#include <windows.ui.composition.interop.h>
#endif

//#include "CheckFailure.h"

using namespace Microsoft::WRL;
ViewComponent::ViewComponent(
	WebViewDialog* app_window,
	IDCompositionDevice* dcomp_device,
#ifdef USE_WEBVIEW2_WIN10
	winrtComp::Compositor wincomp_compositor,
#endif
	bool is_dcomp_target_mode)
	: m_app_window(app_window), m_controller(app_window->GetWebViewController()),
	m_webView(app_window->GetWebView()), m_dcomp_device(dcomp_device),
#ifdef USE_WEBVIEW2_WIN10
	m_wincomp_compositor(wincomp_compositor),
#endif
	m_is_dcomp_target_mode(is_dcomp_target_mode)
{

	m_controller->add_ZoomFactorChanged(
		Callback<ICoreWebView2ZoomFactorChangedEventHandler>(
		[this](ICoreWebView2Controller* sender, IUnknown* args) -> HRESULT
		{
			double zoom_factor;
			sender->get_ZoomFactor(&zoom_factor);
			//std::wstring message = L"WebView2APISample (Zoom: " +
			//    std::to_wstring(int(zoom_factor * 100)) + L"%)";
			////SetWindowText(m_app_window->GetMainWindow(), message.c_str());
			return S_OK;
		})
		.Get(),
			&m_zoom_factor_changed_token);

	ResizeWebView();
}

bool ViewComponent::HandleWindowMessage(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam, LRESULT* result)
{
	switch (message)
	{
	//! [ToggleIsVisibleOnMinimize]
	case WM_SYSCOMMAND: {
		if (wparam == SC_MINIMIZE)
		{
			// Hide the webview when the app window is minimized.
			m_controller->put_IsVisible(FALSE);
		}
		else if (wparam == SC_RESTORE)
		{
			// When the app window is restored, show the webview
			// (unless the user has toggle visibility off).
			if (m_is_visible)
				m_controller->put_IsVisible(TRUE);
		}
		break;
	}
	//! [NotifyParentWindowPositionChanged]
	case WM_MOVING:
	case WM_MOVE: {
		m_controller->NotifyParentWindowPositionChanged();
		return true;
	}
	default:
		break;
	}

	// 마우스 이벤트 수행
	if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST) ||
		message == WM_MOUSELEAVE)
	{
		OnMouseMessage(message, wparam, lparam);
	}

	//! [NotifyParentWindowPositionChanged]
	return false;
}
//! [ToggleIsVisible]
void ViewComponent::ToggleVisibility()
{
	BOOL visible;
	m_controller->get_IsVisible(&visible);
	m_is_visible = !visible;
	m_controller->put_IsVisible(m_is_visible);
}
//! [ToggleIsVisible]

void ViewComponent::SetSizeRatio(float ratio)
{
	m_webview_ratio = ratio;
	ResizeWebView();
}

void ViewComponent::SetZoomFactor(float zoom)
{
	m_webview_zoom_factor = zoom;
	m_controller->put_ZoomFactor(zoom);
}

void ViewComponent::SetBounds(RECT bounds)
{
	m_webview_bounds = bounds;
	ResizeWebView();
}

//! [SetBoundsAndZoomFactor]
void ViewComponent::SetScale(float scale)
{
	RECT bounds;
	m_controller->get_Bounds(&bounds);
	double scale_change = scale / m_webview_scale;

	bounds.bottom = LONG(
		(bounds.bottom - bounds.top) * scale_change + bounds.top);
	bounds.right = LONG(
		(bounds.right - bounds.left) * scale_change + bounds.left);

	m_webview_scale = scale;
	m_controller->SetBoundsAndZoomFactor(bounds, scale);
}
//! [SetBoundsAndZoomFactor]

//! [ResizeWebView]
// Update the bounds of the WebView window to fit available space.
void ViewComponent::ResizeWebView()
{
	SIZE webview_size = {
			LONG((m_webview_bounds.right - m_webview_bounds.left) * m_webview_ratio * m_webview_scale),
			LONG((m_webview_bounds.bottom - m_webview_bounds.top) * m_webview_ratio * m_webview_scale) };

	RECT desired_bounds = m_webview_bounds;
	desired_bounds.bottom = LONG(
		webview_size.cy + m_webview_bounds.top);
	desired_bounds.right = LONG(
		webview_size.cx + m_webview_bounds.left);

	m_controller->put_Bounds(desired_bounds);
}
//! [ResizeWebView]

// Show the current bounds of the WebView.
void ViewComponent::ShowWebViewBounds()
{
	RECT bounds;
	HRESULT result = m_controller->get_Bounds(&bounds);
	if (SUCCEEDED(result))
	{
		std::wstringstream message;
		message << L"Left:\t" << bounds.left << L"\n"
			<< L"Top:\t" << bounds.top << L"\n"
			<< L"Right:\t" << bounds.right << L"\n"
			<< L"Bottom:\t" << bounds.bottom << std::endl;
		//MessageBox(nullptr, message.str().c_str(), L"WebView Bounds", MB_OK);
	}
}

// Show the current zoom factor of the WebView.
void ViewComponent::ShowWebViewZoom()
{
	double zoom_factor;
	HRESULT result = m_controller->get_ZoomFactor(&zoom_factor);
	if (SUCCEEDED(result))
	{
		std::wstringstream message;
		message << L"Zoom Factor:\t" << zoom_factor << std::endl;
		//MessageBox(nullptr, message.str().c_str(), L"WebView Zoom Factor", MB_OK);
	}
}

void ViewComponent::SetTransform(TransformType transformType)
{
}

//! [SendMouseInput]
bool ViewComponent::OnMouseMessage(UINT message, WPARAM wparam, LPARAM lparam)
{
	// Manually relay mouse messages to the WebView
#ifdef USE_WEBVIEW2_WIN10
	if (m_dcomp_device || m_wincomp_compositor)
#else
	if (m_dcomp_device)
#endif
	{
		POINT point;
		POINTSTOPOINT(point, lparam);
		if (message == WM_MOUSEWHEEL || message == WM_MOUSEHWHEEL)
		{
			// Mouse wheel messages are delivered in screen coordinates.
			// SendMouseInput expects client coordinates for the WebView, so convert
			// the point from screen to client.
			::ScreenToClient(m_app_window->GetMainWindow(), &point);
		}
		// Send the message to the WebView if the mouse location is inside the
		// bounds of the WebView, if the message is telling the WebView the
		// mouse has left the client area, or if we are currently capturing
		// mouse events.
		bool is_mouse_in_webview = PtInRect(&m_webview_bounds, point);
		if (is_mouse_in_webview || message == WM_MOUSELEAVE || m_is_capturing_mouse)
		{
			DWORD mouse_data = 0;

			switch (message)
			{
			case WM_MOUSEWHEEL:
			case WM_MOUSEHWHEEL:
				mouse_data = GET_WHEEL_DELTA_WPARAM(wparam);
				break;
			case WM_XBUTTONDBLCLK:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
				mouse_data = GET_XBUTTON_WPARAM(wparam);
				break;
			case WM_MOUSEMOVE:
				if (!m_is_tracking_mouse)
				{
					// WebView needs to know when the mouse leaves the client area
					// so that it can dismiss hover popups. TrackMouseEvent will
					// provide a notification when the mouse leaves the client area.
					TrackMouseEvents(TME_LEAVE);
					m_is_tracking_mouse = true;
				}
				break;
			case WM_MOUSELEAVE:
				m_is_tracking_mouse = false;
				break;
			}

			// We need to capture the mouse in case the user drags the
			// mouse outside of the window bounds and we still need to send
			// mouse messages to the WebView process. This is useful for
			// scenarios like dragging the scroll bar or panning a map.
			// This is very similar to the Pointer Message case where a
			// press started inside of the WebView.
			if (message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN ||
				message == WM_RBUTTONDOWN || message == WM_XBUTTONDOWN)
			{
				if (is_mouse_in_webview && ::GetCapture() != m_app_window->GetMainWindow())
				{
					m_is_capturing_mouse = true;
					::SetCapture(m_app_window->GetMainWindow());
				}
			}
			else if (message == WM_LBUTTONUP || message == WM_MBUTTONUP ||
					 message == WM_RBUTTONUP || message == WM_XBUTTONUP)
			{
				if (::GetCapture() == m_app_window->GetMainWindow())
				{
					m_is_capturing_mouse = false;
					::ReleaseCapture();
				}
			}

			// Adjust the point from app client coordinates to webview client coordinates.
			// WM_MOUSELEAVE messages don't have a point, so don't adjust the point.
			if (message != WM_MOUSELEAVE)
			{
				point.x -= m_webview_bounds.left;
				point.y -= m_webview_bounds.top;
			}

			/*m_compositionController->SendMouseInput(
				static_cast<COREWEBVIEW2_MOUSE_EVENT_KIND>(message),
				static_cast<COREWEBVIEW2_MOUSE_EVENT_VIRTUAL_KEYS>(GET_KEYSTATE_WPARAM(wparam)),
				mouseData, point);*/
			return true;
		}
		else if (message == WM_MOUSEMOVE && m_is_tracking_mouse)
		{
			// When the mouse moves outside of the WebView, but still inside the app
			// turn off mouse tracking and send the WebView a leave event.
			m_is_tracking_mouse = false;
			TrackMouseEvents(TME_LEAVE | TME_CANCEL);
			OnMouseMessage(WM_MOUSELEAVE, 0, 0);
		}
	}
	return false;
}
//! [SendMouseInput]

bool ViewComponent::OnPointerMessage(UINT message, WPARAM wparam, LPARAM lparam)
{
	bool handled = false;
#ifdef USE_WEBVIEW2_WIN10
	if (m_dcomp_device || m_wincomp_compositor)
#else
	if (m_dcomp_device)
#endif
	{
		POINT point;
		POINTSTOPOINT(point, lparam);
		// UINT pointerId = GET_POINTERID_WPARAM(wparam);

		::ScreenToClient(m_app_window->GetMainWindow(), &point);

		// bool pointerStartedInWebView = m_pointer_ids_starting_in_webview.find(pointerId) !=  m_pointer_ids_starting_in_webview.end();

	}
	return handled;
}

void ViewComponent::TrackMouseEvents(DWORD mouse_tracking_flags)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags = mouse_tracking_flags;
	tme.hwndTrack = m_app_window->GetMainWindow();
	tme.dwHoverTime = 0;
	::TrackMouseEvent(&tme);
}

void ViewComponent::BuildDCompTreeUsingVisual()
{

	if (m_dcomp_webview_visual == nullptr)
	{
		m_dcomp_device->CreateTargetForHwnd(
			m_app_window->GetMainWindow(), TRUE, &m_dcomp_hwnd_target);
		m_dcomp_device->CreateVisual(&m_dcomp_root_visual);
		m_dcomp_hwnd_target->SetRoot(m_dcomp_root_visual.get());
		m_dcomp_device->CreateVisual(&m_dcomp_webview_visual);
		m_dcomp_root_visual->AddVisual(m_dcomp_webview_visual.get(), TRUE, nullptr);
	}
}
//! [BuildDCompTree]

void ViewComponent::DestroyDCompVisualTree()
{
	if (m_dcomp_webview_visual)
	{
		m_dcomp_webview_visual->RemoveAllVisuals();
		m_dcomp_webview_visual.reset();

		m_dcomp_root_visual->RemoveAllVisuals();
		m_dcomp_root_visual.reset();

		m_dcomp_hwnd_target->SetRoot(nullptr);
		m_dcomp_hwnd_target.reset();

		m_dcomp_device->Commit();
	}

	/*if (m_dcompTarget)
	{
		m_dcompTarget->RemoveOwnerRef();
		m_dcompTarget = nullptr;
	}*/
}

#ifdef USE_WEBVIEW2_WIN10
void ViewComponent::BuildWinCompVisualTree()
{
	namespace abiComp = ABI::Windows::UI::Composition;

	if (m_wincompWebViewVisual == nullptr)
	{
		auto interop = m_wincomp_compositor.as<abiComp::Desktop::ICompositorDesktopInterop>();
		winrt::check_hresult(interop->CreateDesktopWindowTarget(
			m_app_window->GetMainWindow(), false,
			reinterpret_cast<abiComp::Desktop::IDesktopWindowTarget**>(winrt::put_abi(m_wincompHwndTarget))));

		m_wincompRootVisual = m_wincomp_compositor.CreateContainerVisual();
		m_wincompHwndTarget.Root(m_wincompRootVisual);

		m_wincompWebViewVisual = m_wincomp_compositor.CreateContainerVisual();
		m_wincompRootVisual.Children().InsertAtTop(m_wincompWebViewVisual);
	}
}

void ViewComponent::DestroyWinCompVisualTree()
{
	if (m_wincompWebViewVisual != nullptr)
	{
		m_wincompWebViewVisual.Children().RemoveAll();
		m_wincompWebViewVisual = nullptr;

		m_wincompRootVisual.Children().RemoveAll();
		m_wincompRootVisual = nullptr;

		m_wincompHwndTarget.Root(nullptr);
		m_wincompHwndTarget = nullptr;
	}
}
#endif

ViewComponent::~ViewComponent()
{
	m_controller->remove_ZoomFactorChanged(m_zoom_factor_changed_token);
}
