#include "logiccore.h"
#include <QDebug>
#include <QLoggingCategory>
#include <QMetaType>

Q_LOGGING_CATEGORY(logicCore, "LogicCore")

LogicCore::LogicCore(QObject *parent) : QObject(parent)
{
    m_currentUser.id = "42cfb602-f544-4b1b-b01d-63cd6a0b644f";
    m_workspacesModel = std::make_shared<WorkspacesModel>();
    m_projectModel = std::make_shared<ProjectsModel>();
    m_tasksModel = std::make_shared<TasksModel>();
    m_timeEntriesModel = std::make_shared<TimeEntriesModel>();

    updateWorkspacesModel();
    updateProjectsModel();
    updateTasksModel();
}

LogicCore::~LogicCore()
{

}

WorkspacesModel *LogicCore::workspacesModel() const
{
    return m_workspacesModel.get();
}

ProjectsModel *LogicCore::projectsModel() const
{
    return m_projectModel.get();
}

TasksModel *LogicCore::tasksModel() const
{
    return m_tasksModel.get();
}

TimeEntriesModel *LogicCore::timeEntriesModel() const
{
    return m_timeEntriesModel.get();
}

void LogicCore::startNewTask(const QString &taskName, int projectIndex)
{
    auto project = m_projectModel->getItem(projectIndex);
    if (project == nullptr)
        return;
    QString projectId = project->id;
    m_webService.createTask(taskName, projectId, m_tasksModel, m_timeEntriesModel, [](bool success, QString info){
        qCDebug(logicCore) << QString("Create task success: %0, info: %1").arg(success).arg(info);
    });
}

void LogicCore::startExistTask()
{
    //TODO: implement
}

void LogicCore::stopTask()
{
    //TODO: implement
}

void LogicCore::deleteTask()
{
    //TODO: implement
}

void LogicCore::deleteTimeEntry()
{
    //TODO: implement
}

void LogicCore::updateTask()
{
    //TODO: implement
}

void LogicCore::updateTimeEntry()
{
    //TODO: implement
}

void LogicCore::setProjectForTask()
{
    //TODO: implement
}

void LogicCore::createNewProject()
{
    //TODO: implement
}

void LogicCore::moveProjectToWorkspace()
{
    //TODO: implement
}

void LogicCore::updateProject()
{
    //TODO: implement
}

void LogicCore::deleteProject()
{
    //TODO: implement
}

void LogicCore::setProjectAsDefault()
{
    //TODO: implement
}

void LogicCore::createNewWorkspace()
{
    //TODO: implement
}

void LogicCore::updateWorkspace()
{
    //TODO: implement
}

void LogicCore::deleteWorkspace()
{
    //TODO: implement
}

void LogicCore::setWorkspaceAsDefault()
{
    //TODO: implement
}

void LogicCore::updateWorkspacesModel()
{
    m_webService.getAllWorkspaces(m_currentUser.id, m_workspacesModel, [](bool succes, QString info){
        qCDebug(logicCore) << QString("Update workspaces success: %0, info: %1").arg(succes).arg(info);
    });
}

void LogicCore::updateProjectsModel()
{
    m_webService.getAllProjects(m_currentUser.id, m_projectModel, [](bool succes, QString info){
        qCDebug(logicCore) << QString("Update projects success: %0, info: %1").arg(succes).arg(info);
    });
}

void LogicCore::updateTasksModel()
{
    m_webService.getAllTasks(m_currentUser.id, m_tasksModel, [](bool succes, QString info){
        qCDebug(logicCore) << QString("Update tasks success: %0, info: %1").arg(succes).arg(info);
    });
}
