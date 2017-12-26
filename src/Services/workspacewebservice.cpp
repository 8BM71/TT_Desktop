#include "workspacewebservice.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

WorkspaceWebService::WorkspaceWebService(QObject *parent) : QObject(parent)
{

}

void WorkspaceWebService::getAllWorkspaces(const QString &ownerId, WorkspacesModelPtr workspaceModel, SuccessCallback successCallback)
{
    QString query = QString("{"
                                "workspaces(ownerId: \"%1\") {"
                                    "id name ownerId}"
                            "}").arg(ownerId);

    HttpSinglton::instance()->postRequest(query, [this, workspaceModel, ownerId, successCallback](ResponsePtr resp) {
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

void WorkspaceWebService::createWorkspace(const QString &ownerId, const QVariantMap &params, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($ws: WorkspaceInput!) {"
                                "createWorkspace(workspace: $ws)"
                            "}");

    QJsonObject wsParams{
        {"ws",
            QJsonObject {
                {"name", params["name"].toString()},
                {"ownerId", ownerId}
            }
        }
    };

    HttpSinglton::instance()->postRequest(query, [this, successCallback](ResponsePtr resp) {
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

void WorkspaceWebService::updateWorkspace(const QString &id, const QVariantMap &params, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($id: String!, $ws: WorkspaceInput!){"
                                "updateWorkspace(id: $id, workspace: $ws)"
                            "}");
    QJsonObject wsParams{
        {"id", id},
        {"ws",
            QJsonObject {
                {"name", params["name"].toString()},
                {"description", params["description"].toString()}
            }
        }
    };
    HttpSinglton::instance()->postRequest(query, [this, successCallback](ResponsePtr resp) {
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
                    if(dataObject.value("updateWorkspace").toBool(false))
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
    }, wsParams);
}

void WorkspaceWebService::removeWorkspace(const QString &id, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($id: String!) {"
                                "removeWorkspace(id: $id)"
                            "}");

    QJsonObject wsParams{
        {"id", id}
    };

    HttpSinglton::instance()->postRequest(query, [this, successCallback](ResponsePtr resp) {
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
                    if(dataObject.value("removeWorkspace").toBool(false))
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
    }, wsParams);
}
