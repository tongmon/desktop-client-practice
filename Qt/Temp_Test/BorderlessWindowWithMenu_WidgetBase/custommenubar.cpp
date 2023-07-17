#include "custommenubar.hpp"
#include "custommenu.hpp"
#include <QEvent>
#include <QFont>
#include <QLayout>

CustomMenuBar::CustomMenuBar(QWidget *parent)
    : QWidget{parent}
{
    m_menuPopedUp = m_isEntered = false;

    m_menuLayout = new QHBoxLayout(parent);
    setLayout(m_menuLayout);
    m_menuLayout->setContentsMargins(0, 0, 0, 0);
    m_menuLayout->setSpacing(0);
    m_menuLayout->setAlignment(Qt::AlignLeft);

    QString style = R"(
        QPushButton {
            border-image: url(:/icon/Transparent.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }

        QPushButton:hover {
            background-color: rgba(255, 255, 255, 30%);
            background-repeat: no-repeat;
        }

        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 30%);
            background-repeat: no-repeat;
        }

        QPushButton::menu-indicator {
            width: 0px;
        }
    )";

    m_menuButtons["File"] = new QPushButton("File");

    auto menu = new CustomMenu(m_menuButtons["File"], m_menuButtons);
    menu->addAction("New Text File");
    menu->addAction("New File");
    menu->addAction("New Window");
    menu->addAction("Open File");
    menu->addAction("Open Directory");
    m_menuButtons["File"]->setMenu(menu);
    menu->installEventFilter(this);

    m_menuButtons["Edit"] = new QPushButton("Edit");

    menu = new CustomMenu(m_menuButtons["Edit"], m_menuButtons);
    menu->addAction("Undo");
    menu->addAction("Redo");
    menu->addAction("Cut");
    menu->addAction("Copy");
    menu->addAction("Paste");
    m_menuButtons["Edit"]->setMenu(menu);
    menu->installEventFilter(this);

    for (auto &item : m_menuButtons)
    {
        item.second->setFixedSize(35, parent->height());
        item.second->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        item.second->setStyleSheet(style);
        item.second->installEventFilter(this);
        m_menuLayout->addWidget(item.second);
    }
}

CustomMenuBar::~CustomMenuBar()
{
}

void CustomMenuBar::paintEvent(QPaintEvent *event)
{
    // setFixedSize(reinterpret_cast<QWidget *>(parent())->size());

    QWidget::paintEvent(event);
}

bool CustomMenuBar::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        qDebug() << "Mouse move check started!";

        QPushButton *menuVisibleBtn = nullptr;
        for (auto &item : m_menuButtons)
            if (item.second->menu()->isVisible())
            {
                menuVisibleBtn = item.second;
                break;
            }

        if (!menuVisibleBtn)
        {
            qDebug() << "There are no visible menu!";
            return QObject::eventFilter(obj, event);
        }

        auto mouseEvt = reinterpret_cast<QMouseEvent *>(event);
        for (auto &item : m_menuButtons)
        {
            qDebug() << "item.second->rect().contains(mouseEvt->pos()): " << item.second->rect().contains(mouseEvt->pos());
            qDebug() << "item.second->menu()->isHidden(): " << item.second->menu()->isHidden();

            // if (item.second->geometry().contains(mouseEvt->globalPos()) && item.second->menu()->isHidden())
            // if (item.second->rect().contains(mouseEvt->pos()) && item.second->menu()->isHidden())
            {
                qDebug() << "Menu should be changed!";
                menuVisibleBtn->menu()->hide();
                item.second->menu()->show();
                break;
            }
        }

        return false;
    }

    return QObject::eventFilter(obj, event);
}

// void CustomMenuBar::enterEvent(QEnterEvent *event)
//{
//     m_isEntered = true;
//     QWidget::enterEvent(event);
// }
//
// void CustomMenuBar::leaveEvent(QEvent *event)
//{
//     m_menuPopedUp = m_isEntered = false;
//     QWidget::leaveEvent(event);
// }
//
// void CustomMenuBar::mousePressEvent(QMouseEvent *event)
//{
//     if (m_isEntered && event->button() == Qt::LeftButton)
//     {
//         // 메뉴가 꺼져있으면 메뉴를 킴
//         if (!m_menuPopedUp)
//         {
//             for (auto &item : m_menuButtons)
//             {
//                 if (item.second->rect().contains(event->pos()))
//                     item.second->showMenu();
//                 else
//                     item.second->menu()->hide();
//             }
//         }
//         // 메뉴가 켜진 상태에서 한 번 더 클릭하면 메뉴 끔
//         else
//         {
//             for (auto &item : m_menuButtons)
//                 item.second->menu()->hide();
//         }
//         m_menuPopedUp = !m_menuPopedUp;
//         return;
//     }
//     QWidget::mousePressEvent(event);
// }
//
// void CustomMenuBar::mouseReleaseEvent(QMouseEvent *event)
//{
// }
//
// void CustomMenuBar::mouseMoveEvent(QMouseEvent *event)
//{
//     if (m_isEntered && m_menuPopedUp)
//     {
//         for (auto &item : m_menuButtons)
//         {
//             if (item.second->rect().contains(event->pos()))
//             {
//                 if (item.second->menu()->isHidden())
//                     item.second->showMenu();
//             }
//             else
//                 item.second->menu()->hide();
//         }
//         return;
//     }
//     QWidget::mouseMoveEvent(event);
// }