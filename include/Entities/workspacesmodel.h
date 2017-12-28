#ifndef WORKSPACESMODEL_H
#define WORKSPACESMODEL_H

#include <QAbstractListModel>
#include "items.h"

namespace Entities {

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
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual QHash<int, QByteArray> roleNames() const override;

public slots:
    int count() const;
    void clearModel();

    WorkspacePtr getItem(const QString &id) const;
    WorkspacePtr getItem(const int index) const;

    QVariantMap getItemData(const QString &id) const;
    QVariantMap get(int ind) const;

    void addItem(const QString &id, const QString &name, const QString &ownerId);
    void removeItem(const QString &id);
    void removeItem(const int index);
    int getIndex(const QString &id);

signals:
    void countChanged(int count);

private:
    QList<WorkspacePtr> m_items;
};

typedef std::shared_ptr<WorkspacesModel> WorkspacesModelPtr;

}

#endif // WORKSPACESMODEL_H
