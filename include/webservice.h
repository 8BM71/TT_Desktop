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
#include "timeentriesmodel.h"

struct Response
{
    bool isError = false;
    QString errorString = "";
    int statusCode = 0;
    QByteArray data;
    QMap<QString, QString> headerData;
};

typedef std::shared_ptr<Response> ResponsePtr;

typedef std::function<void (ResponsePtr)> PerformCallback;
typedef std::function<void (bool, QString)> SuccessCallback;

using namespace Entities;

class WebService : public QObject
{
    Q_OBJECT
public:
    explicit WebService(QObject *parent = nullptr);
    ~WebService();

    void getAllWorkspaces(const QString &ownerId, WorkspacesModelPtr workspaceModel, SuccessCallback successCallback);

    void getAllProjects(const QString &ownerId, ProjectsModelPtr projectModel, SuccessCallback successCallback);

    void getAllTasks(const QString &ownerId, TasksModelPtr taskModel, SuccessCallback successCallback);

    void getAllTimeEntries(const QString &ownerId, TimeEntriesModelPtr timeModel, SuccessCallback successCallback);

    void createWorkspace(const QString &name, const QString &ownerId, SuccessCallback successCallback);

    void deleteWorkspace(const QString &workspaceId, SuccessCallback successCallback);

    void createProject(const QString &name, const QString &workspaceId, SuccessCallback successCallback);

    void deleteProject(const QString &projectId, SuccessCallback successCallback);

    void createTask(const QString &name, const QString &projectId, TasksModelPtr taskModel, TimeEntriesModelPtr timeModel, SuccessCallback successCallback);

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
