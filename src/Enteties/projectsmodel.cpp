#include "projectsmodel.h"

namespace Enteties {

ProjectsModel::ProjectsModel(QObject *parent)
    : QAbstractListModel(parent)
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

void ProjectsModel::addItem(const QString &id, const QString &name, const QString &workscpace)
{

}

void ProjectsModel::removeItem(const QString &id)
{

}

void ProjectsModel::removeItem(const int index)
{

}


}
