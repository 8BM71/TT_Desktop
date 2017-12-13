#ifndef LOGICCORE_H
#define LOGICCORE_H

#include <QObject>
#include "webservice.h"
#include "items.h"

class LogicCore : public QObject
{
    Q_OBJECT
public:
    explicit LogicCore(QObject *parent = nullptr);

    // Task, time entry methods
    void startNewTask();

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

signals:

public slots:


private:
    WebService m_webService;
    Enteties::User m_currentUser;
};

#endif // LOGICCORE_H
