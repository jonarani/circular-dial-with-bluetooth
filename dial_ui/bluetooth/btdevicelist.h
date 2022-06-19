#ifndef BTDEVICELIST_H
#define BTDEVICELIST_H

#include <QObject>
#include <QVector>
#include <QBluetoothDeviceInfo>

struct DeviceItem {
    DeviceItem(){}
    DeviceItem(QString name, QString address)
        : name(name), address(address) {}
    QString name;
    QString address;
};

class BtDeviceList : public QObject
{
    Q_OBJECT
public:
    explicit BtDeviceList(QObject *parent = nullptr);

    Q_INVOKABLE const QString getItemAddressAt(int index);

    QVector<DeviceItem> foundDevices() const;

    // Necessary when want to modify the data
    // In this case it is not necessary
    bool setItemAt(int index, const DeviceItem &item);


signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem(const QBluetoothDeviceInfo &device);
    void removeItems();

private:
    QVector<DeviceItem> m_foundDevices;

};

#endif // BTDEVICELIST_H
