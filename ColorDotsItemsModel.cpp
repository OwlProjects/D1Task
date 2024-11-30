#include "ColorDotsItemsModel.h"


ColorDotsItemsModel::ColorDotsItemsModel(QObject *parent)
    : QAbstractItemModel{parent}
    , root_{new QStandardItem()}
{}

ColorDotsItemsModel::~ColorDotsItemsModel()
{
    delete root_;
}

void ColorDotsItemsModel::setImage(QImage image)
{
    beginResetModel();
    image_ = image;
    bits_ = QByteArray(reinterpret_cast<char*>(image_.bits()), image_.sizeInBytes());
    delete root_;
    channelsCount_ = image_.hasAlphaChannel() ? 4 : 3;
    root_ = new QStandardItem("Pixels");
    for (int i = 0; i < bits_.size(); i += channelsCount_) {
        QStandardItem* pix = new QStandardItem(QString("Pixel %1").arg(i / channelsCount_));
        QStandardItem* newItem;
        uchar red = static_cast<uchar>(bits_[i + 0]);
        uchar green = static_cast<uchar>(bits_[i + 1]);
        uchar blue = static_cast<uchar>(bits_[i + 2]);
        newItem = new QStandardItem(QString("red: %1").arg(red, 2, 16));
        newItem->setData(red);
        pix->appendRow(newItem);
        newItem = new QStandardItem(QString("green: %1").arg(green, 2, 16));
        newItem->setData(green);
        pix->appendRow(newItem);
        newItem = new QStandardItem(QString("blue:  %1").arg(blue, 2, 16));
        newItem->setData(blue);
        pix->appendRow(newItem);
        if (channelsCount_ == 4) {
            uchar alpha = static_cast<uchar>(bits_[i + 3]);
            newItem = new QStandardItem(QString("alpha: %1").arg(alpha, 2, 16));
            newItem->setData(alpha);
            pix->appendRow(newItem);
        }
        root_->appendRow(pix);
    }
    endResetModel();
}

QModelIndex ColorDotsItemsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};

    QStandardItem *parentItem = parent.isValid()
        ? static_cast<QStandardItem*>(parent.internalPointer())
        : root_;

    if (auto *childItem = parentItem->child(row))
        return createIndex(row, column, childItem);
    return {};
}


QModelIndex ColorDotsItemsModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return {};

    auto *childItem = static_cast<QStandardItem*>(child.internalPointer());
    QStandardItem *parentItem = childItem->parent();

    return parentItem != root_
        ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex{};
}

int ColorDotsItemsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const QStandardItem *parentItem = parent.isValid()
        ? static_cast<const QStandardItem*>(parent.internalPointer())
        : root_;

    return parentItem->rowCount();
}

int ColorDotsItemsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<QStandardItem*>(parent.internalPointer())->columnCount();
    return root_->columnCount();
}

QVariant ColorDotsItemsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    if (role == Qt::DisplayRole) {
        const auto *item = static_cast<const QStandardItem*>(index.internalPointer());
        return item->data(index.column());
    } else if (role == Qt::DecorationRole) {
        const auto *item = static_cast<const QStandardItem*>(index.internalPointer());
        if (item->hasChildren()) {

            return channelsCount_ == 4
                ? QColor(item->child(0)->data().toUInt(),
                        item->child(1)->data().toUInt(),
                        item->child(2)->data().toUInt(),
                        item->child(3)->data().toUInt())
                : QColor(item->child(0)->data().toUInt(),
                        item->child(1)->data().toUInt(),
                        item->child(2)->data().toUInt());
        } else {
            return QColor();
        }
    } else {
        return {};
    }
}

Qt::ItemFlags ColorDotsItemsModel::flags(const QModelIndex &index) const
{
    // Q_UNUSED(index);
    // return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return QAbstractItemModel::flags(index);
}

constexpr quint32 ColorDotsItemsModel::rowCol2PlainIndex(quint32 r, quint32 c, quint32 rLength) const
{
    return c + r * rLength;
}

QVariant ColorDotsItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole &&
        orientation == Qt::Horizontal &&
        section == 0) {
        return "Pixels";
    }
    return "Data";
}
