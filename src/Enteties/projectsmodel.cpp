#include "projectsmodel.h"
#include <QUuid>

namespace Enteties {

ProjectsModel::ProjectsModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_count(0)
{

}

ProjectsModel::~ProjectsModel()
{

}

int ProjectsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

QVariant ProjectsModel::data(const QModelIndex &index, int role) const
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
    case WorkspaceRole:
        return item->workspaceId;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ProjectsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ItemIdRole, "itemId");
    roles.insert(NameRole, "name");
    roles.insert(DescriptionRole, "description");
    roles.insert(WorkspaceRole, "workspace");

    return roles;
}

int ProjectsModel::count() const
{
    return m_count;
}

void ProjectsModel::clearModel()
{
    if(!count())
        return;

    beginRemoveRows(QModelIndex(), 0, count() - 1);
    m_items.clear();
    endRemoveRows();

    emit countChanged(count());
}

ProjectPtr ProjectsModel::getItem(const QString &id)
{
    for (auto item : m_items)
    {
        if (item->id == id)
            return item;
    }
    return nullptr;
}

void ProjectsModel::addItem(const QString &id, const QString &name, const QString &workscpaceId)
{
    auto newItem = std::make_shared<Project>();
    newItem->id = id == "" ? QUuid::createUuid().toString()
                           : id;
    newItem->name = name;
    newItem->workspaceId = workscpaceId;
    newItem->description = "";

    beginInsertRows(QModelIndex(), count(), count());
    m_items.append(newItem);
    endInsertRows();
}

void ProjectsModel::removeItem(const QString &id)
{
    for(int i = 0; i < count(); i++)
    {
        if(m_items[i]->id == id)
            removeItem(i);
    }
}

void ProjectsModel::removeItem(const int index)
{
    if(index < 0 || index >= count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_items.removeAt(index);
    endRemoveRows();

    emit countChanged(count());
}


}