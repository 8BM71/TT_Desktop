#ifndef PROJECTWEBSERVICE_H
#define PROJECTWEBSERVICE_H

#include <QObject>
#include "projectsmodel.h"
#include "httpsinglton.h"

using namespace Entities;

class ProjectWebService : public QObject
{
    Q_OBJECT
public:
    explicit ProjectWebService(QObject *parent = nullptr);

    void getAllProjects(const QString &ownerId, ProjectsModelPtr projectModel, SuccessCallback successCallback);

    void createProject(const QString &name, const QString &workspaceId, SuccessCallback successCallback);

    /**
     * @brief updateProject
     * @param id
     * @param params - QVariantMap:
     * "name" - new name of Project (QString)
     * @param successCallback
     */
    void updateProject(const QString &id, const QVariantMap &params, SuccessCallback successCallback);

    void removeProject(const QString &id, SuccessCallback successCallback);

};

#endif // PROJECTWEBSERVICE_H
