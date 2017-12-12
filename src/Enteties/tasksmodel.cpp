#include "tasksmodel.h"
#include <QUuid>

namespace Enteties {

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

void TasksModel::addItem(const QString &id, const QString &projectId, const QString &name, const QString &description)
{
    auto newItem = std::make_shared<Task>();
    newItem->id = id == "" ? QUuid::createUuid().toString()
                           : id;
    newItem->name = name;
    newItem->projectId = projectId;
    newItem->description = description;

    beginInsertRows(QModelIndex(), count(), count());
    m_items.append(newItem);
    endInsertRows();
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

}
