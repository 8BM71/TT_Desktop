#include "logiccore.h"
#include <QDebug>
#include <QLoggingCategory>
#include <QMetaType>
#include <QTimerEvent>
#include <QDateTime>

Q_LOGGING_CATEGORY(logicCore, "LogicCore")

LogicCore::LogicCore(QObject *parent)
    : QObject(parent)
    , m_timerDuration("00:00:00")
    , m_running(false)
    , m_timerId(-1)
    , m_waiting(false)
{
    m_currentUser = std::make_shared<User>();
    m_workspacesModel = std::make_shared<WorkspacesModel>();
    m_projectModel = std::make_shared<ProjectsModel>();
    m_tasksModel = std::make_shared<TasksModel>();
    m_timeEntriesModel = std::make_shared<TimeEntriesModel>();

    m_currentUser->id = "e093e100-82aa-43a5-ba97-2812c710f716";
    updateAll();
//    m_webService.createUser("user", "user12@mail.ru", m_currentUser, [this](bool success, QString info){
//        qCDebug(logicCore) << QString("Create user success: %0, info: %1").arg(success).arg(info);
//    });
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

bool LogicCore::waiting() const
{
    return m_waiting;
}

QString LogicCore::currentTaskName() const
{
    return m_currentTaskName;
}

void LogicCore::startNewTask(const QString &taskName, const QString &projectId)
{
    if (m_waiting)
        return;
    m_waiting = true;
    emit this->waitingChanged(m_waiting);
    m_currentTask = std::make_shared<Task>();

    m_webService.createTask(taskName, projectId, m_currentTask, [this](bool success, QString info){
        qCDebug(logicCore) << QString("Create task success: %0, info: %1").arg(success).arg(info);
        if (success)
        {
            this->startExistTask(m_currentTask->id);
        }

    });
}

void LogicCore::stopTask()
{
    if (m_waiting)
        return;
    m_waiting = true;
    emit this->waitingChanged(m_waiting);
    m_webService.stopTimeEntry(m_currentTimeEntry, [this](bool success, QString info){
        qCDebug(logicCore) << QString("Stop task success: %0, info: %1").arg(success).arg(info);
        if (success)
        {
            if (m_timerId != -1)
            {
                killTimer(m_timerId);
                m_timerId = -1;
            }
            m_waiting = false;
            emit this->waitingChanged(m_waiting);
            m_running = false;
            emit this->runningChanged(m_running);
            this->setCurrentTaskName("");
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

    m_webService.startTask(taskId, m_currentTimeEntry, [this](bool success, QString info){
        qCDebug(logicCore) << QString("Start task success: %0, info: %1").arg(success).arg(info);
        if (success)
        {
            if (m_timerId != -1)
            {
                killTimer(m_timerId);
                m_timerId = -1;
            }

            this->setCurrentTaskName(m_currentTask->name);

            m_waiting = false;
            emit this->waitingChanged(m_waiting);
            m_running = true;
            emit this->runningChanged(m_running);
            this->updateTimerDuration();

            m_timerId = startTimer(1000);
        }
    });
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

void LogicCore::createNewProject(const QString &name, const QString &workspaceId)
{
    m_webService.createProject(name, workspaceId, [this, name, workspaceId](bool success, QString id){
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

void LogicCore::updateProject()
{
    //TODO: implement
}

void LogicCore::deleteProject(const QString &projectId)
{
    m_webService.deleteProject(projectId, [this, projectId](bool success, QString info){
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
    m_webService.createWorkspace(name, m_currentUser->id, [this, name](bool success, QString id){
        if(success)
        {
            m_workspacesModel->addItem(id, name, m_currentUser->id);
        }
    });
}

void LogicCore::updateWorkspace()
{
    //TODO: implement
}

void LogicCore::deleteWorkspace(const QString &workspaceId)
{
    m_webService.deleteWorkspace(workspaceId, [this, workspaceId](bool success, QString info){
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

void LogicCore::setCurrentTaskName(QString currentTaskName)
{
    if (m_currentTaskName == currentTaskName)
        return;

    m_currentTaskName = currentTaskName;
    emit currentTaskNameChanged(m_currentTaskName);
}

void LogicCore::updateWorkspacesModel()
{
    m_webService.getAllWorkspaces(m_currentUser->id, m_workspacesModel, [](bool succes, QString info){
        qCDebug(logicCore) << QString("Update workspaces success: %0, info: %1").arg(succes).arg(info);
    });
}

void LogicCore::updateProjectsModel()
{
    m_webService.getAllProjects(m_currentUser->id, m_projectModel, [this](bool succes, QString info){
        qCDebug(logicCore) << QString("Update projects success: %0, info: %1").arg(succes).arg(info);
        emit this->projectsModelChanged();
    });
}

void LogicCore::updateTasksModel()
{
    m_webService.getAllTasks(m_currentUser->id, m_tasksModel, [](bool succes, QString info){
        qCDebug(logicCore) << QString("Update tasks success: %0, info: %1").arg(succes).arg(info);
    });
}

void LogicCore::updateTimeEntriesModel()
{
    m_webService.getAllTimeEntries(m_currentUser->id, m_timeEntriesModel, [](bool succes, QString info){
        qCDebug(logicCore) << QString("Update time entries success: %0, info: %1").arg(succes).arg(info);
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

void LogicCore::updateAll()
{
    m_webService.updateAllEntities(m_currentUser->id, m_workspacesModel, m_projectModel, m_tasksModel, m_timeEntriesModel, [](bool succes, QString info){
        qCDebug(logicCore) << QString("Update all entities success: %0, info: %1").arg(succes).arg(info);
    });
}
