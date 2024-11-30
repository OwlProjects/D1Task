#ifndef COLORITEMMODEL_H
#define COLORITEMMODEL_H

#include <QAbstractItemModel>
#include <QImage>


class ColorItemModel: public QAbstractListModel
{
public:
    explicit ColorItemModel(QObject *parent = nullptr);

public:
    void setImage(QImage image);

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;



private:
    QImage image_;
    QByteArray bits_;
    int channelsCount_;

private:
    constexpr quint32 rowCol2PlainIndex(quint32 r, quint32 c, quint32 rLength) const;


    // QAbstractItemModel interface
public:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

#endif // COLORITEMMODEL_H
