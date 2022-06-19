#ifndef BTDEVICEMODEL_H
#define BTDEVICEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QBluetoothDeviceInfo>
#include <QBluetoothAddress>

class BtDeviceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BtDeviceModel(QObject *parent = nullptr);

    enum Roles {
        NameRole = Qt::UserRole,
        AddressRole,
    };

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    struct DeviceInfo {
        DeviceInfo(){}
        DeviceInfo(const QString &name, const QString &address)
            : name(name), address(address){}
        QString name;
        QString address;
    };

    QVector<DeviceInfo> m_foundDevices;

    void addFoundDevice(const QString &name, const QString &address);

public slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
};

#endif // BTDEVICEMODEL_H
