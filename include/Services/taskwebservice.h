#ifndef TASKWEBSERVICE_H
#define TASKWEBSERVICE_H

#include <QObject>
#include "tasksmodel.h"
#include "httpsinglton.h"

using namespace Entities;

class TaskWebService : public QObject
{
    Q_OBJECT
public:
    explicit TaskWebService(QObject *parent = nullptr);

    void getAllTasks(const QString &ownerId, TasksModelPtr taskModel, SuccessCallback successCallback);

    void createTask(const QString &name, const QString &projectId, TaskPtr task, SuccessCallback successCallback);

    /**
     * @brief updateTask
     * @param id
     * @param params - QVariantMap:
     * "name" - new name of Task (QString)
     * "description" - new descrption of Task (QString)
     * @param successCallback
     */
    void updateTask(const QString &id, const QVariantMap &params, SuccessCallback successCallback);

    void removeTask(const QString &id, SuccessCallback successCallback);
};

#endif // TASKWEBSERVICE_H
