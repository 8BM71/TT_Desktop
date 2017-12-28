#ifndef LOGICCORE_H
#define LOGICCORE_H

#include <QObject>

#include "items.h"
#include "workspacesmodel.h"
#include "projectsmodel.h"
#include "tasksmodel.h"
#include "timeentriesmodel.h"

#include "appwebservice.h"
#include "workspacewebservice.h"
#include "projectwebservice.h"
#include "taskwebservice.h"
#include "timeentrywebservice.h"

using namespace Entities;

class LogicCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(WorkspacesModel* workspacesModel READ workspacesModel NOTIFY workspacesModelChanged)
    Q_PROPERTY(ProjectsModel* projectsModel READ projectsModel NOTIFY projectsModelChanged)
    Q_PROPERTY(TasksModel* tasksModel READ tasksModel NOTIFY tasksModelChanged)
    Q_PROPERTY(TimeEntriesModel* timeEntriesModel READ timeEntriesModel NOTIFY timeEntriesModelChanged)
    Q_PROPERTY(QString timerDuration READ timerDuration NOTIFY timerDurationChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(QVariantMap currentTask READ currentTask NOTIFY currentTaskChanged)

    Q_PROPERTY(bool workspacesLoading READ workspacesLoading NOTIFY workspacesLoadingChanged)
    Q_PROPERTY(bool projectsLoading READ projectsLoading NOTIFY projectsLoadingChanged)
    Q_PROPERTY(bool tasksLoading READ tasksLoading NOTIFY tasksLoadingChanged)

    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged)
    Q_PROPERTY(QString userEmail READ userEmail NOTIFY userEmailChanged)

public:
    explicit LogicCore(QObject *parent = nullptr);
    ~LogicCore();

    WorkspacesModel* workspacesModel() const;

    ProjectsModel* projectsModel() const;

    TasksModel* tasksModel() const;

    TimeEntriesModel* timeEntriesModel() const;

    QString timerDuration() const;

    bool running() const;

    bool workspacesLoading() const;

    bool projectsLoading() const;

    bool tasksLoading() const;

    QVariantMap currentTask() const;

    QString userName() const;

    QString userEmail() const;

signals:
    void workspacesModelChanged();

    void projectsModelChanged();

    void tasksModelChanged();

    void timeEntriesModelChanged();

    void timerDurationChanged(QString timerDuration);

    void runningChanged(bool running);

    void workspacesLoadingChanged(bool workspacesLoading);

    void projectsLoadingChanged(bool projectsLoading);

    void tasksLoadingChanged(bool tasksLoading);

    void currentTaskChanged(QVariantMap currentTask);

    void logined();

    void userNameChanged();

    void userEmailChanged();

public slots:

    void siginWithGoogle();

    // Task, time entry methods
    void startNewTask(const QString &taskName, const QString &projectId);

    void stopTask();

    void startExistTask(const QString &taskId);

    void deleteTask(const QString &taskId);

    void deleteTimeEntry(const QString &id);

    void updateTask(const QString &taskId, const QString &name, const QString &projectId, const QString &description);

    void updateTimeEntry();

    void setProjectForTask();
    // !Task, time entry methods


    // Project methods
    void createNewProject(const QString &name, const QString &workspaceId);

    void moveProjectToWorkspace();

    void updateProject(const QString &id, const QString &name);

    void deleteProject(const QString &projectId);

    void setProjectAsDefault();
    // !Project methods


    // Workspace methods
    void createNewWorkspace(const QString &name);

    void updateWorkspace(const QString &id, const QString &name);

    void deleteWorkspace(const QString &workspaceId);

    void setWorkspaceAsDefault();
    // !Workspace methods

    void setWorkspacesLoading(bool workspacesLoading);

    void setProjectsLoading(bool projectsLoading);

    void setTasksLoading(bool tasksLoading);

    void updateWorkspacesModel();

    void updateProjectsModel();

    void updateTasksModel();

    void updateTimeEntriesModel();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void onAuthorizedSuccess(const QString &id, const QString &username, const QString &email);

    void onAuthorizedFailed(const QString &errorString);

private:
    void updateAll();

    void updateTimerDuration();

private:
    AppWebService m_appService;
    WorkspaceWebService m_wsService;
    ProjectWebService m_projService;
    TaskWebService m_taskService;
    TimeEntryWebService m_timeService;
    UserPtr m_currentUser;
    WorkspacesModelPtr m_workspacesModel;
    ProjectsModelPtr m_projectModel;
    TasksModelPtr m_tasksModel;
    TimeEntriesModelPtr m_timeEntriesModel;
    QString m_timerDuration;
    bool m_running;
    int m_timerId;
    TimeEntryPtr m_currentTimeEntry;
    TaskPtr m_currentTask;
    bool m_workspacesLoading;
    bool m_projectsLoading;
    bool m_tasksLoading;
};

#endif // LOGICCORE_H
