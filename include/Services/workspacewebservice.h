#ifndef WORKSPACEWEBSERVICE_H
#define WORKSPACEWEBSERVICE_H

#include <QObject>
#include "workspacesmodel.h"
#include "httpsinglton.h"

using namespace Entities;

class WorkspaceWebService : public QObject
{
    Q_OBJECT
public:
    explicit WorkspaceWebService(QObject *parent = nullptr);

    void getAllWorkspaces(const QString &ownerId, WorkspacesModelPtr workspaceModel, SuccessCallback successCallback);

    /**
     * @brief createWorkspace
     * @param ownerId
     * @param params - QVariantMap:
     * "name" - new name of workspace (QString)
     * "description" - new description of ws (QString)
     * @param successCallback
     */
    void createWorkspace(const QString &ownerId, const QVariantMap &params, SuccessCallback successCallback);

    /**
     * @brief updateWorkspace
     * @param id
     * @param params - QVariantMap:
     * "name" - new name of workspace (QString)
     * "description" - new description of ws (QString)
     * @param successCallback
     */
    void updateWorkspace(const QString &id, const QVariantMap &params, SuccessCallback successCallback);

    void removeWorkspace(const QString &id, SuccessCallback successCallback);
};

#endif // WORKSPACEWEBSERVICE_H
