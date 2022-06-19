#ifndef BTDEVICEMODEL_H
#define BTDEVICEMODEL_H

#include <QAbstractListModel>

class BtDeviceList;

class BtDeviceModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(BtDeviceList *list READ list WRITE setList)
public:
    explicit BtDeviceModel(QObject *parent = nullptr);

    enum {
        NameRole = Qt::UserRole,
        AddressRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    BtDeviceList *list() const;
    void setList(BtDeviceList *newList);

private:
    BtDeviceList *m_list;
};

#endif // BTDEVICEMODEL_H
