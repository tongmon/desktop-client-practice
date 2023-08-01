#ifndef HEADER__FILE__WINNATIVEWINDOW
#define HEADER__FILE__WINNATIVEWINDOW

#include <QQuickWindow>
#include <Windows.h>
#include <Windowsx.h>

class WinQuickWindow;

class WinNativeWindow
{
    DWORD aero_borderless = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN;

    HWND m_hwnd;

  public:
    struct SizeType
    {
        SizeType()
            : required(false), width(0), height(0)
        {
        }
        bool required;
        int width;
        int height;
    };

    WinNativeWindow(const int x, const int y, const int width, const int height);
    ~WinNativeWindow();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HWND GetHandle()
    {
        return m_hwnd;
    }

    // These six functions exist to restrict native window resizing to whatever you want your app minimum/maximum size to be
    void SetMinimumSize(const int width, const int height);
    int GetMinimumHeight();
    int GetMinimumWidth();
    SizeType GetMinimunSizeInfo();

    void SetMaximumSize(const int width, const int height);
    int GetMaximumHeight();
    int GetMaximumWidth();
    SizeType GetMaximunSizeInfo();

    void SetGeometry(const int x, const int y, const int width, const int height);

    inline static WinQuickWindow *quick_window = nullptr;
    inline static HWND child_hwnd = nullptr;
    inline static QQuickWindow *child_window = nullptr;

  private:
    SizeType m_minimum_size;
    SizeType m_maximum_size;
};

#endif /* HEADER__FILE__WINNATIVEWINDOW */
