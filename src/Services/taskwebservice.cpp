#include "taskwebservice.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

TaskWebService::TaskWebService(QObject *parent) : QObject(parent)
{

}

void TaskWebService::getAllTasks(const QString &ownerId, TasksModelPtr taskModel, SuccessCallback successCallback)
{
    QString query = QString("{"
                                "workspaces(ownerId: \"%1\") {"
                                    "id projects {"
                                        "id tasks {"
                                            "id name}"
                                        "}"
                                    "}"
                            "}").arg(ownerId);

    HttpSinglton::instance()->postRequest(query, [this, taskModel, successCallback](ResponsePtr resp) {
        if (resp->isError)
        {
            successCallback(false, resp->errorString);
        }
        else
        {
            if (resp->statusCode == 200)
            {
                QJsonObject dataObject = QJsonDocument::fromJson(resp->data)
                        .object()
                        .value("data")
                        .toObject(QJsonObject());
                if (!dataObject.isEmpty())
                {
                    QJsonArray workspaceArray = dataObject.value("workspaces").toArray(QJsonArray());
                    taskModel->clearModel();
                    for (auto workspaceValue : workspaceArray)
                    {
                        if (workspaceValue.isObject())
                        {
                            QJsonObject workspace = workspaceValue.toObject();

                            QJsonArray projectArray = workspace.value("projects").toArray(QJsonArray());
                            for (auto projectValue : projectArray)
                            {
                                if (projectValue.isObject())
                                {
                                    QJsonObject project = projectValue.toObject();
                                    QString projectId = project.value("id").toString("");

                                    QJsonArray taskArray = project.value("tasks").toArray(QJsonArray());
                                    for (auto taskValue : taskArray)
                                    {
                                        if (taskValue.isObject())
                                        {
                                            QJsonObject task = taskValue.toObject();
                                            QString taskId = task.value("id").toString("");
                                            QString taskName = task.value("name").toString("");
                                            taskModel->addItem(taskId, projectId, taskName);
                                        }
                                    }

                                }
                            }
                        }
                    }
                    successCallback(true, "Task model updated");
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    });
}

void TaskWebService::createTask(const QString &name, const QString &projectId, TaskPtr task, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($projId: String!, $task: TaskInput!){"
                                "createTask(projId: $projId, task: $task)"
                            "}");

    QJsonObject taskParams {
        {"projId", projectId},
        {"task",
            QJsonObject {
                {"name", name}
            }
        }
    };

    HttpSinglton::instance()->postRequest(query, [this, successCallback, task, projectId, name](ResponsePtr resp) {
        if (resp->isError)
        {
            successCallback(false, resp->errorString);
        }
        else
        {
            if (resp->statusCode == 200)
            {
                QJsonObject dataObject = QJsonDocument::fromJson(resp->data)
                        .object()
                        .value("data")
                        .toObject(QJsonObject());
                if (!dataObject.isEmpty())
                {
                    QString id = dataObject.value("createTask").toString("");
                    task->id = id;
                    task->name = name;
                    task->projectId = projectId;
                    successCallback(true, QString("Task '%0' created with id: %1").arg(name).arg(id));
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    }, taskParams);
}

void TaskWebService::updateTask(const QString &id, const QVariantMap &params, SuccessCallback successCallback)
{

}

void TaskWebService::removeTask(const QString &id, SuccessCallback successCallback)
{

}
