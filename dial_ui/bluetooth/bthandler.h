#ifndef BTHANDLER_H
#define BTHANDLER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothAddress>
#include <QBluetoothUuid>
#include <QBluetoothSocket>
#include <QTimer>

class BtHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isSearchFinished READ isSearchFinished WRITE setIsSearchFinished NOTIFY isSearchFinishedChanged)
    Q_PROPERTY(BtStates state READ state WRITE setState NOTIFY stateChanged)
public:
    explicit BtHandler(QObject *parent = nullptr);

    enum BtStates {
        IDLE = 0,
        SEARCHING,
        IDLE_SEARCH_FINISHED,
        CONNECTING,
        CONNECTED,
        DISCONNECTED,
        ERROR_OCCURED,
    };
    Q_ENUM(BtStates)

    Q_INVOKABLE void searchBtDevices();
    Q_INVOKABLE void connectToDevice(const QString &deviceAddress);
    Q_INVOKABLE void addToQueue(qreal rotation);
    Q_INVOKABLE void disconnect();

    bool isSearchFinished() const;
    void setIsSearchFinished(bool newIsSearchFinished);

    BtStates state() const;
    void setState(BtStates newState);

signals:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);

    void isSearchFinishedChanged();

    void stateChanged();

public slots:
    void newDeviceFound(const QBluetoothDeviceInfo &device);
    void searchFinished();

private slots:
    void readSocket();
    void connected();
    void disconnected();
    void socketErrorOccurred(QBluetoothSocket::SocketError error);
    void sendMessage();

private:
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent = nullptr;
    QBluetoothSocket *m_socket = nullptr;

    bool m_isSearchFinished = false;

    BtStates m_state;

    QVector<qreal> m_rotations;

    QTimer m_timer;
};

#endif // BTHANDLER_H
