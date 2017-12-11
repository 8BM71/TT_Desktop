#ifndef TASKSMODEL_H
#define TASKSMODEL_H

#include <QAbstractListModel>
#include "items.h"

namespace Enteties {

class TasksModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TasksModel(QObject *parent = nullptr);
    ~TasksModel();

signals:

public slots:
};

}

#endif // TASKSMODEL_H
