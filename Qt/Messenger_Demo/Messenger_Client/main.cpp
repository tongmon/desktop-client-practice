#include "WinQuickWindow.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>
#include <soci/postgresql/soci-postgresql.h>
#include <soci/soci.h>

int main(int argc, char *argv[])
{
    // MSVC에서 soci::postgresql 사용시 버그로 인해 컴파일이 안됨.
    // soci::postgresql -> *soci::factory_postgresql()로 사용해야 함
    soci::session sql(*soci::factory_postgresql(), "host=127.0.0.1 port=3000 dbname=MESSENGER_DB user=tongstar password=@Lsy12131213");

    int count;
    sql << "select count(*) from user_tb", soci::into(count);

    // Qt::AA_UseSoftwareOpenGL, Qt::AA_UseDesktopOpenGL, Qt::AA_UseOpenGLES 등 많다.
    QGuiApplication::setAttribute(Qt::AA_UseOpenGLES);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Qt Quick 렌더링 옵션 조정
    QSurfaceFormat format;
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer); // 화면 깜빡이지 않게 더블 버퍼링 활성화
    format.setSwapInterval(0);                            // 수직동기화를 0으로 끄지 않으면 윈도우 이동시 버벅거린다.
    QSurfaceFormat::setDefaultFormat(format);

    QGuiApplication app(argc, argv);

    // 이 시점에 native event filter를 적용해주는 것이 중요하다.
    // QQuickWindow가 생성된 이후에 native event filter가 적용되면 윈도우를 frameless로 만들기 어렵다.
    WinQuickWindow win_quick_window;
    app.installNativeEventFilter(&win_quick_window);

    // 아이콘 설정
    app.setWindowIcon(QIcon(":/icon/ApplicationIcon.png"));

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/qml/MainWindow.qml"));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [&](QObject *obj, const QUrl &objUrl) {
            if ((!obj && url == objUrl) ||
                !win_quick_window.InitWindow(engine))
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
