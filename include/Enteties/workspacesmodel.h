#ifndef WORKSPACESMODEL_H
#define WORKSPACESMODEL_H

#include <QAbstractListModel>
#include "items.h"

namespace Enteties {

class WorkspacesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum Roles{
        ItemIdRole = Qt::UserRole + 1,
        NameRole,
        OwnerRole
    };

    explicit WorkspacesModel(QObject *parent = nullptr);
    ~WorkspacesModel();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    int count() const;
    void clearModel();
    WorkspacePtr getItem(const QString &id);

    void addItem(const QString &id, const QString &name, const QString &ownerId);
    void removeItem(const QString &id);
    void removeItem(const int index);

signals:
    void countChanged(int count);

private:
    QList<WorkspacePtr> m_items;
};

}

#endif // WORKSPACESMODEL_H
