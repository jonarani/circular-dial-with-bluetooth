#include "bthandler.h"
#include <QDebug>

BtHandler::BtHandler(QObject *parent)
    : QObject{parent}
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BtHandler::newDeviceFound);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BtHandler::searchFinished);
}

void BtHandler::searchBtDevices()
{
    if (discoveryAgent == nullptr){
        qDebug() << "Error: discoveryAgent is nullptr";
        return;
    }

    qDebug() << "Started searching";
    discoveryAgent->start();
}

void BtHandler::newDeviceFound(const QBluetoothDeviceInfo &device)
{
    qDebug() << device.name();
    qDebug() << device.address().toString();
    qDebug() << "";

    emit deviceDiscovered(device);
}

void BtHandler::searchFinished()
{
    qDebug() << "Finished searching.";
}
