#include "btdevicemodel.h"

BtDeviceModel::BtDeviceModel(QObject *parent): QAbstractListModel{parent}
{

}

int BtDeviceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_foundDevices.count();
}

QVariant BtDeviceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() )
        return QVariant();

    const DeviceInfo &data = m_foundDevices.at(index.row());

    if (role == NameRole)
        return data.name;
    else if (role == NameRole)
        return data.name;
    else if (role == AddressRole)
        return data.address;
    else
        return QVariant();
}

QHash<int, QByteArray> BtDeviceModel::roleNames() const
{
    static QHash<int, QByteArray> mapping {
        {NameRole, "name"},
        {AddressRole, "address"},
    };
    return mapping;
}

void BtDeviceModel::addFoundDevice(const QString &name, const QString &address)
{
    const QModelIndex startIndex = index(0, 0);
    const QModelIndex endIndex   = index(m_foundDevices.count() - 1, 0);
    int rowOfInsert = m_foundDevices.count();

    beginInsertRows(QModelIndex(), rowOfInsert, rowOfInsert);
    m_foundDevices.append({name, address});
    endInsertRows();

    emit dataChanged(startIndex, endIndex, QVector<int>() << NameRole << AddressRole);
}

void BtDeviceModel::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    addFoundDevice(device.name(), device.address().toString());
}
