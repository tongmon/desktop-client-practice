#ifndef HEADER__FILE__MAINWIDGET
#define HEADER__FILE__MAINWIDGET

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
};

#endif /* HEADER__FILE__MAINWIDGET */
