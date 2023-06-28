#include "MainWidget.hpp"
#include "CustomButton.hpp"
#include "ui_mainwidget.h"

#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    auto *customBtn = new CustomButton("CustomButton", this);

    ui->horizontalLayout->addWidget(customBtn);
}

MainWidget::~MainWidget()
{
    delete ui;
}