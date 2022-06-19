#ifndef BTHANDLER_H
#define BTHANDLER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothAddress>
#include <QBluetoothUuid>

#include "btdevicemodel.h"

class BtHandler : public QObject
{
    Q_OBJECT
public:
    explicit BtHandler(QObject *parent = nullptr, BtDeviceModel *btDeviceModel = nullptr);

    Q_INVOKABLE void searchBtDevices();

signals:

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = nullptr;
    BtDeviceModel *m_btDeviceModel = nullptr;

    struct DeviceInfo {
        DeviceInfo(){}
        DeviceInfo(const QString &name, const QString &address)
            : name(name), address(address){}
        QString name;
        QString address;
    };

    QVector<DeviceInfo> m_foundDevices;

public slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void searchFinished();

};

#endif // BTHANDLER_H
