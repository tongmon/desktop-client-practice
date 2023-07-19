#include <QApplication>
#include <QGoodCentralWidget>
#include <QGoodWindow>

int main(int argc, char *argv[])
{
    QGoodWindow::setup();

    QApplication app(argc, argv);

    QGoodWindow gw;
    QGoodCentralWidget gcw(&gw);
    gw.setCentralWidget(&gcw);
    gw.show();

    return app.exec();
}
