#ifndef PROJECTSMODEL_H
#define PROJECTSMODEL_H

#include <QAbstractListModel>
#include "items.h"

namespace Entities {

class ProjectsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles{
        ItemIdRole = Qt::UserRole + 1,
        NameRole,
        DescriptionRole,
        WorkspaceRole
    };

    explicit ProjectsModel(QObject *parent = nullptr);
    ~ProjectsModel();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    int count() const;

    void addItem(const QString &id, const QString &name, const QString &workscpace);
    void removeItem(const QString &id);
    void removeItem(const int index);

signals:
    void countChanged(int count);

private:
    int m_count;
    QList<ProjectPtr> m_items;
};

}
#endif // PROJECTSMODEL_H
