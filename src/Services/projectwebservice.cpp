#include "projectwebservice.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

ProjectWebService::ProjectWebService(QObject *parent) : QObject(parent)
{

}

void ProjectWebService::getAllProjects(const QString &ownerId, ProjectsModelPtr projectModel, SuccessCallback successCallback)
{
    QString query = QString("{"
                                "workspaces(ownerId: \"%1\") {"
                                    "id projects {"
                                        "id name}"
                                    "}"
                            "}").arg(ownerId);

    HttpSinglton::instance()->postRequest(query, [this, projectModel, successCallback](ResponsePtr resp) {
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

void ProjectWebService::createProject(const QString &name, const QString &workspaceId, SuccessCallback successCallback)
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

void ProjectWebService::updateProject(const QString &id, const QVariantMap &params, SuccessCallback successCallback)
{

}

void ProjectWebService::removeProject(const QString &id, SuccessCallback successCallback)
{
    QString query = QString("mutation M {"
                                "removeProject(id: \"%1\")"
                            "}").arg(id);

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
