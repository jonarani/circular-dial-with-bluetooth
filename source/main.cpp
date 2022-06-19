#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "bluetooth/bthandler.h"
#include "bluetooth/btdevicelist.h"
#include "bluetooth/btdevicemodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<BtDeviceModel>("BtDevice", 1, 0, "BtDeviceModel");
    // Can be used as property in qml model
    qmlRegisterUncreatableType<BtDeviceList>("BtDevice", 1, 0, "BtDeviceList",
                                             "BtDeviceList should not be created in QML");

    BtHandler btHandler;
    BtDeviceList deviceList;

    QObject::connect(&btHandler, &BtHandler::deviceDiscovered, &deviceList, &BtDeviceList::appendItem);

    QQmlApplicationEngine engine;


    engine.rootContext()->setContextProperty("_btHandler", &btHandler);
    engine.rootContext()->setContextProperty("_deviceList", &deviceList);

    const QUrl url(QStringLiteral("qrc:/UI/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
