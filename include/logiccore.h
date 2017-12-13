#ifndef LOGICCORE_H
#define LOGICCORE_H

#include <QObject>
#include "webservice.h"
#include "items.h"
#include "workspacesmodel.h"
#include "projectsmodel.h"
#include "tasksmodel.h"
#include "timeentriesmodel.h"

using namespace Enteties;

class LogicCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(WorkspacesModel* workspacesModel READ workspacesModel NOTIFY workspacesModelChanged)
    Q_PROPERTY(ProjectsModel* projectsModel READ projectsModel NOTIFY projectsModelChanged)
    Q_PROPERTY(TasksModel* tasksModel READ tasksModel NOTIFY tasksModelChanged)
    Q_PROPERTY(TimeEntriesModel* timeEntriesModel READ timeEntriesModel NOTIFY timeEntriesModelChanged)

public:
    explicit LogicCore(QObject *parent = nullptr);
    ~LogicCore();

    WorkspacesModel* workspacesModel() const;

    ProjectsModel* projectsModel() const;

    TasksModel* tasksModel() const;

    TimeEntriesModel* timeEntriesModel() const;

signals:
    void workspacesModelChanged();

    void projectsModelChanged();

    void tasksModelChanged();

    void timeEntriesModelChanged();

public slots:
    // Task, time entry methods
    void startNewTask(const QString &taskName, const QString& projectId);

    void startExistTask();

    void stopTask();

    void deleteTask();

    void deleteTimeEntry();

    void updateTask();

    void updateTimeEntry();

    void setProjectForTask();
    // !Task, time entry methods


    // Project methods
    void createNewProject();

    void moveProjectToWorkspace();

    void updateProject();

    void deleteProject();

    void setProjectAsDefault();
    // !Project methods


    // Workspace methods
    void createNewWorkspace();

    void updateWorkspace();

    void deleteWorkspace();

    void setWorkspaceAsDefault();
    // !Workspace methods

private:
    void updateWorkspacesModel();

    void updateProjectsModel();

    void updateTasksModel();

private:
    WebService m_webService;
    Enteties::User m_currentUser;
    std::shared_ptr<WorkspacesModel> m_workspacesModel;
    std::shared_ptr<ProjectsModel> m_projectModel;
    std::shared_ptr<TasksModel> m_tasksModel;
    std::shared_ptr<TimeEntriesModel> m_timeEntriesModel;
};

#endif // LOGICCORE_H
