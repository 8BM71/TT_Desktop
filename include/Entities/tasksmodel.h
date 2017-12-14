#ifndef TASKSMODEL_H
#define TASKSMODEL_H

#include <QAbstractListModel>
#include "items.h"

namespace Entities {

class TasksModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum Roles{
        ItemIdRole = Qt::UserRole + 1,
        NameRole,
        DescriptionRole,
        ProjectRole
    };

    explicit TasksModel(QObject *parent = nullptr);
    ~TasksModel();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

public slots:
    int count() const;
    void clearModel();
    TaskPtr getItem(const QString &id);

    QVariantMap getItemData(const QString &id);

    void addItem(const QString &id, const QString &projectId, const QString &name, const QString &description = "");
    void removeItem(const QString &id);
    void removeItem(const int index);

signals:
    void countChanged(int count);

private:
    QList<TaskPtr> m_items;

};

typedef std::shared_ptr<TasksModel> TasksModelPtr;

}

#endif // TASKSMODEL_H
