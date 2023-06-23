#ifndef HEADER__FILE__MAINWIDGET
#define HEADER__FILE__MAINWIDGET

#include <QNetworkAccessManager>
#include <QNetworkReply>
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
    QNetworkAccessManager *netManager;
    QNetworkReply *netReply;
    QByteArray *mDataBuffer;

  private slots:
    void on_pushButton_clicked();
};

#endif /* HEADER__FILE__MAINWIDGET */
