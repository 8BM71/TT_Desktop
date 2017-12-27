#include "tasksmodel.h"
#include <QUuid>
#include <QDebug>

namespace Entities {

TasksModel::TasksModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

TasksModel::~TasksModel()
{

}

int TasksModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return count();
}

QVariant TasksModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > count())
        return QVariant();

    auto item = m_items[index.row()];
    switch (role) {
    case ItemIdRole:
        return item->id;
    case NameRole:
        return item->name;
    case DescriptionRole:
        return item->description;
    case ProjectRole:
        return item->projectId;
    default:
        return QVariant();
    }
}

bool TasksModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    auto item = m_items[index.row()];
    switch (role) {
        case NameRole:{
            item->name = value.toString();
            emit dataChanged(index, index, QVector<int>() << NameRole);
            return true;
        }
        case DescriptionRole:{
            item->description = value.toString();
            emit dataChanged(index, index, QVector<int>() << DescriptionRole);
            return true;
        }
        case ProjectRole:{
            item->projectId = value.toString();
            emit dataChanged(index, index, QVector<int>() << ProjectRole);
            return true;
        }
    }

    return false;
}

QHash<int, QByteArray> TasksModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ItemIdRole, "itemId");
    roles.insert(NameRole, "name");
    roles.insert(DescriptionRole, "description");
    roles.insert(ProjectRole, "project");

    return roles;
}

int TasksModel::count() const
{
    return m_items.size();
}

void TasksModel::clearModel()
{
    if(!count())
        return;

    beginRemoveRows(QModelIndex(), 0, count() - 1);
    m_items.clear();
    endRemoveRows();

    emit countChanged(count());
}

TaskPtr TasksModel::getItem(const QString &id)
{
    for (auto item : m_items)
    {
        if (item->id == id)
            return item;
    }
    return nullptr;
}

QVariantMap TasksModel::getItemData(const QString &id)
{
    auto item = getItem(id);
    if (item)
    {
        QVariantMap itemData;
        itemData.insert(roleNames().value(Roles::ItemIdRole), item->id);
        itemData.insert(roleNames().value(Roles::NameRole), item->name);
        itemData.insert(roleNames().value(Roles::ProjectRole), item->projectId);
        itemData.insert(roleNames().value(Roles::DescriptionRole), item->description);
        return itemData;
    }
    return QVariantMap();
}

void TasksModel::addItem(const QString &id, const QString &projectId, const QString &name, const QString &description)
{
    auto newItem = std::make_shared<Task>();
    newItem->id = id == "" ? QUuid::createUuid().toString()
                           : id;
    newItem->name = name;
    newItem->projectId = projectId;
    newItem->description = description;

    addItem(newItem);
}

void TasksModel::addItem(TaskPtr item)
{
    if (item == nullptr)
        return;
    beginInsertRows(QModelIndex(), count(), count());
    m_items.append(item);
    endInsertRows();
    emit countChanged(count());
}

void TasksModel::removeItem(const QString &id)
{
    for(int i = 0; i < count(); i++)
    {
        if(m_items[i]->id == id)
            removeItem(i);
    }
}

void TasksModel::removeItem(const int index)
{
    if(index < 0 || index >= count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_items.removeAt(index);
    endRemoveRows();

    emit countChanged(count());
}

int TasksModel::getIndex(const QString &id)
{
    for (int i = 0; i < m_items.size(); i++)
    {
        if (m_items[i]->id == id)
            return i;
    }
    return -1;
}

}
