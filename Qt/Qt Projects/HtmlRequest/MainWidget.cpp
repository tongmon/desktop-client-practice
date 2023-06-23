#include "MainWidget.hpp"
#include "ui_mainwidget.h"
#include <QDebug>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    netManager = new QNetworkAccessManager(this);
    netReply = nullptr;
    mDataBuffer = new QByteArray();
}

MainWidget::~MainWidget()
{
    delete ui;
    delete mDataBuffer;
}

void MainWidget::on_pushButton_clicked()
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://www.google.com/")); // https를 처리하려면 Qt 소스를 빌드할 때 OpenSSL이 설치되어 있어야 한다.

    netReply = netManager->get(request);

    // 리퀘스트 결과를 읽을 준비가 readyRead 시그널이 발생한다.
    // 슬롯 람다 함수에서는 결과를 읽어서 mDataBuffer에 집어넣는다.
    connect(netReply, &QIODevice::readyRead, [this]() -> void {
        mDataBuffer->append(netReply->readAll());
    });

    // 리퀘스트 처리가 완전히 완료되면 finished 시그널이 발생한다.
    // 슬롯 람다 함수에서는 text edit에 결과값을 넣는다.
    connect(netReply, &QNetworkReply::finished, [this]() -> void {
        if (netReply->error())
        {
            qDebug() << netReply->errorString();
        }
        else
        {
            ui->textEdit->setPlainText(QString(*mDataBuffer));
        }
    });
}