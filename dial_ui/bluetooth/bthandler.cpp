#include "bthandler.h"
#include <QDebug>

BtHandler::BtHandler(QObject *parent)
    : QObject{parent}
{
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BtHandler::newDeviceFound);

    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BtHandler::searchFinished);
}

void BtHandler::searchBtDevices()
{
    setIsSearchFinished(false);

    if (m_discoveryAgent == nullptr){
        qDebug() << "Error: discoveryAgent is nullptr";
        return;
    }

    qDebug() << "Started searching";
    m_discoveryAgent->start();
}

void BtHandler::connectToDevice(const QString &deviceAddress)
{
    qDebug() << "Connecting to..." << deviceAddress;
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
    setIsSearchFinished(true);
}

bool BtHandler::isSearchFinished() const
{
    return m_isSearchFinished;
}

void BtHandler::setIsSearchFinished(bool newIsSearchFinished)
{
    if (m_isSearchFinished == newIsSearchFinished)
        return;
    m_isSearchFinished = newIsSearchFinished;
    emit isSearchFinishedChanged();
}
