#include "bthandler.h"
#include <QDebug>

// When running on PC
static const QString serviceUuid("{00000000-0000-0000-0000-000000000000}");
// When running on Android
//static const QString serviceUuid("00001101-0000-1000-8000-00805F9B34FB");

BtHandler::BtHandler(QObject *parent)
    : QObject{parent},
        m_state(IDLE)
{
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BtHandler::newDeviceFound);

    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BtHandler::searchFinished);
}

void BtHandler::searchBtDevices()
{
    setState(SEARCHING);
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
    setState(CONNECTING);
    qDebug() << "Connecting to..." << deviceAddress;

    if (m_socket == nullptr) {
        m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

        connect(m_socket, &QBluetoothSocket::readyRead, this, &BtHandler::readSocket);
        connect(m_socket, &QBluetoothSocket::connected, this, &BtHandler::connected);
        connect(m_socket, &QBluetoothSocket::disconnected, this, &BtHandler::disconnected);
        connect(m_socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
                this, &BtHandler::socketErrorOccurred);
    }

    QBluetoothUuid uuid(serviceUuid);
    QBluetoothAddress address(deviceAddress);

    m_socket->connectToService(address, uuid);
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
    setState(IDLE_SEARCH_FINISHED);
    qDebug() << "Finished searching.";
    setIsSearchFinished(true);
}

void BtHandler::sendMessage(const qreal rotation)
{
    QString message("Test message");
    // TODO: convert rotation into QString
    qDebug() << message;
    //QByteArray text = message.toUtf8() + '\r' + '\n';
    //m_socket->write(text);
}

void BtHandler::readSocket()
{
    if (m_socket == nullptr)
        return;

    // Received data must be terminated with \r\n
    while (m_socket->canReadLine()) {
        QByteArray line = m_socket->readLine();

    }
}

void BtHandler::connected()
{
    setState(CONNECTED);
    qDebug() << "Connected";
}

void BtHandler::disconnected()
{
    setState(DISCONNECTED);
    qDebug() << "Disconnected";
}

void BtHandler::socketErrorOccurred(QBluetoothSocket::SocketError error)
{
    setState(ERROR_OCCURED);
    qDebug() << "Socket error: " << error;
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

BtHandler::BtStates BtHandler::state() const
{
    return m_state;
}

void BtHandler::setState(BtStates newState)
{
    if (m_state == newState)
        return;
    m_state = newState;
    emit stateChanged();
}
