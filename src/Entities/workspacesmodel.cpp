#include "workspacesmodel.h"
#include <QUuid>
#include <QDebug>

namespace Entities {

WorkspacesModel::WorkspacesModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

WorkspacesModel::~WorkspacesModel()
{

}

int WorkspacesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return count();
}

QVariant WorkspacesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > count())
        return QVariant();

    auto item = m_items[index.row()];
    switch (role) {
    case ItemIdRole:
        return item->id;
    case NameRole:
        return item->name;
    case OwnerRole:
        return item->ownerId;
    default:
        return QVariant();
    }
}

bool WorkspacesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    auto item = m_items[index.row()];
    switch (role) {
        case NameRole:{
            item->name = value.toString();
            emit dataChanged(index, index, QVector<int>() << NameRole);
            return true;
        }
        //case DescriptionRole:{
        //    item->description = value.toString();
        //    emit dataChanged(index, index, QVector<int>() << DescriptionRole);
        //    return true;
        //}
        case OwnerRole:{
            item->ownerId = value.toString();
            emit dataChanged(index, index, QVector<int>() << OwnerRole);
            return true;
        }
    }

    return false;
}

QHash<int, QByteArray> WorkspacesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ItemIdRole, "itemId");
    roles.insert(NameRole, "name");
    roles.insert(OwnerRole, "owner");

    return roles;
}

int WorkspacesModel::count() const
{
    return m_items.size();
}

void WorkspacesModel::clearModel()
{
    if(!count())
        return;

    beginRemoveRows(QModelIndex(), 0, count() - 1);
    m_items.clear();
    endRemoveRows();

    emit countChanged(count());
}

WorkspacePtr WorkspacesModel::getItem(const QString &id) const
{
    for (auto item : m_items)
    {
        if (item->id == id)
            return item;
    }
    return nullptr;
}

WorkspacePtr WorkspacesModel::getItem(const int index) const
{
    if(index < 0 || index >= count())
            return nullptr;
    return m_items.at(index);
}

QVariantMap WorkspacesModel::getItemData(const QString &id) const
{
    auto item = getItem(id);
    if(item)
    {
        QVariantMap itemData;
        itemData.insert("id", item->id);
        itemData.insert("name", item->name);
        itemData.insert("owner", item->ownerId);

        return itemData;
    }

    return QVariantMap();
}

QVariantMap WorkspacesModel::get(int ind) const
{
    auto item = getItem(ind);

    if(item)
    {
        QVariantMap itemData;
        itemData.insert("id", item->id);
        itemData.insert("name", item->name);
        itemData.insert("owner", item->ownerId);

        return itemData;
    }

    return QVariantMap();
}

void WorkspacesModel::addItem(const QString &id, const QString &name, const QString &ownerId)
{
    auto newItem = std::make_shared<Workspace>();
    newItem->id = id == "" ? QUuid::createUuid().toString()
                           : id;
    newItem->name = name;
    newItem->ownerId = ownerId;

    beginInsertRows(QModelIndex(), count(), count());
    m_items.append(newItem);
    endInsertRows();

    emit countChanged(count());
}

void WorkspacesModel::removeItem(const QString &id)
{
    for(int i = 0; i < count(); i++)
    {
        if(m_items[i]->id == id)
            removeItem(i);
    }
}

void WorkspacesModel::removeItem(const int index)
{
    if(index < 0 || index >= count())
            return;

    beginRemoveRows(QModelIndex(), index, index);
    m_items.removeAt(index);
    endRemoveRows();

    emit countChanged(count());
}

int WorkspacesModel::getIndex(const QString &id)
{
    for(int i = 0; i < count(); i++)
    {
        if(m_items[i]->id == id)
            return i;
    }

    return -1;
}

}
