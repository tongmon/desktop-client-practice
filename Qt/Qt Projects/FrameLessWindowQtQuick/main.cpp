#include "WinQuickWindow.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <memory>

int main(int argc, char *argv[])
{
    // HiDPI 지원 디스플레이에서 HiDPI 스케일링 사용
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // https://bugreports.qt.io/browse/QTBUG-44486#comment-327410
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    std::unique_ptr<WinQuickWindow> quick_window(nullptr);

    const QUrl main_url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [&](QObject *obj, const QUrl &obj_url) {
            if (!obj && main_url == obj_url)
                QCoreApplication::exit(-1);
            else
            {
                quick_window = std::make_unique<WinQuickWindow>(*qobject_cast<QQuickWindow *>(engine.rootObjects().at(0)), engine);
                app.installNativeEventFilter(quick_window.get());
            }
        },
        Qt::QueuedConnection);
    engine.load(main_url);

    return app.exec();
}
