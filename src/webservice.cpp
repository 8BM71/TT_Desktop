#include "webservice.h"
#include <QTimer>
#include <QDebug>
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
    m_host = "http://54.69.134.117:8008";
}

WebService::~WebService()
{
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
                            "id timeEntry {"
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

                                    QJsonObject timeEntry = task.value("timeEntry").toObject(QJsonObject());

                                    QString timeEntryId = timeEntry.value("id").toString("");
                                    qint64 duration = timeEntry.value("duration").toVariant().toLongLong();

                                    QDateTime startDateTime;
                                    startDateTime.setMSecsSinceEpoch(timeEntry["startDate"].toString("").toLongLong());

                                    QDateTime endDateTime;
                                    endDateTime.setMSecsSinceEpoch(timeEntry["endDate"].toString("").toLongLong());

                                    auto timeEntryItem = std::make_shared<TimeEntry>();

                                    timeEntryItem->id = timeEntryId;
                                    timeEntryItem->startDate = startDateTime.date().toString("dd.MM.yy");
                                    timeEntryItem->startTime = startDateTime.time().toString("hh.mm.ss");
                                    timeEntryItem->duration = QString::number(duration);
                                    timeEntryItem->endDate = endDateTime.date().toString("dd.MM.yy");
                                    timeEntryItem->endTime = endDateTime.time().toString("hh.mm.ss");
                                    timeEntryItem->taskId = taskId;

                                    timeModel->addItem(timeEntryItem);

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
    QString query = QString("mutation M {"
                            "createWorkspace(name: \"%1\", ownerId: \"%2\") {"
                            "id name ownerId}"
                            "}").arg(name).arg(ownerId);

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
                    QJsonObject resultObject = dataObject.value("createWorkspace").toObject(QJsonObject());
                    if (!resultObject.isEmpty())
                    {
                        QString id = resultObject.value("id").toString("");
                        //QString name = resultObject.value("name").toString("");
                        successCallback(true, id/*QString("Workspace '%1' created with id %2").arg(name).arg(id)*/);
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

void WebService::deleteWorkspace(const QString &workspaceId, SuccessCallback successCallback)
{
    QString query = QString("mutation M {"
                            "deleteWorkspace(wsId: \"%1\")"
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
    QString query = QString("mutation M {"
                            "createProject(name: \"%1\", wsId: \"%2\") {"
                            "id name}"
                            "}").arg(name).arg(workspaceId);

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
                    QJsonObject resultObject = dataObject.value("createProject").toObject(QJsonObject());
                    if (!resultObject.isEmpty())
                    {
                        QString id = resultObject.value("id").toString("");
                        //QString name = resultObject.value("name").toString("");
                        successCallback(true, id/*QString("Project '%1' created with id %2").arg(name).arg(id)*/);
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

void WebService::deleteProject(const QString &projectId, SuccessCallback successCallback)
{
    QString query = QString("mutation M {"
                            "deleteProject(projId: \"%1\")"
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

void WebService::createTask(const QString &name, const QString &projectId, TasksModelPtr taskModel, TimeEntriesModelPtr timeModel, SuccessCallback successCallback)
{
    QString query = QString("mutation M {"
                            "createTask(projId: \"%1\", name: \"%2\") {"
                            "id name timeEntry {id startDate}}"
                            "}").arg(projectId).arg(name);

    postRequest(query, [this, successCallback, taskModel, projectId, timeModel](ResponsePtr resp) {
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
                    QJsonObject resultObject = dataObject.value("createTask").toObject(QJsonObject());
                    if (!resultObject.isEmpty())
                    {
                        QString taskId = resultObject.value("id").toString("");
                        QString taskName = resultObject.value("name").toString("");

                        QJsonObject timeEntry = resultObject.value("timeEntry").toObject(QJsonObject());

                        QString timeId = "";

                        if (!timeEntry.isEmpty())
                        {
                            timeId = timeEntry.value("id").toString("");
                            QString startDate = timeEntry.value("startDate").toString();

                            QDateTime dateTime;

                            dateTime.setMSecsSinceEpoch(startDate.toLongLong());

                            QString dateAsString = dateTime.date().toString("dd.MM.yy");

                            QString timeAsString = dateTime.time().toString("hh.mm.ss");

                            auto timeEntryItem = std::make_shared<TimeEntry>();
                            timeEntryItem->id = timeId;
                            timeEntryItem->taskId = taskId;
                            timeEntryItem->startTime = timeAsString;
                            timeEntryItem->startDate = dateAsString;
                            timeEntryItem->startMSecsSinceEpoch = startDate.toLongLong();
                            timeModel->addItem(timeEntryItem);


                            qCDebug(webService) << startDate << dateAsString << timeAsString;

//                            timeModel->addItem(timeId, taskId, dateAsString, timeAsString);
                        }
                        taskModel->addItem(taskId, projectId, taskName);
                        successCallback(true, timeId); //NOTE: may be return timentry and task
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

void WebService::getRequest(const QNetworkRequest &request, PerformCallback callback)
{
    auto reply = m_manager.get(request);
    requestFunction(reply, callback);
}

void WebService::postRequest(const QString &query, PerformCallback callback)
{
    QString requestUrl(m_host + "/graphql");

    QJsonObject requestObject
    {
        {"query", QJsonValue(query)}
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
