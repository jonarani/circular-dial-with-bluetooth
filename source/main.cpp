#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "bthandler.h"
#include "btdevicemodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    BtDeviceModel btDeviceModel;
    BtHandler btHandler(nullptr, &btDeviceModel);
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("_btHandler", &btHandler);
    engine.rootContext()->setContextProperty("_btDeviceModel", &btDeviceModel);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
