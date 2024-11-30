#ifndef COLORDOTSITEMSMODEL_H
#define COLORDOTSITEMSMODEL_H

#include <QStandardItem>
#include <QAbstractItemModel>
#include <QImage>

class ColorDotsItemsModel : public QAbstractItemModel
{
public:
    explicit ColorDotsItemsModel(QObject *parent = nullptr);
    ~ColorDotsItemsModel();


public:
    void setImage(QImage image);

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QImage image_;
    QByteArray bits_;
    int channelsCount_;
    int curChIdx_ = 0;
    QStandardItem* root_ = nullptr;

private:
    constexpr quint32 rowCol2PlainIndex(quint32 r, quint32 c, quint32 rLength) const;




};

#endif // COLORDOTSITEMSMODEL_H
