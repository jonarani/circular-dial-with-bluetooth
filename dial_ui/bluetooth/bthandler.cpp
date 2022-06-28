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

    // For testing purposes
    m_timer.setInterval(50);

    connect(&m_timer, &QTimer::timeout, this, &BtHandler::sendMessage);
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

void BtHandler::addToQueue(qreal rotation)
{
    if (rotation < 0.0)
    {
        rotation = 360.0 + rotation;
    }

    m_rotations.push_back(rotation);

    if (!m_timer.isActive())
        m_timer.start();
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

void BtHandler::sendMessage()
{
    if (!m_rotations.empty())
    {
        if (m_state == CONNECTED)
        {
            qreal rotation = m_rotations.last();
            QString msg = QString::number(rotation, 'f', 1);
            qDebug() << "Sending: " << msg;
            QByteArray text = msg.toUtf8() + '\0' + '\r' + '\n';
            m_socket->write(text);
            m_rotations.clear();
        }
    }
    else if (m_timer.isActive())
    {
        m_timer.stop();
    }
}

void BtHandler::disconnect()
{
    qDebug() << "Trying to disconnect";
    if (m_socket->disconnect())
    {
        qDebug() << "Succesfully disconnected";
        setState(DISCONNECTED);
    }
}

void BtHandler::readSocket()
{
    if (m_socket == nullptr)
        return;

    // Received data must be terminated with \r\n
    while (m_socket->canReadLine()) {
        QByteArray line = m_socket->readLine();
        qDebug() << line;
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
