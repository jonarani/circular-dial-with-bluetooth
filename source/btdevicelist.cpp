#include "btdevicelist.h"
#include <QBluetoothAddress>

BtDeviceList::BtDeviceList(QObject *parent)
    : QObject{parent}
{
    // Test data for the list view
//    m_foundDevices.append({QString("name 1"), QString("addr 1")});
//    m_foundDevices.append({QString("name 2"), QString("addr 2")});
//    m_foundDevices.append({QString("name 3"), QString("addr 3")});
}

QVector<DeviceItem> BtDeviceList::foundDevices() const
{
    return m_foundDevices;
}

// Necessary when want to modify data. In this case not necessary
// Returns bool whether or not it was sucesfully set
bool BtDeviceList::setItemAt(int index, const DeviceItem &item)
{
    if (index < 0 || index > m_foundDevices.size())
        return false;

    const DeviceItem &oldItem = m_foundDevices.at(index);
    if (item.name == oldItem.name && item.address == oldItem.address)
        return false;

    m_foundDevices[index] = item;

    return true;
}

void BtDeviceList::appendItem(const QBluetoothDeviceInfo &device)
{
    emit preItemAppended();

    DeviceItem item;
    item.name = device.name();
    item.address = device.address().toString();

    m_foundDevices.append(item);

    emit postItemAppended();
}

void BtDeviceList::removeItems()
{
    for (int i = 0; i < m_foundDevices.size(); ) {
        emit preItemRemoved(i);
        m_foundDevices.removeAt(i);
        emit postItemRemoved();
    }
}
