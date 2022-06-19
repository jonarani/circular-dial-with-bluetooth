#ifndef BTHANDLER_H
#define BTHANDLER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothAddress>
#include <QBluetoothUuid>

class BtHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isSearchFinished READ isSearchFinished WRITE setIsSearchFinished NOTIFY isSearchFinishedChanged)
public:
    explicit BtHandler(QObject *parent = nullptr);
    Q_INVOKABLE void searchBtDevices();
    Q_INVOKABLE void connectToDevice(const QString &deviceAddress);

    bool isSearchFinished() const;
    void setIsSearchFinished(bool newIsSearchFinished);

signals:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);

    void isSearchFinishedChanged();

public slots:
    void newDeviceFound(const QBluetoothDeviceInfo &device);
    void searchFinished();

private:
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent = nullptr;

    bool m_isSearchFinished = false;
};

#endif // BTHANDLER_H
