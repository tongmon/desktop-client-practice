#ifndef HEADER__FILE__MAINWIDGET
#define HEADER__FILE__MAINWIDGET

#include <QFileSystemModel>
#include <QWidget>

namespace Ui
{
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

  private:
    Ui::MainWidget *ui;
    QFileSystemModel *filesModel;
    QFileSystemModel *dirModel;

  private slots:
    void on_treeView_clicked(const QModelIndex &);
};

#endif /* HEADER__FILE__MAINWIDGET */
