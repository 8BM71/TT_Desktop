#ifndef LOGICCORE_H
#define LOGICCORE_H

#include <QObject>
#include "webservice.h"
#include "items.h"
#include "workspacesmodel.h"
#include "projectsmodel.h"
#include "tasksmodel.h"
#include "timeentriesmodel.h"

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

public:
    explicit LogicCore(QObject *parent = nullptr);
    ~LogicCore();

    WorkspacesModel* workspacesModel() const;

    ProjectsModel* projectsModel() const;

    TasksModel* tasksModel() const;

    TimeEntriesModel* timeEntriesModel() const;

    QString timerDuration() const;

    bool running() const;

signals:
    void workspacesModelChanged();

    void projectsModelChanged();

    void tasksModelChanged();

    void timeEntriesModelChanged();

    void timerDurationChanged(QString timerDuration);

    void runningChanged(bool running);

public slots:
    // Task, time entry methods
    void startNewTask(const QString &taskName, int projectIndex);

    void startExistTask();

    void stopTask();

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

    void deleteProject();

    void setProjectAsDefault();
    // !Project methods


    // Workspace methods
    void createNewWorkspace(const QString &name);

    void updateWorkspace();

    void deleteWorkspace();

    void setWorkspaceAsDefault();
    // !Workspace methods

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);

private:
    void updateWorkspacesModel();

    void updateProjectsModel();

    void updateTasksModel();

    void updateTimeEntriesModel();

private:
    WebService m_webService;
    Entities::User m_currentUser;
    std::shared_ptr<WorkspacesModel> m_workspacesModel;
    std::shared_ptr<ProjectsModel> m_projectModel;
    std::shared_ptr<TasksModel> m_tasksModel;
    std::shared_ptr<TimeEntriesModel> m_timeEntriesModel;
    QString m_timerDuration;
    bool m_running;
    int m_timerId;
    TimeEntryPtr m_currentTimeEntry;
};

#endif // LOGICCORE_H
