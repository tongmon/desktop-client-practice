#include "MainWidget.hpp"
#include "ButtonEventFilter.hpp"
#include "ui_mainwidget.h"

#include <QDebug>
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // 마우스에도 이벤트 필터를 부착한다.
    ui->pushButton->installEventFilter(new ButtonEventFilter(this));
}

MainWidget::~MainWidget()
{
    delete ui;
}

// EventFilter에 의해 클릭 이벤트가 감지되지 않으므로 실행이 안된다.
void MainWidget::on_pushButton_clicked()
{
    qDebug() << "PushButton Clicked!";
}