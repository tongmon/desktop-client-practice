#include "MainWindow.hpp"

#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <cstdlib>
#include <ctime>
#include <dwmapi.h>
#include <memory>
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_minimize_btn{nullptr}, m_maximize_btn{nullptr}, m_close_btn{nullptr}
{
    m_hwnd = (HWND)winId();
    m_resize_border_width = 6;

    // 윈도우 그림자 설정
    const MARGINS aero_shadow_on = {1, 1, 1, 1};
    ::DwmExtendFrameIntoClientArea(m_hwnd, &aero_shadow_on);

    // 아이콘 설정
    setWindowIcon(QIcon(":/icon/ApplicationIcon.png"));

    setGeometry(100, 100, 1024, 768);
    setMinimumSize(400, 300);

    QObject::connect(windowHandle(), &QWindow::screenChanged, this, &MainWindow::OnScreenChanged);

    // 메인 윈도우라 위젯을 먼저 추가함
    QWidget *entire_widget = new QWidget(this);
    entire_widget->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(entire_widget);

    // 메인 윈도우 색상 설정
    QPalette Pal(entire_widget->palette());
    Pal.setColor(QPalette::Background, QColor(30, 34, 39));
    entire_widget->setAutoFillBackground(true);
    entire_widget->setPalette(Pal);

    // 메인 윈도우에 수직 레이아웃 추가
    QVBoxLayout *entire_layout = new QVBoxLayout(this);
    entire_widget->setLayout(entire_layout);
    entire_layout->setContentsMargins(0, 0, 0, 0);
    entire_layout->setSpacing(0);

    // 타이틀바 레이아웃 설정
    QHBoxLayout *titlebar_layout = new QHBoxLayout(this);
    entire_layout->addLayout(titlebar_layout);
    titlebar_layout->setContentsMargins(0, 0, 0, 0);
    titlebar_layout->setSpacing(0);

    // 타이틀바에 메뉴바 추가
    QMenuBar *menubar = new QMenuBar(this);
    titlebar_layout->addWidget(menubar);
    menubar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_activable_widgets.push_back(menubar);
    menubar->setStyleSheet(R"(
        QMenuBar {
            border-image: url(:/icon/Transparent.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }
        QMenuBar::item {
            color: rgb(152, 160, 175);
        }
        QMenuBar::item[active="false"] {
            color: rgb(112, 120, 135);
        }
        QMenuBar::item:selected {
            spacing: 3px;           
            padding: 2px 10px;
            background-color: rgb(52, 56, 61);
            border-radius: 5px;
        }

        QMenu {
             color: rgb(152, 160, 175);
             background-color: rgb(30, 34, 39);
        }
        QMenu::item:selected {
            background-color: rgb(52, 56, 61);
        }
    )");

    QMenu *menu = menubar->addMenu("File");
    menu->addAction("New Text File");
    menu->addAction("New File");
    menu->addAction("New Window");
    menu->addAction("Open File");
    menu->addAction("Open Directory");

    menu = menubar->addMenu("Edit");
    menu->addAction("Undo");
    menu->addAction("Redo");
    menu->addAction("Cut");
    menu->addAction("Copy");
    menu->addAction("Paste");

    menu = menubar->addMenu("Selected Zone");
    menu->addAction("Select All");
    menu->addAction("Expand Select Zone");
    menu->addAction("Collapse Select Zone");
    menu->addAction("Add Cursor");
    menu->addAction("Select Line Mode");

    menu = menubar->addMenu("View");
    menu->addAction("Command Pallete");
    menu->addAction("Open View");
    menu->addAction("Shape");
    menu->addAction("Edit Layout");

    QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    titlebar_layout->addSpacerItem(spacer);
    m_movable_area.push_back(spacer);

    m_minimize_btn = new QPushButton(this);
    titlebar_layout->addWidget(m_minimize_btn);
    m_minimize_btn->setFixedSize(46, 35);
    m_minimize_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_minimize_btn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/icon/Minimize.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }

        QPushButton:hover {
            background-color: rgba(255, 255, 255, 20%);
        }

        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 40%);
        }
      
        QPushButton:!active {
            border-image: url(:/icon/MinimizeDeactivated.png);
        }
    )");

    m_maximize_btn = new QPushButton(this);
    titlebar_layout->addWidget(m_maximize_btn);
    m_maximize_btn->setFixedSize(46, 35);
    m_maximize_btn->setCheckable(true);
    m_maximize_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_maximize_btn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/icon/Maximize.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 20%);
        }
        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 40%);
        }
        QPushButton:checked {
            border-image: url(:/icon/Restore.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }
        QPushButton:checked:hover {
            background-color: rgba(255, 255, 255, 20%);
        }
        QPushButton:checked:pressed {
            background-color: rgba(255, 255, 255, 40%);
        }
        QPushButton:!active {
            border-image: url(:/icon/MaximizeDeactivated.png);
        }
        QPushButton:checked:!active {
            border-image: url(:/icon/RestoreDeactivated.png);
        }
    )");

    m_close_btn = new QPushButton(this);
    titlebar_layout->addWidget(m_close_btn);
    m_close_btn->setFixedSize(46, 35);
    m_close_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_close_btn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/icon/Close.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }
        QPushButton:hover {
            border-image: url(:/icon/CloseHoverOrPressed.png);
            background-color: rgba(220, 41, 47, 90%);
        }
        QPushButton:pressed {
            border-image: url(:/icon/CloseHoverOrPressed.png);
            background-color: rgba(200, 41, 47, 60%);
        }
        QPushButton:!active {
            border-image: url(:/icon/CloseDeactivated.png);
        }
        QPushButton:hover:!active {
            border-image: url(:/icon/CloseHoverOrPressed.png);
        }
    )");

    QObject::connect(m_minimize_btn, &QPushButton::clicked, this, &MainWindow::OnMinimizeButtonClicked);
    QObject::connect(m_maximize_btn, &QPushButton::clicked, this, &MainWindow::OnMaximizeButtonClicked);
    QObject::connect(m_close_btn, &QPushButton::clicked, this, &MainWindow::OnCloseButtonClicked);

    entire_layout->setAlignment(titlebar_layout, Qt::AlignTop);

    QWidget *main_widget = new QWidget(this);
    entire_layout->addWidget(main_widget);
    main_widget->setContentsMargins(0, 0, 0, 0);
    main_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 메인 윈도우 색상 결정
    Pal = main_widget->palette();
    Pal.setColor(QPalette::Background, QColor(35, 39, 46));
    main_widget->setAutoFillBackground(true);
    main_widget->setPalette(Pal);

    // 메인 윈도우에 수직 레이아웃 추가
    QVBoxLayout *main_widget_layout = new QVBoxLayout(this);
    main_widget->setLayout(main_widget_layout);

    // 버튼 추가
    QPushButton *btn = new QPushButton("This is Button!", this);
    btn->setFixedSize(180, 35);
    main_widget_layout->addWidget(btn);
    main_widget_layout->setAlignment(btn, Qt::AlignHCenter);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::nativeEvent(const QByteArray &event_type, void *message, long *result)
{
    MSG *msg = (MSG *)message;

    switch (msg->message)
    {
    case WM_NCCALCSIZE: {
        if (msg->lParam)
        {
            WINDOWPLACEMENT wp;
            GetWindowPlacement(m_hwnd, &wp);

            if (wp.showCmd == SW_MAXIMIZE)
            {
                NCCALCSIZE_PARAMS *sz = (NCCALCSIZE_PARAMS *)msg->lParam;
                sz->rgrc[0].left += 8;
                sz->rgrc[0].top += 8;
                sz->rgrc[0].right -= 8;
                sz->rgrc[0].bottom -= 8;
            }
        }
        return true;
    }
    case WM_NCHITTEST: {
        RECT winrect;
        GetWindowRect(msg->hwnd, &winrect);
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);
        long local_x = x - winrect.left;
        long local_y = y - winrect.top;

        if (x >= winrect.left && x < winrect.left + m_resize_border_width &&
            y < winrect.bottom && y >= winrect.bottom - m_resize_border_width)
        {
            *result = HTBOTTOMLEFT;
            return true;
        }

        if (x < winrect.right && x >= winrect.right - m_resize_border_width &&
            y < winrect.bottom && y >= winrect.bottom - m_resize_border_width)
        {
            *result = HTBOTTOMRIGHT;
            return true;
        }

        if (x >= winrect.left && x < winrect.left + m_resize_border_width &&
            y >= winrect.top && y < winrect.top + m_resize_border_width)
        {
            *result = HTTOPLEFT;
            return true;
        }

        if (x < winrect.right && x >= winrect.right - m_resize_border_width &&
            y >= winrect.top && y < winrect.top + m_resize_border_width)
        {
            *result = HTTOPRIGHT;
            return true;
        }

        if (x >= winrect.left && x < winrect.left + m_resize_border_width)
        {
            *result = HTLEFT;
            return true;
        }

        if (x < winrect.right && x >= winrect.right - m_resize_border_width)
        {
            *result = HTRIGHT;
            return true;
        }

        if (y < winrect.bottom && y >= winrect.bottom - m_resize_border_width)
        {
            *result = HTBOTTOM;
            return true;
        }

        if (y >= winrect.top && y < winrect.top + m_resize_border_width)
        {
            *result = HTTOP;
            return true;
        }

        // 클릭하여 이동할 수 있는 영역 검사
        for (const auto &item : m_movable_area)
        {
            if (item->geometry().contains(local_x, local_y))
            {
                *result = HTCAPTION;
                return true;
            }
        }

        *result = HTTRANSPARENT;
        break;
    }
    case WM_SIZE: {
        if (m_maximize_btn)
        {
            WINDOWPLACEMENT wp;
            GetWindowPlacement(m_hwnd, &wp);
            m_maximize_btn->setChecked(wp.showCmd == SW_MAXIMIZE ? true : false);
        }
        break;
    }
    default:
        break;
    }

    return false;
}

bool MainWindow::event(QEvent *evt)
{
    switch (evt->type())
    {
    case QEvent::WindowActivate: {
        for (auto &item : m_activable_widgets)
        {
            item->setProperty("active", true);
            item->setStyleSheet(item->styleSheet());
        }
        break;
    }

    case QEvent::WindowDeactivate: {
        for (auto &item : m_activable_widgets)
        {
            item->setProperty("active", false);
            item->setStyleSheet(item->styleSheet());
        }
        break;
    }

    default:
        break;
    }

    return QMainWindow::event(evt);
}

void MainWindow::OnScreenChanged(QScreen *screen)
{
    SetWindowPos(m_hwnd, NULL, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE);
}

void MainWindow::OnMinimizeButtonClicked()
{
    SendMessage(m_hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void MainWindow::OnMaximizeButtonClicked()
{
    SendMessage(m_hwnd, WM_SYSCOMMAND, m_maximize_btn->isChecked() ? SC_MAXIMIZE : SC_RESTORE, 0);

    // hover 상태 남아있는 현상 제거
    m_maximize_btn->setAttribute(Qt::WA_UnderMouse, false);
}

void MainWindow::OnCloseButtonClicked()
{
    SendMessage(m_hwnd, WM_CLOSE, 0, 0);
}
