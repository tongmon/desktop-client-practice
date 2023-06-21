#include "MainWidget.hpp"
#include "CustomTableModel.hpp"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    CustomTableModel *model = new CustomTableModel(this);
    ui->tableView->setModel(model);
}

MainWidget::~MainWidget()
{
    delete ui;
}