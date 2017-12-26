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
    Q_PROPERTY(bool waiting READ waiting NOTIFY waitingChanged)
    Q_PROPERTY(QString currentTaskName READ currentTaskName WRITE setCurrentTaskName NOTIFY currentTaskNameChanged)

    Q_PROPERTY(bool workspacesLoading READ workspacesLoading WRITE setWorkspacesLoading NOTIFY workspacesLoadingChanged)
    Q_PROPERTY(bool projectsLoading READ projectsLoading WRITE setProjectsLoading NOTIFY projectsLoadingChanged)
    Q_PROPERTY(bool tasksLoading READ tasksLoading WRITE setTasksLoading NOTIFY tasksLoadingChanged)

public:
    explicit LogicCore(QObject *parent = nullptr);
    ~LogicCore();

    WorkspacesModel* workspacesModel() const;

    ProjectsModel* projectsModel() const;

    TasksModel* tasksModel() const;

    TimeEntriesModel* timeEntriesModel() const;

    QString timerDuration() const;

    bool running() const;

    bool waiting() const;

    QString currentTaskName() const;

    bool workspacesLoading() const;

    bool projectsLoading() const;

    bool tasksLoading() const;

signals:
    void workspacesModelChanged();

    void projectsModelChanged();

    void tasksModelChanged();

    void timeEntriesModelChanged();

    void timerDurationChanged(QString timerDuration);

    void runningChanged(bool running);

    void waitingChanged(bool waiting);

    void currentTaskNameChanged(QString currentTaskName);

    void workspacesLoadingChanged(bool workspacesLoading);

    void projectsLoadingChanged(bool projectsLoading);

    void tasksLoadingChanged(bool tasksLoading);

public slots:
    // Task, time entry methods
    void startNewTask(const QString &taskName, const QString &projectId);

    void stopTask();

    void startExistTask(const QString &taskId);

    void deleteTask();

    void deleteTimeEntry();

    void updateTask();

    void updateTimeEntry();

    void setProjectForTask();
    // !Task, time entry methods


    // Project methods
    void createNewProject(const QString &name, const QString &workspaceId);

    void moveProjectToWorkspace();

    void updateProject();

    void deleteProject(const QString &projectId);

    void setProjectAsDefault();
    // !Project methods


    // Workspace methods
    void createNewWorkspace(const QString &name);

    void updateWorkspace();

    void deleteWorkspace(const QString &workspaceId);

    void setWorkspaceAsDefault();
    // !Workspace methods
    void setCurrentTaskName(QString currentTaskName);

    void setWorkspacesLoading(bool workspacesLoading);

    void setProjectsLoading(bool projectsLoading);

    void setTasksLoading(bool tasksLoading);

    void updateWorkspacesModel();

    void updateProjectsModel();

    void updateTasksModel();

    void updateTimeEntriesModel();

protected:
    void timerEvent(QTimerEvent *event);

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
    bool m_waiting;
    QString m_currentTaskName;
    bool m_workspacesLoading;
    bool m_projectsLoading;
    bool m_tasksLoading;
};

#endif // LOGICCORE_H
