#include "ColorItemModel.h"

#include <QByteArray>


ColorItemModel::ColorItemModel(QObject *parent):
    QAbstractListModel(parent)
{}

void ColorItemModel::setImage(QImage image)
{
    beginResetModel();
    image_ = image;
    bits_ = QByteArray(reinterpret_cast<char*>(image_.bits()), image_.sizeInBytes());
    channelsCount_ = image_.hasAlphaChannel() ? 4 : 3;
    endResetModel();
}

int ColorItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return bits_.size() / channelsCount_;
}

int ColorItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return channelsCount_;
}

QVariant ColorItemModel::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole: {
        int r = index.row();
        int c = index.column();
        int dataIndex = rowCol2PlainIndex(r, c, columnCount(index));
        if (dataIndex < bits_.size()) {
            return QString::number(static_cast<uchar>(bits_[dataIndex]), 16).toUpper();
        }
        break;
    }
    case Qt::BackgroundRole: {
        int r = index.row();
        int c = 0;
        int dataIndex = rowCol2PlainIndex(r, c, columnCount(index));
        if (dataIndex < bits_.size()) {
            QColor result = QColor( static_cast<uchar>(bits_[dataIndex + 0]),
                                    static_cast<uchar>(bits_[dataIndex + 1]),
                                    static_cast<uchar>(bits_[dataIndex + 2]),
                                    static_cast<uchar>(bits_[dataIndex + 3]));
            if (result.isValid())
                return result;
        }
        break;
    }
    }
    return QVariant();
}

Qt::ItemFlags ColorItemModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

constexpr quint32 ColorItemModel::rowCol2PlainIndex(quint32 r, quint32 c, quint32 rLength) const
{
    return c + r * rLength;
}

QVariant ColorItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return "Red"; break;
            case 1: return "Green"; break;
            case 2: return "Blue"; break;
            case 3: return "Alpha"; break;
            }
        } else {
            return section;
        }
    }
    return QVariant();
}
