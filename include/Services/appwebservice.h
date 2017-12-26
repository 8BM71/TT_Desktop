#ifndef APPWEBSERVICE_H
#define APPWEBSERVICE_H

#include <QObject>
#include "httpsinglton.h"

#include "workspacesmodel.h"
#include "projectsmodel.h"
#include "tasksmodel.h"
#include "timeentriesmodel.h"

using namespace Entities;

class AppWebService : public QObject
{
    Q_OBJECT
public:
    explicit AppWebService(QObject *parent = nullptr);

    void createUser(const QString &username, const QString &email, UserPtr user, SuccessCallback successCallback);

    void updateAllEntities(const QString &ownerId,
                           WorkspacesModelPtr workspaceModel,
                           ProjectsModelPtr projectModel,
                           TasksModelPtr taskModel,
                           TimeEntriesModelPtr timeModel,
                           SuccessCallback successCallback);
};

#endif // APPWEBSERVICE_H
