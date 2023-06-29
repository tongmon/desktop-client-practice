#include "MainWidget.hpp"
#include "ui_mainwidget.h"

#include <QDebug>
#include <QMouseEvent>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_pushButton_clicked()
{
    // 누르기 이벤트
    QMouseEvent *mousePressEvt = new QMouseEvent(QEvent::MouseButtonPress, // 발생시킬 마우스 이벤트 유형
                                                 QPointF(1, 1),            // 마우스 이벤트 발생 위치
                                                 Qt::LeftButton,           // 어떤 마우스 버튼이 눌리는 이벤트인지
                                                 Qt::LeftButton,           // 여러 개의 마우스 버튼이 동시에 눌리는 경우를 위해 존재
                                                 Qt::NoModifier);          // 마우스 버튼과 키보드의 특정 키가 같이 눌리는 경우를 위해 존재

    // 떼기 이벤트
    QMouseEvent *mouseReleaseEvt = new QMouseEvent(QEvent::MouseButtonRelease,
                                                   QPointF(1, 1),
                                                   Qt::LeftButton,
                                                   Qt::LeftButton,
                                                   Qt::NoModifier);

    // 마우스 누르기
    if (QApplication::sendEvent(ui->cornerButton, mousePressEvt))
        qDebug() << "Send Event Success!";
    else
        qDebug() << "Send Event Fail!";

    // 마우스 떼기
    if (QApplication::sendEvent(ui->cornerButton, mouseReleaseEvt))
        qDebug() << "Send Event Success!";
    else
        qDebug() << "Send Event Fail!";

    // 밑과 같이 이벤트 큐에 바로 넣지 않고 바로 해당 ui로 직접 이벤트를 던져주는 post 기능을 이용할 수도 있다.
    // QApplication::postEvent(ui->pushButton, mousePressEvt);
}

void MainWidget::on_cornerButton_clicked()
{
    static int i = 1;
    ui->cornerButton->setText(QString("Button Clicked: %0").arg(i++));
}