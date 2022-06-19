#ifndef BTHANDLER_H
#define BTHANDLER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothAddress>
#include <QBluetoothUuid>

class BtHandler : public QObject
{
    Q_OBJECT
public:
    explicit BtHandler(QObject *parent = nullptr);
    Q_INVOKABLE void searchBtDevices();

signals:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);

public slots:
    void newDeviceFound(const QBluetoothDeviceInfo &device);
    void searchFinished();

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = nullptr;
};

#endif // BTHANDLER_H
