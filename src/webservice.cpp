#include "webservice.h"
#include <QTimer>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(webService, "WebService")

WebService::WebService(QObject *parent)
    : QObject(parent)
    , m_waitTime(30 * 1000)
{
    m_host = "http://localhost:8080";
}

WebService::~WebService()
{
}

void WebService::getAllWorkspaces(const QString &ownerId, std::shared_ptr<Enteties::WorkspacesModel> workspaceModel, SuccessCallback successCallback)
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

void WebService::getAllProjects(const QString &ownerId, std::shared_ptr<Enteties::ProjectsModel> projectModel, SuccessCallback successCallback)
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

void WebService::getAllTasks(const QString &ownerId, std::shared_ptr<Enteties::TasksModel> taskModel, SuccessCallback successCallback)
{
    QString query = QString("{"
                            "workspaces(ownerId: \"%1\") {"
                                "id projects {"
                                    "id tasks {"
                                        "id}"
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
                                            taskModel->addItem(taskId, projectId, "");
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
                        QString name = resultObject.value("name").toString("");
                        successCallback(true, QString("Workspace '%1' created with id %2").arg(name).arg(id));
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
                        QString name = resultObject.value("name").toString("");
                        successCallback(true, QString("Project '%1' created with id %2").arg(name).arg(id));
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

void WebService::createTask(const QString &name, const QString &projectId, std::shared_ptr<Enteties::TasksModel> taskModel, SuccessCallback successCallback)
{
    QString query = QString("mutation M {"
                            "createTask(projId: \"%1\", name: \"%2\") {"
                                "id name}"
                            "}").arg(projectId).arg(name);

    postRequest(query, [this, successCallback, taskModel, projectId](ResponsePtr resp) {
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
                        QString id = resultObject.value("id").toString("");
                        QString taskName = resultObject.value("name").toString("");
                        taskModel->addItem(id, projectId, taskName);
                        successCallback(true, QString("Task %0 created with id %1").arg(taskName).arg(id));
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














