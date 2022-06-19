#include "bthandler.h"
#include <QDebug>

BtHandler::BtHandler(QObject *parent, BtDeviceModel *btDeviceModel)
    : QObject{parent}
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    m_btDeviceModel = btDeviceModel;

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BtHandler::deviceDiscovered);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            btDeviceModel, &BtDeviceModel::deviceDiscovered);

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

void BtHandler::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << device.name();
    qDebug() << device.address().toString();
    qDebug() << "";
    m_foundDevices.append({device.name(), device.address().toString()});
}

void BtHandler::searchFinished()
{
    qDebug() << "Finished searching.";
}
