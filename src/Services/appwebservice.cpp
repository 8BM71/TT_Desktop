#include "appwebservice.h"
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>

const QUrl authUri("https://accounts.google.com/o/oauth2/auth");
const QUrl tokenUri("https://accounts.google.com/o/oauth2/token");
const QUrl redirectUri("http://localhost:8080/cb");
const QString clientId("68255243973-9h8rb8mq10kq00a2830eetktipe1miab.apps.googleusercontent.com");
const QString clientSecret("CZ4m2O8togWdDCXeQGK4Mj2Y");

AppWebService::AppWebService(QObject *parent) : QObject(parent)
{
    auto replyHandler = new GoogleQOAuthHttpServerReplyHandler(static_cast<quint16>(redirectUri.port()), this);
    replyHandler->setCallbackPath("cb");
    m_google.setReplyHandler(replyHandler);
    m_google.setAuthorizationUrl(authUri);
    m_google.setClientIdentifier(clientId);
    m_google.setAccessTokenUrl(tokenUri);
    m_google.setClientIdentifierSharedKey(clientSecret);
    m_google.setScope("profile email");


    connect(&m_google, &QOAuth2AuthorizationCodeFlow::statusChanged, [=](QAbstractOAuth::Status status){
        if (status == QAbstractOAuth::Status::Granted)
        {
//            qDebug() << "Granted token: " << m_google.token();
            auto handler = dynamic_cast<GoogleQOAuthHttpServerReplyHandler*>(m_google.replyHandler());
            this->m_idToken = handler->idToken();
            this->auth();
        }
    });

    connect(&m_google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

}

void AppWebService::createUser(const QString &username, const QString &email, UserPtr user, SuccessCallback successCallback)
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

    HttpSinglton::instance()->postRequest(query, [this, successCallback, user, username, email](ResponsePtr resp) {
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

void AppWebService::updateAllEntities(const QString &ownerId, WorkspacesModelPtr workspaceModel, ProjectsModelPtr projectModel, TasksModelPtr taskModel, TimeEntriesModelPtr timeModel, SuccessCallback successCallback)
{
    QString query = QString("{"
                                "workspaces(ownerId: \"%1\") {"
                                    "id name projects { "
                                        "id name tasks {"
                                            "id name description timeEntries {"
                                                "id duration endDate startDate}"
                                            "}"
                                        "}"
                                    "}"
                            "}").arg(ownerId);

    HttpSinglton::instance()->postRequest(query, [this, ownerId, workspaceModel, projectModel, taskModel, timeModel, successCallback](ResponsePtr resp) {
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
                    projectModel->clearModel();
                    taskModel->clearModel();
                    timeModel->clearModel();
                    for (auto workspaceValue : workspaceArray)
                    {
                        if (workspaceValue.isObject())
                        {
                            QJsonObject workspace = workspaceValue.toObject();

                            QString wsId = workspace["id"].toString("");
                            QString wsName = workspace["name"].toString("");

                            workspaceModel->addItem(wsId, wsName, ownerId);

                            QJsonArray projectArray = workspace.value("projects").toArray(QJsonArray());


                            for (auto projectValue : projectArray)
                            {
                                if (projectValue.isObject())
                                {
                                    QJsonObject project = projectValue.toObject();

                                    QString projId = project["id"].toString("");
                                    QString projName = project["name"].toString("");

                                    projectModel->addItem(projId, projName, wsId);

                                    QJsonArray taskArray = project.value("tasks").toArray(QJsonArray());

                                    for (auto taskValue : taskArray)
                                    {
                                        if (taskValue.isObject())
                                        {
                                            QJsonObject task = taskValue.toObject();
                                            QString taskId = task.value("id").toString("");
                                            QString taskName = task["name"].toString("");
                                            QString taskDesc = task["description"].toString("");

                                            taskModel->addItem(taskId, projId, taskName, taskDesc);

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
                        }
                    }
                    successCallback(true, "All models updated");
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    });
}

void AppWebService::auth()
{
    QString query = QString("mutation M ($token: String!){"
                                "auth(token: $token) {"
                                    "id username email}"
                            "}");

    QJsonObject authParams {
        {"token", m_idToken}
    };

    HttpSinglton::instance()->postRequest(query, [this](ResponsePtr resp) {
        if (resp->isError)
        {
            emit this->authorizedFailed(resp->errorString);
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
                    QJsonObject resultObject = dataObject.value("auth").toObject(QJsonObject());
                    if (!resultObject.isEmpty())
                    {
                        QString id = resultObject.value("id").toString("");
                        QString username = resultObject.value("username").toString();
                        QString email = resultObject.value("email").toString();

//                        qDebug() << "id" << id;
//                        qDebug() << "username" << username;
//                        qDebug() << "email" << email;

                        emit this->authorizedSuccess(id, username, email);
                    }
                }
                else
                    emit this->authorizedFailed("Incorrect response from server");
            }
            else
                emit this->authorizedFailed(QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    }, authParams);
}

void AppWebService::authorizeWithGoogle()
{
    m_google.grant();
}
