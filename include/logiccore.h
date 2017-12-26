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

signals:
    void workspacesModelChanged();

    void projectsModelChanged();

    void tasksModelChanged();

    void timeEntriesModelChanged();

    void timerDurationChanged(QString timerDuration);

    void runningChanged(bool running);

    void waitingChanged(bool waiting);

    void currentTaskNameChanged(QString currentTaskName);

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

protected:
    void timerEvent(QTimerEvent *event);

private:
    void updateAll();

    void updateWorkspacesModel();

    void updateProjectsModel();

    void updateTasksModel();

    void updateTimeEntriesModel();

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
};

#endif // LOGICCORE_H
