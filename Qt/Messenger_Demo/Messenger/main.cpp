#include "LoginPageContext.hpp"
#include "WinQuickWindow.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlProperty>
#include <QQuickWindow>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
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
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
            else
            {
                if (win_quick_window.SetQuickWindow(qobject_cast<QQuickWindow *>(engine.rootObjects().at(0))))
                {
                    // qml 창에 대한 이벤트 핸들러 등록
                    engine.installEventFilter(&win_quick_window);

                    // qml에 mainWindowContext 객체 등록, 해당 객체에 minimize, maximize / restore, close 기능 연결되어 있음
                    engine.rootContext()->setContextProperty("mainWindowContext", &win_quick_window);

                    // LoginPage 관련 Context 함수 등록
                    std::shared_ptr<LoginPageContext> lpc(new LoginPageContext);
                    engine.rootContext()->setContextProperty("loginPageContext", lpc.get());
                }
            }
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
