#include "timeentriesmodel.h"
#include <QUuid>

namespace Entities {

TimeEntriesModel::TimeEntriesModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

TimeEntriesModel::~TimeEntriesModel()
{

}

int TimeEntriesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return count();
}

QVariant TimeEntriesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > count())
        return QVariant();

    auto item = m_items[index.row()];
    switch (role) {
    case ItemIdRole:
        return item->id;
    case TaskId:
        return item->taskId;
    case DurationRole:
        return item->duration;
    case StartDateRole:
        return item->startDate;
    case EndDateRole:
        return item->endDate;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TimeEntriesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ItemIdRole, "itemId");
    roles.insert(TaskId, "task");
    roles.insert(DurationRole, "duration");
    roles.insert(StartDateRole, "startDate");
    roles.insert(EndDateRole, "endDate");

    return roles;
}

int TimeEntriesModel::count() const
{
    return m_items.size();
}

void TimeEntriesModel::clearModel()
{
    if(!count())
        return;

    beginRemoveRows(QModelIndex(), 0, count() - 1);
    m_items.clear();
    endRemoveRows();

    emit countChanged(count());
}

TimeEntryPtr TimeEntriesModel::getItem(const QString &id)
{
    for (auto item : m_items)
    {
        if (item->id == id)
            return item;
    }
    return nullptr;
}

void TimeEntriesModel::addItem(const QString &id, const QString &taskId, const QString &startDate, const QString &endDate, const QString &duration)
{
    auto newItem = std::make_shared<TimeEntry>();
    newItem->id = id == "" ? QUuid::createUuid().toString()
                           : id;
    newItem->taskId = taskId;
    newItem->startDate = startDate;
    newItem->endDate = endDate;
    newItem->duration = duration;

    beginInsertRows(QModelIndex(), count(), count());
    m_items.append(newItem);
    endInsertRows();
}

void TimeEntriesModel::removeItem(const QString &id)
{
    for(int i = 0; i < count(); i++)
    {
        if(m_items[i]->id == id)
            removeItem(i);
    }
}

void TimeEntriesModel::removeItem(const int index)
{
    if(index < 0 || index >= count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_items.removeAt(index);
    endRemoveRows();

    emit countChanged(count());
}

}
