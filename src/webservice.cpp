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

void WebService::getAllWorkspaces(const QString &ownerId, std::shared_ptr<Enteties::WorkspacesModel> workspaceModel)
{
    QString requestUrl(m_host + "/graphql");

    QString query = QString("{"
                            "workspaces(ownerId: \"%1\") {"
                                "id name ownerId}"
                            "}").arg(ownerId);
    QJsonObject requestObject
    {
        {"query", QJsonValue(query)}
    };

    QJsonDocument doc(requestObject);

    QNetworkRequest request;
    request.setUrl(requestUrl);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    postRequest(request, doc.toJson(), [this, workspaceModel, ownerId](ResponsePtr resp) {
        if (resp->isError)
        {
            qCDebug(webService) << "Request error" <<  resp->errorString;
        }
        else
        {
            qCDebug(webService) << "Request status code" << resp->statusCode;
            if (resp->statusCode == 200)
            {
//                qCDebug(webService) << resp->data;
                QJsonObject dataObject = QJsonDocument::fromJson(resp->data)
                        .object()
                        .value("data")
                        .toObject(QJsonObject());
                if (!dataObject.isEmpty())
                {
                    QJsonArray workspaceArray = dataObject.value("workspaces").toArray(QJsonArray());
                    workspaceModel->clearModel(); //NOTE: надо ли прям очищать
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
                    qCDebug(webService) << "Workspace model updated" << workspaceModel->count();
                }
            }
        }
    });
}

void WebService::getAllProjects(const QString &ownerId, std::shared_ptr<Enteties::ProjectsModel> projectsModel)
{
    QString requestUrl(m_host + "/graphql");

    QString query = QString("{"
                            "workspaces(ownerId: \"%1\") {"
                                "id projects {"
                                    "id name}"
                                "}"
                            "}").arg(ownerId);
    QJsonObject requestObject
    {
        {"query", QJsonValue(query)}
    };

    QJsonDocument doc(requestObject);

    QNetworkRequest request;
    request.setUrl(requestUrl);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    postRequest(request, doc.toJson(), [this, projectsModel](ResponsePtr resp) {
        if (resp->isError)
        {
            qCDebug(webService) << "Request error" <<  resp->errorString;
        }
        else
        {
            qCDebug(webService) << "Request status code" << resp->statusCode;
            if (resp->statusCode == 200)
            {
//                qCDebug(webService) << resp->data;
                QJsonObject dataObject = QJsonDocument::fromJson(resp->data)
                        .object()
                        .value("data")
                        .toObject(QJsonObject());
                if (!dataObject.isEmpty())
                {
                    QJsonArray workspaceArray = dataObject.value("workspaces").toArray(QJsonArray());
                    projectsModel->clearModel();
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
                                    projectsModel->addItem(projectId, projectName, workspaceId);
                                }
                            }
                        }
                    }
                    qCDebug(webService) << "Workspace model updated" << projectsModel->count();
                }
            }
        }
    });
}

void WebService::getAllTasks(const QString &ownerId, std::shared_ptr<Enteties::TasksModel> taskModel)
{
    QString requestUrl(m_host + "/graphql");

    QString query = QString("{"
                            "workspaces(ownerId: \"%1\") {"
                                "id projects {"
                                    "id tasks {"
                                        "id}"
                                    "}"
                                "}"
                            "}").arg(ownerId);
    QJsonObject requestObject
    {
        {"query", QJsonValue(query)}
    };

    QJsonDocument doc(requestObject);

    QNetworkRequest request;
    request.setUrl(requestUrl);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    postRequest(request, doc.toJson(), [this, taskModel](ResponsePtr resp) {
        if (resp->isError)
        {
            qCDebug(webService) << "Request error" <<  resp->errorString;
        }
        else
        {
            qCDebug(webService) << "Request status code" << resp->statusCode;
            if (resp->statusCode == 200)
            {
//                qCDebug(webService) << resp->data;
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
                    qCDebug(webService) << "Workspace model updated" << taskModel->count();
                }
            }
        }
    });
}

void WebService::getRequest(const QNetworkRequest &request, PerformCallback callback)
{
    auto reply = m_manager.get(request);
    requestFunction(reply, callback);
}

void WebService::postRequest(const QNetworkRequest &request, const QByteArray &data, PerformCallback callback)
{
    auto reply = m_manager.post(request, data);
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














