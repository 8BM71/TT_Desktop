#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <functional>
#include <memory>

#include "workspacesmodel.h"
#include "projectsmodel.h"
#include "tasksmodel.h"

struct Response
{
    bool isError = false;
    QString errorString = "";
    int statusCode = 0;
    QByteArray data;
    QMap<QString, QString> headerData;
};

typedef std::shared_ptr<Response> ResponsePtr;

typedef std::function<void(ResponsePtr)> PerformCallback;

class WebService : public QObject
{
    Q_OBJECT
public:
    explicit WebService(QObject *parent = nullptr);

    void getAllWorkspaces(const QString &ownerId, std::shared_ptr<Enteties::WorkspacesModel> workspaceModel, std::function<void(bool)> successCallback);

//    void getWorkspaceById();

    void getAllProjects(const QString &ownerId, std::shared_ptr<Enteties::ProjectsModel> projectsModel, std::function<void(bool)> successCallback); //TODO: add success callback or signal

//    void getProjectById();

    void getAllTasks(const QString &ownerId, std::shared_ptr<Enteties::TasksModel> taskModel); //TODO: add success callback or signal

//    void getTaskById();

//    void getTimeEntries();

    void createWorkspace(const QString &name, const QString &ownerId); //TODO: add success callback or signal

    void createProject(const QString &name, const QString &workspaceId); //TODO: add success callback or signal

    void createTask(const QString &projectId);

//    void createTimeEntry();

    //TODO: authorization

signals:

public slots:


private:
    void getRequest(const QNetworkRequest& request, PerformCallback callback);

    void postRequest(const QString &query, PerformCallback callback);

    void requestFunction(QNetworkReply * reply, PerformCallback callback);



private:
    QNetworkAccessManager m_manager;
    QString m_host;
    int m_waitTime;
};

#endif // WEBSERVICE_H
