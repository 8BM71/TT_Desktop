#ifndef TIMEENTRIESMODEL_H
#define TIMEENTRIESMODEL_H

#include <QAbstractListModel>
#include "items.h"

namespace Enteties {

class TimeEntriesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles{
        ItemIdRole = Qt::UserRole + 1,
        DurationRole,
        StartDateRole,
        EndDateRole,
        TaskId
    };

    explicit TimeEntriesModel(QObject *parent = nullptr);
    ~TimeEntriesModel();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    int count() const;
    void clearModel();
    TimeEntryPtr getItem(const QString &id);

    void addItem(const QString &id, const QString &taskId, const QString &startDate, const QString &endDate = "", const QString &duration = "");
    void removeItem(const QString &id);
    void removeItem(const int index);

signals:
    void countChanged(int count);

private:
    int m_count;
    QList<TimeEntryPtr> m_items;
};

}

#endif // TIMEENTRIESMODEL_H
