#include "btdevicemodel.h"
#include "btdevicelist.h"

BtDeviceModel::BtDeviceModel(QObject *parent)
    : QAbstractListModel(parent),
      m_list(nullptr)
{
}

int BtDeviceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_list)
        return 0;

    return m_list->foundDevices().size();
}

QVariant BtDeviceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_list)
        return QVariant();

    const DeviceItem item = m_list->foundDevices().at(index.row());

    switch (role) {
        case NameRole:
            return QVariant(item.name);
        case AddressRole:
            return QVariant(item.address);
    }

    return QVariant();
}

bool BtDeviceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_list)
        return false;

    DeviceItem item = m_list->foundDevices().at(index.row());

    switch (role) {
        case NameRole:
            item.name = value.toString();
            break;
        case AddressRole:
            item.address = value.toString();
            break;
    }

    if (m_list->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, {role});
        return true;
    }

    return false;
}

Qt::ItemFlags BtDeviceModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> BtDeviceModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[NameRole] = "name";
    names[AddressRole] = "address";

    return names;
}

BtDeviceList *BtDeviceModel::list() const
{
    return m_list;
}

void BtDeviceModel::setList(BtDeviceList *newList)
{
    beginResetModel();
    if (m_list)
        m_list->disconnect(this);

    m_list = newList;

    if (m_list) {
        connect(m_list, &BtDeviceList::preItemAppended, this, [=]() {
            const int index = m_list->foundDevices().size();
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_list, &BtDeviceList::postItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(m_list, &BtDeviceList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_list, &BtDeviceList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });
    }

    endResetModel();
}
