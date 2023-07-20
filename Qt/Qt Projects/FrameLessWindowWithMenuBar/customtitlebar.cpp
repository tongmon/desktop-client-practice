#include "customtitlebar.hpp"
#include "custommenubar.hpp"
#include "qwinwidget.h"

#include <QApplication>
#include <QDebug>
#include <regex>

CustomTitleBar::CustomTitleBar(QWidget *parent, int height)
{
    m_minBtnStyle = R"(
        QPushButton {
            border-image: url(:/icon/Minimize.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }

        QPushButton:hover {
            border-image: url(:/icon/Minimize.png);
            background-color: rgba(255, 255, 255, 20%);
            background-repeat: no-repeat;
        }

        QPushButton:pressed {
            border-image: url(:/icon/Minimize.png);
            background-color: rgba(255, 255, 255, 40%);
            background-repeat: no-repeat;
        }
    )";

    m_maxBtnStyle = R"(
        QPushButton {
            border-image: url(:/icon/Maximize.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }
        QPushButton:hover {
            border-image: url(:/icon/Maximize.png);
            background-color: rgba(255, 255, 255, 20%);
            background-repeat: no-repeat;
        }
        QPushButton:pressed {
            border-image: url(:/icon/Maximize.png);
            background-color: rgba(255, 255, 255, 40%);
            background-repeat: no-repeat;
        }
        QPushButton:checked {
            border-image: url(:/icon/Restore.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }
        QPushButton:checked:hover {
            border-image: url(:/icon/Restore.png);
            background-color: rgba(255, 255, 255, 20%);
            background-repeat: no-repeat;
        }
        QPushButton:checked:pressed {
            border-image: url(:/icon/Restore.png);
            background-color: rgba(255, 255, 255, 40%);
            background-repeat: no-repeat;
        }
    )";

    m_closeBtnStyle = R"(
        QPushButton {
            border-image: url(:/icon/Close.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }
        QPushButton:hover {
            border-image: url(:/icon/CloseHoverOrPressed.png);
            background-color: rgba(220, 41, 47, 90%);
            background-repeat: no-repeat;
        }
        QPushButton:pressed {
            border-image: url(:/icon/CloseHoverOrPressed.png);
            background-color: rgba(200, 41, 47, 60%);
            background-repeat: no-repeat;
        }
    )";

    setContentsMargins(0, 0, 0, 0);
    setFixedHeight(height);
    setLayout(new QHBoxLayout(this));

    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->setSpacing(0);

    m_titlebarLayout = new QHBoxLayout(this);
    reinterpret_cast<QHBoxLayout *>(layout())->addLayout(m_titlebarLayout);

    m_minimizeButton = new QPushButton(this);
    m_minimizeButton->setFixedSize(46, height);
    m_minimizeButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_minimizeButton->setStyleSheet(m_minBtnStyle);
    layout()->addWidget(m_minimizeButton);

    m_maximizeButton = new QPushButton(this);
    m_maximizeButton->setFixedSize(46, height);
    m_maximizeButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_maximizeButton->setCheckable(true);
    m_maximizeButton->setStyleSheet(m_maxBtnStyle);
    layout()->addWidget(m_maximizeButton);

    m_closeButton = new QPushButton(this);
    m_closeButton->setFixedSize(46, height);
    m_closeButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_closeButton->setStyleSheet(m_closeBtnStyle);
    layout()->addWidget(m_closeButton);
}

CustomTitleBar::~CustomTitleBar()
{
}

bool CustomTitleBar::isClickEventAllowedZone()
{
    // 타이틀바 필수 버튼 영역에 커서가 위치한지 검사
    if (m_minimizeButton->rect().contains(m_minimizeButton->mapFromGlobal(QCursor::pos())) ||
        m_maximizeButton->rect().contains(m_maximizeButton->mapFromGlobal(QCursor::pos())) ||
        m_closeButton->rect().contains(m_closeButton->mapFromGlobal(QCursor::pos())))
        return true;

    // 나머지 ui에 커서가 위치한지 검사
    // 일단은 메뉴만 존재
    for (int i = 0; i < m_titlebarLayout->count(); i++)
    {
        auto customMenubar = dynamic_cast<CustomMenuBar *>(m_titlebarLayout->itemAt(i)->widget());
        if (customMenubar && customMenubar->isClickEventAllowedZone())
            return true;
    }

    return false;
}

bool CustomTitleBar::event(QEvent *evt)
{
    if (evt->type() == QEvent::WindowActivate || evt->type() == QEvent::WindowDeactivate)
    {
        if (evt->type() == QEvent::WindowActivate)
        {
            m_minimizeButton->setStyleSheet(m_minBtnStyle);
            m_maximizeButton->setStyleSheet(m_maxBtnStyle);
            m_closeButton->setStyleSheet(m_closeBtnStyle);
        }
        else
        {
            m_minimizeButton->setStyleSheet(std::regex_replace(m_minBtnStyle.toStdString(), std::regex("Minimize.png"), "MinimizeDeactivated.png").c_str());
            m_closeButton->setStyleSheet(std::regex_replace(m_closeBtnStyle.toStdString(), std::regex("Close.png"), "CloseDeactivated.png").c_str());

            std::string tempStyle = std::regex_replace(m_maxBtnStyle.toStdString(), std::regex("Maximize.png"), "MaximizeDeactivated.png");
            tempStyle = std::regex_replace(tempStyle, std::regex("Restore.png"), "RestoreDeactivated.png");
            m_maximizeButton->setStyleSheet(tempStyle.c_str());
        }

        for (int i = 0; i < m_titlebarLayout->count(); i++)
            QApplication::sendEvent(m_titlebarLayout->itemAt(i)->widget(), evt);

        return true;
    }

    return QWidget::event(evt);
}

// void CustomTitleBar::changeEvent(QEvent *evt)
// {
//     if (evt->type() == QEvent::WindowActivate)
//     {
//         qDebug() << "Activate";
//     }
//
//     if (evt->type() == QEvent::WindowDeactivate)
//     {
//         qDebug() << "Deactivate";
//     }
//
//     if (evt->type() == QEvent::ActivationChange)
//     {
//         CustomMenuBar *customMenubar = nullptr;
//         for (int i = 0; i < m_titlebarLayout->count() && !customMenubar; i++)
//             customMenubar = dynamic_cast<CustomMenuBar *>(m_titlebarLayout->itemAt(i)->widget());
//
//         // 메뉴가 켜지는 경우에도 top window 변경 메시지가 전달되기에 메뉴가 켜지는 경우에는 ActivationChange 메시지를 무시한다.
//         if (!customMenubar || !customMenubar->IsMenuOn())
//         {
//             for (int i = 0; i < m_titlebarLayout->count(); i++)
//                 QApplication::sendEvent(m_titlebarLayout->itemAt(i)->widget(), evt);
//         }
//     }
// }

void CustomTitleBar::addWidget(QWidget *widget, int stretch, Qt::Alignment alignment)
{
    m_titlebarLayout->addWidget(widget, stretch, alignment);
}
