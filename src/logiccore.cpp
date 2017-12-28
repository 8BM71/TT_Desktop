#include "logiccore.h"
#include <QDebug>
#include <QLoggingCategory>
#include <QMetaType>
#include <QTimerEvent>
#include <QDateTime>
#include <QModelIndex>

Q_LOGGING_CATEGORY(logicCore, "LogicCore")

LogicCore::LogicCore(QObject *parent)
    : QObject(parent)
    , m_timerDuration("00:00:00")
    , m_running(false)
    , m_timerId(-1)
    , m_workspacesLoading(false)
    , m_projectsLoading(false)
    , m_tasksLoading(false)
{
    m_currentUser = std::make_shared<User>();
    m_workspacesModel = std::make_shared<WorkspacesModel>();
    m_projectModel = std::make_shared<ProjectsModel>();
    m_tasksModel = std::make_shared<TasksModel>();
    m_timeEntriesModel = std::make_shared<TimeEntriesModel>();

    connect(&m_appService, &AppWebService::authorizedSuccess, this, &LogicCore::onAuthorizedSuccess);
    connect(&m_appService, &AppWebService::authorizedFailed, this, &LogicCore::onAuthorizedFailed);
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

QString LogicCore::timerDuration() const
{
    return m_timerDuration;
}

bool LogicCore::running() const
{
    return m_running;
}

bool LogicCore::workspacesLoading() const
{
    return m_workspacesLoading;
}

bool LogicCore::projectsLoading() const
{
    return m_projectsLoading;
}

bool LogicCore::tasksLoading() const
{
    return m_tasksLoading;
}

QVariantMap LogicCore::currentTask() const
{
    if(m_currentTask == nullptr)
        return QVariantMap();

    QVariantMap task;
    task.insert(m_tasksModel->roleNames().value(TasksModel::Roles::ItemIdRole), m_currentTask->id);
    task.insert(m_tasksModel->roleNames().value(TasksModel::Roles::NameRole), m_currentTask->name);
    task.insert(m_tasksModel->roleNames().value(TasksModel::Roles::ProjectRole), m_currentTask->projectId);
    task.insert(m_tasksModel->roleNames().value(TasksModel::Roles::DescriptionRole), m_currentTask->description);

    return task;
}

QString LogicCore::userName() const
{
    return m_currentUser->name;
}

QString LogicCore::userEmail() const
{
    return m_currentUser->email;
}

void LogicCore::siginWithGoogle()
{
    m_appService.authorizeWithGoogle();
}

void LogicCore::startNewTask(const QString &taskName, const QString &projectId)
{
    m_currentTask = std::make_shared<Task>();

    m_taskService.createTask(taskName, projectId, m_currentTask, [this](bool success, QString info){
        qCDebug(logicCore) << QString("Create task success: %0, info: %1").arg(success).arg(info);
        if (success)
        {
            this->startExistTask(m_currentTask->id);
        }

    });
}

void LogicCore::stopTask()
{
    m_timeService.stopTimeEntry(m_currentTimeEntry, [this](bool success, QString info){
        qCDebug(logicCore) << QString("Stop task success: %0, info: %1").arg(success).arg(info);
        if (success)
        {
            if (m_timerId != -1)
            {
                killTimer(m_timerId);
                m_timerId = -1;
            }
            m_running = false;
            emit this->runningChanged(m_running);
            emit this->currentTaskChanged(QVariantMap());
            m_timerDuration = "00:00:00";
            emit this->timerDurationChanged(m_timerDuration);
            m_tasksModel->addItem(m_currentTask);
            m_timeEntriesModel->addItem(m_currentTimeEntry);
        }
    });
}

void LogicCore::startExistTask(const QString &taskId)
{
    if (m_currentTask == nullptr)
        m_currentTask = m_tasksModel->getItem(taskId);

    m_currentTimeEntry = std::make_shared<TimeEntry>();

    m_timeService.startTask(taskId, m_currentTimeEntry, [this](bool success, QString info){
        qCDebug(logicCore) << QString("Start task success: %0, info: %1").arg(success).arg(info);
        if (success)
        {
            if (m_timerId != -1)
            {
                killTimer(m_timerId);
                m_timerId = -1;
            }

            emit this->currentTaskChanged(this->currentTask());

            m_running = true;
            emit this->runningChanged(m_running);
            this->updateTimerDuration();

            m_timerId = startTimer(1000);
        }
    });
}


void LogicCore::deleteTask(const QString &taskId)
{
    m_taskService.removeTask(taskId, [this, taskId](bool success, QString info){
        if(success)
            m_tasksModel->removeItem(taskId);
        else
            qCDebug(logicCore) << "Task delete error:" << info;
    });
}

void LogicCore::deleteTimeEntry(const QString &id)
{
    m_timeService.removeTimeEntry(id, [this, id](bool success, QString info){
        qCDebug(logicCore) << QString("Remove time entry success: %0, info: %1").arg(success).arg(info);
        if (success)
            m_timeEntriesModel->removeItem(id);
    });
}

void LogicCore::updateTask(const QString &taskId, const QString &name, const QString &projectId, const QString &description)
{
    QVariantMap params;
    params.insert(m_tasksModel->roleNames().value(TasksModel::Roles::ItemIdRole), taskId);
    params.insert(m_tasksModel->roleNames().value(TasksModel::Roles::NameRole), name);
    params.insert(m_tasksModel->roleNames().value(TasksModel::Roles::ProjectRole), projectId);
    params.insert(m_tasksModel->roleNames().value(TasksModel::Roles::DescriptionRole), description);

    m_taskService.updateTask(taskId, params, [this, params](bool success, QString info){
        if(success) {
            int ind = m_tasksModel->getIndex(params["itemId"].toString());

            QModelIndex index = m_tasksModel->index(ind);
            m_tasksModel->setData(index,params["name"],TasksModel::Roles::NameRole);
            m_tasksModel->setData(index,params["projectId"],TasksModel::Roles::ProjectRole);
            m_tasksModel->setData(index,params["description"],TasksModel::Roles::DescriptionRole);
        }
    });
}

void LogicCore::updateTimeEntry()
{
    //TODO: implement
}

void LogicCore::setProjectForTask()
{
    //TODO: implement
}

void LogicCore::createNewProject(const QString &name, const QString &workspaceId)
{
    m_projService.createProject(name, workspaceId, [this, name, workspaceId](bool success, QString id){
        if(success)
        {
            m_projectModel->addItem(id, name, workspaceId);
        }
    });
}

void LogicCore::moveProjectToWorkspace()
{
    //TODO: implement
}

void LogicCore::updateProject(const QString &id, const QString &name)
{
    QVariantMap params;
    params.insert(m_projectModel->roleNames().value(ProjectsModel::Roles::ItemIdRole), id);
    params.insert(m_projectModel->roleNames().value(ProjectsModel::Roles::NameRole), name);

    m_projService.updateProject(id, params,[this, params](bool success, QString info){
        if(success) {
            int ind = m_projectModel->getIndex(params["itemId"].toString());

            QModelIndex index = m_projectModel->index(ind);
            m_projectModel->setData(index,params["name"],ProjectsModel::Roles::NameRole);
        }
    });
}

void LogicCore::deleteProject(const QString &projectId)
{
    m_projService.removeProject(projectId, [this, projectId](bool success, QString info){
        if(success)
            m_projectModel->removeItem(projectId);
        else
            qCDebug(logicCore) << "Project delete error:" << info;
    });
}

void LogicCore::setProjectAsDefault()
{
    //TODO: implement
}

void LogicCore::createNewWorkspace(const QString &name)
{
    QVariantMap params {
        {"name", name}
    };

    m_wsService.createWorkspace(m_currentUser->id, params, [this, name](bool success, QString id){
        if(success)
        {
            m_workspacesModel->addItem(id, name, m_currentUser->id);
        }
    });
}

void LogicCore::updateWorkspace(const QString &id, const QString &name)
{
    QVariantMap params;
    params.insert(m_workspacesModel->roleNames().value(WorkspacesModel::Roles::NameRole),name);
    params.insert("description",""); //TODO: implement

    m_wsService.updateWorkspace(id, params, [this, id, params](bool success, QString info){
        if(success) {
            int ind = m_workspacesModel->getIndex(id);
            QModelIndex index = m_workspacesModel->index(ind);
            m_workspacesModel->setData(index,params["name"],WorkspacesModel::Roles::NameRole);
            //m_workspacesModel->setData(index,params["description"],WorkspacesModel::Roles::DescriptionRole);
        }
    });
}

void LogicCore::deleteWorkspace(const QString &workspaceId)
{
    m_wsService.removeWorkspace(workspaceId, [this, workspaceId](bool success, QString info){
        if(success)
            m_workspacesModel->removeItem(workspaceId);
        else
            qCDebug(logicCore) << "Workspace delete error:" << info;
    });
}

void LogicCore::setWorkspaceAsDefault()
{
    //TODO: implement
}

void LogicCore::setWorkspacesLoading(bool workspacesLoading)
{
    if (m_workspacesLoading == workspacesLoading)
        return;

    m_workspacesLoading = workspacesLoading;
    emit workspacesLoadingChanged(m_workspacesLoading);
}

void LogicCore::setProjectsLoading(bool projectsLoading)
{
    if (m_projectsLoading == projectsLoading)
        return;

    m_projectsLoading = projectsLoading;
    emit projectsLoadingChanged(m_projectsLoading);
}

void LogicCore::setTasksLoading(bool tasksLoading)
{
    if (m_tasksLoading == tasksLoading)
        return;

    m_tasksLoading = tasksLoading;
    emit tasksLoadingChanged(m_tasksLoading);
}

void LogicCore::updateWorkspacesModel()
{
    setWorkspacesLoading(true);
    m_wsService.getAllWorkspaces(m_currentUser->id, m_workspacesModel, [this](bool succes, QString info){
        qCDebug(logicCore) << QString("Update workspaces success: %0, info: %1").arg(succes).arg(info);
        emit this->workspacesModelChanged();
        setWorkspacesLoading(false);
    });
}

void LogicCore::updateProjectsModel()
{
    setProjectsLoading(true);
    m_projService.getAllProjects(m_currentUser->id, m_projectModel, [this](bool succes, QString info){
        qCDebug(logicCore) << QString("Update projects success: %0, info: %1").arg(succes).arg(info);
        emit this->projectsModelChanged();
        setProjectsLoading(false);
    });
}

void LogicCore::updateTasksModel()
{
    setTasksLoading(true);
    m_taskService.getAllTasks(m_currentUser->id, m_tasksModel, [this](bool succes, QString info){
        qCDebug(logicCore) << QString("Update tasks success: %0, info: %1").arg(succes).arg(info);
        emit this->tasksModelChanged();
        setTasksLoading(false);
    });
}

void LogicCore::updateTimeEntriesModel()
{
    setTasksLoading(true);
    m_timeService.getAllTimeEntries(m_currentUser->id, m_timeEntriesModel, [this](bool succes, QString info){
        qCDebug(logicCore) << QString("Update time entries success: %0, info: %1").arg(succes).arg(info);
        emit this->timeEntriesModelChanged();
        setTasksLoading(false);
    });
}

void LogicCore::updateTimerDuration()
{
    qint64 duration = std::abs(QDateTime::currentDateTime().toMSecsSinceEpoch() - m_currentTimeEntry->startMSecsSinceEpoch);

    QTime time(0, 0, 0, 0);
    time = time.addMSecs(static_cast<int>(duration));
    m_timerDuration = time.toString("hh:mm:ss");

    emit this->timerDurationChanged(m_timerDuration);
}


void LogicCore::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId)
    {
        updateTimerDuration();
    }
}

void LogicCore::onAuthorizedSuccess(const QString &id, const QString &username, const QString &email)
{
    qCDebug(logicCore) << "Authorization success";
    m_currentUser->id = id;
    if (username.contains('@') && username.endsWith(".com"))
        m_currentUser->name = username.split("@").first();
    m_currentUser->email = email;
    updateAll();
    emit this->logined();
    emit this->userNameChanged();
    emit this->userEmailChanged();
}

void LogicCore::onAuthorizedFailed(const QString &errorString)
{
    qCDebug(logicCore) << "Authorization failed" << errorString;
}

void LogicCore::updateAll()
{
    setWorkspacesLoading(true);
    setProjectsLoading(true);
    setTasksLoading(true);
    m_appService.updateAllEntities(m_currentUser->id, m_workspacesModel, m_projectModel, m_tasksModel, m_timeEntriesModel, [this](bool succes, QString info){
        emit this->workspacesModelChanged();
        emit this->projectsModelChanged();
        emit this->tasksModelChanged();
        emit this->timeEntriesModelChanged();

        setWorkspacesLoading(false);
        setProjectsLoading(false);
        setTasksLoading(false);
        qCDebug(logicCore) << QString("Update all entities success: %0, info: %1").arg(succes).arg(info);
    });
}
