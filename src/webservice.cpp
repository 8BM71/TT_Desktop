#include "webservice.h"
#include <QTimer>
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(webService, "WebService")

WebService::WebService(QObject *parent)
    : QObject(parent)
    , m_waitTime(30 * 1000)
{
    m_host = "https://tttpu.tk";
}

WebService::~WebService()
{
}

void WebService::createUser(const QString &username, const QString &email, UserPtr user, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($user: UserInput!){"
                                "createUser(user: $user)"
                            "}");
    QJsonObject userParams{
        {"user",
            QJsonObject {
                {"username", username},
                {"email", email}
            }
        }
    };

    postRequest(query, [this, successCallback, user, username, email](ResponsePtr resp) {
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
                    QString userId = dataObject.value("createUser").toString("");
                    user->id = userId;
                    user->name = username;
                    user->email = email;
                    successCallback(true, QString("User %1 created with id %2").arg(username, userId));
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    }, userParams);
}

void WebService::getAllWorkspaces(const QString &ownerId, WorkspacesModelPtr workspaceModel, SuccessCallback successCallback)
{
    QString query = QString("{"
                                "workspaces(ownerId: \"%1\") {"
                                    "id name ownerId}"
                            "}").arg(ownerId);

    postRequest(query, [this, workspaceModel, ownerId, successCallback](ResponsePtr resp) {
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
                    workspaceModel->clearModel();
                    for (auto workspaceValue : workspaceArray)
                    {
                        if (workspaceValue.isObject())
                        {
                            QJsonObject workspace = workspaceValue.toObject();
                            QString id = workspace.value("id").toString("");
                            QString name = workspace.value("name").toString("");
                            workspaceModel->addItem(id, name, ownerId);
                        }
                    }
                    successCallback(true, "Workspace model updated");
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    });
}

void WebService::getAllProjects(const QString &ownerId, ProjectsModelPtr projectModel, SuccessCallback successCallback)
{
    QString query = QString("{"
                                "workspaces(ownerId: \"%1\") {"
                                    "id projects {"
                                        "id name}"
                                    "}"
                            "}").arg(ownerId);

    postRequest(query, [this, projectModel, successCallback](ResponsePtr resp) {
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
                    projectModel->clearModel();
                    for (auto workspaceValue : workspaceArray)
                    {
                        if (workspaceValue.isObject())
                        {
                            QJsonObject workspace = workspaceValue.toObject();
                            QString workspaceId = workspace.value("id").toString("");

                            QJsonArray projectArray = workspace.value("projects").toArray(QJsonArray());
                            for (auto projectValue : projectArray)
                            {
                                if (projectValue.isObject())
                                {
                                    QJsonObject project = projectValue.toObject();
                                    QString projectId = project.value("id").toString("");
                                    QString projectName = project.value("name").toString("");
                                    projectModel->addItem(projectId, projectName, workspaceId);
                                }
                            }
                        }
                    }
                    successCallback(true, "Project model updated");
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    });
}

void WebService::getAllTasks(const QString &ownerId, TasksModelPtr taskModel, SuccessCallback successCallback)
{
    QString query = QString("{"
                                "workspaces(ownerId: \"%1\") {"
                                    "id projects {"
                                        "id tasks {"
                                            "id name}"
                                        "}"
                                    "}"
                            "}").arg(ownerId);

    postRequest(query, [this, taskModel, successCallback](ResponsePtr resp) {
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

void WebService::getAllTimeEntries(const QString &ownerId, TimeEntriesModelPtr timeModel, SuccessCallback successCallback)
{
    QString query = QString("{"
                                "workspaces(ownerId: \"%1\") {"
                                    "tasks {"
                                        "id timeEntries {"
                                            "id duration endDate startDate}"
                                         "}"
                                    "}"
                            "}").arg(ownerId);

    postRequest(query, [this, timeModel, successCallback](ResponsePtr resp) {
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
                    timeModel->clearModel();
                    for (auto workspaceValue : workspaceArray)
                    {
                        if (workspaceValue.isObject())
                        {
                            QJsonObject workspace = workspaceValue.toObject();

                            QJsonArray taskArray = workspace.value("tasks").toArray(QJsonArray());
                            for (auto taskValue : taskArray)
                            {
                                if (taskValue.isObject())
                                {
                                    QJsonObject task = taskValue.toObject();
                                    QString taskId = task.value("id").toString("");

                                    QJsonArray timeArray = task.value("timeEntries").toArray(QJsonArray());

                                    for(auto timeValue : timeArray)
                                    {
                                        if (timeValue.isObject())
                                        {
                                            QJsonObject timeEntry = timeValue.toObject();

                                            QString timeEntryId = timeEntry.value("id").toString("");


                                            QTime time(0, 0, 0, 0);
                                            time = time.addMSecs(static_cast<int>(timeEntry.value("duration").toVariant().toLongLong()));

                                            QString durationAsString = time.toString("hh:mm:ss");

                                            QDateTime startDateTime;
                                            qint64 startDateValue = timeEntry["startDate"].toString("").toLongLong();
                                            if (startDateValue > 0)
                                                startDateTime.setMSecsSinceEpoch(startDateValue);

                                            QDateTime endDateTime;
                                            qint64 endDateValue = timeEntry["endDate"].toString("").toLongLong();
                                            if (endDateValue > 0)
                                                endDateTime.setMSecsSinceEpoch(endDateValue);

                                            auto timeEntryItem = std::make_shared<TimeEntry>();

                                            timeEntryItem->id = timeEntryId;
                                            timeEntryItem->startDate = startDateTime.date().toString("dd.MM.yy");
                                            timeEntryItem->startTime = startDateTime.time().toString("hh:mm:ss");
                                            timeEntryItem->duration = durationAsString;
                                            timeEntryItem->endDate = endDateTime.date().toString("dd.MM.yy");
                                            timeEntryItem->endTime = endDateTime.time().toString("hh:mm:ss");
                                            timeEntryItem->taskId = taskId;

                                            timeModel->addItem(timeEntryItem);

                                        }
                                    }
                                }
                            }
                        }
                    }
                    successCallback(true, "Time entry model updated");
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    });
}

void WebService::createWorkspace(const QString &name, const QString &ownerId, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($ws: WorkspaceInput!) {"
                                "createWorkspace(workspace: $ws)"
                            "}");

    QJsonObject wsParams{
        {"ws",
            QJsonObject {
                {"name", name},
                {"ownerId", ownerId}
            }
        }
    };

    postRequest(query, [this, successCallback](ResponsePtr resp) {
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
                    QString id = dataObject.value("createWorkspace").toString("");
                    successCallback(true, id);
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    }, wsParams);
}

void WebService::deleteWorkspace(const QString &workspaceId, SuccessCallback successCallback)
{
    QString query = QString("mutation M {"
                            "removeWorkspace(id: \"%1\")"
                            "}").arg(workspaceId);

    postRequest(query, [this, successCallback](ResponsePtr resp) {
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
                    if(dataObject.value("deleteWorkspace").toBool(false))
                        successCallback(true, "Ok");
                    else
                    {
                        QJsonArray errorArray = QJsonDocument::fromJson(resp->data)
                                .object()
                                .value("errors")
                                .toArray(QJsonArray());

                        if(!errorArray.isEmpty())
                        {
                            QJsonObject errorObject = errorArray[0].toObject();
                            QString message = errorObject.value("message").toString("Some error");
                            successCallback(false, message);
                        }
                        else
                        {
                            successCallback(false, "Some error");
                        }
                    }
                }
                else
                    successCallback(false, "Incorrect response from server");

            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    });
}

void WebService::createProject(const QString &name, const QString &workspaceId, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($wsId: String!, $proj: ProjectInput!) {"
                                "createProject(wsId: $wsId, project: $proj)"
                            "}");

    QJsonObject projParams {
        {"wsId", workspaceId},
        {"proj",
            QJsonObject {
                {"name", name}
            }
        }
    };

    postRequest(query, [this, successCallback](ResponsePtr resp) {
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
                    QString id = dataObject.value("createProject").toString("");
                        successCallback(true, id);
                }
                else
                    successCallback(false, "Incorrect response from server");

            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    }, projParams);
}

void WebService::deleteProject(const QString &projectId, SuccessCallback successCallback)
{
    QString query = QString("mutation M {"
                                "removeProject(id: \"%1\")"
                            "}").arg(projectId);

    postRequest(query, [this, successCallback](ResponsePtr resp) {
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

                    if(dataObject.value("deleteProject").toBool(false))
                        successCallback(true, "Ok");
                    else
                    {
                        QJsonArray errorArray = QJsonDocument::fromJson(resp->data)
                                .object()
                                .value("errors")
                                .toArray(QJsonArray());

                        if(!errorArray.isEmpty())
                        {
                            QJsonObject errorObject = errorArray[0].toObject();
                            QString message = errorObject.value("message").toString("Some error");
                            successCallback(false, message);
                        }
                        else
                        {
                            successCallback(false, "Some error");
                        }
                    }
                }
                else
                    successCallback(false, "Incorrect response from server");

            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    });
}

void WebService::createTask(const QString &name, const QString &projectId, TaskPtr task, SuccessCallback successCallback)
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

    postRequest(query, [this, successCallback, task, projectId, name](ResponsePtr resp) {
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

void WebService::deleteTask(const QString &taskId, SuccessCallback successCallback)
{

}

void WebService::startTask(const QString &taskId, TimeEntryPtr timeEntry, SuccessCallback successCallback)
{

    QString query = QString("mutation M ($taskId: String!){"
                                "startTask(taskId: $taskId) {"
                                    "id startDate}"
                            "}");

    QJsonObject taskParams {
        {"taskId", taskId}
    };

    postRequest(query, [this, successCallback, timeEntry, taskId](ResponsePtr resp) {
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
                    QJsonObject resultObject = dataObject.value("startTask").toObject(QJsonObject());
                    if (!resultObject.isEmpty())
                    {
                        QString timeId = resultObject.value("id").toString("");
                        QString startDate = resultObject.value("startDate").toString();

                        QDateTime dateTime;

                        dateTime.setMSecsSinceEpoch(startDate.toLongLong());

                        QString dateAsString = dateTime.date().toString("dd.MM.yy");

                        QString timeAsString = dateTime.time().toString("hh:mm:ss");

                        timeEntry->id = timeId;
                        timeEntry->taskId = taskId;
                        timeEntry->startDate = dateAsString;
                        timeEntry->startTime = timeAsString;
                        timeEntry->startMSecsSinceEpoch = startDate.toLongLong();

                        successCallback(true, QString("Time entry created with id: %1").arg(timeId));
                    }
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    }, taskParams);
}

void WebService::stopTimeEntry(TimeEntryPtr timeEntry, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($id: String!) {"
                                "stopTimeEntry(id: $id) {"
                                    "id duration endDate}"
                            "}");

    QJsonObject teParams {
        {"id", timeEntry->id}
    };

    postRequest(query, [this, successCallback, timeEntry](ResponsePtr resp) {
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
                    QJsonObject resultObject = dataObject.value("stopTimeEntry").toObject(QJsonObject());
                    if (!resultObject.isEmpty())
                    {
                        QTime time(0, 0, 0, 0);
                        time = time.addMSecs(static_cast<int>(resultObject.value("duration").toVariant().toLongLong()));

                        QString durationAsString = time.toString("hh:mm:ss");

                        QString endDate = resultObject.value("endDate").toString();

                        QDateTime dateTime;

                        dateTime.setMSecsSinceEpoch(endDate.toLongLong());

                        QString dateAsString = dateTime.date().toString("dd.MM.yy");

                        QString timeAsString = dateTime.time().toString("hh:mm:ss");

                        timeEntry->endDate = dateAsString;
                        timeEntry->endTime = timeAsString;
                        timeEntry->duration = durationAsString;

                        successCallback(true, QString("Time entry '%1' stopped").arg(timeEntry->id));
                    }
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    }, teParams);
}

void WebService::getRequest(const QNetworkRequest &request, PerformCallback callback)
{
    auto reply = m_manager.get(request);
    requestFunction(reply, callback);
}

void WebService::postRequest(const QString &query, PerformCallback callback, QJsonValue vars)
{
    QString requestUrl(m_host + "/graphql");

    QJsonObject requestObject
    {
        {"query", query},
        {"variables", vars}
    };

    QJsonDocument doc(requestObject);

    QNetworkRequest request;
    request.setUrl(requestUrl);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");


    auto reply = m_manager.post(request, doc.toJson());
    requestFunction(reply, callback);
}

void WebService::requestFunction(QNetworkReply *reply, PerformCallback callback)
{
    if (reply == nullptr)
    {
        ResponsePtr resp = std::make_shared<Response>();
        resp->isError = true;
        resp->errorString = "Reply is nullptr";
        callback(resp);
        return;
    }

    std::shared_ptr<QTimer> timer = std::make_shared<QTimer>();

    timer->setInterval(m_waitTime);
    timer->setSingleShot(true);

    connect(timer.get(), &QTimer::timeout, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::readyRead, timer.get(), &QTimer::stop);

    timer->start();

    connect(reply, &QNetworkReply::finished, [this, reply, timer, callback] (){
        timer->stop();

        ResponsePtr response = std::make_shared<Response>();

        if (reply == nullptr)
        {
            response->isError = true;
            response->errorString = "Reply is nullptr";
            callback(response);
            return;
        }

        if (reply->error() != QNetworkReply::NoError)
        {
            response->isError = true;
            response->errorString = reply->errorString();
        }
        else
        {
            response->statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            for (auto it : reply->rawHeaderPairs())
            {
                QString name(it.first);
                QString data(it.second);
                response->headerData.insert(name, data);
            }
            response->data = reply->readAll();
        }
        callback(response);
        reply->deleteLater();
    });
}
