#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "workspacesmodel.h"
#include "projectsmodel.h"
#include "tasksmodel.h"
#include "webservice.h"
#include <memory>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setApplicationName("TimeTracker");
    QCoreApplication::setOrganizationName("TPU");
    QCoreApplication::setOrganizationDomain("tttpu.tk");

    QGuiApplication app(argc, argv);

    std::shared_ptr<Enteties::WorkspacesModel> model(new Enteties::WorkspacesModel);
    std::shared_ptr<Enteties::ProjectsModel> projectModel(new Enteties::ProjectsModel);
    std::shared_ptr<Enteties::TasksModel> taskModel(new Enteties::TasksModel);

    QString userId = "42cfb602-f544-4b1b-b01d-63cd6a0b644f";

    WebService service;

//    service.createWorkspace("myOwnWorkspace1", userId);

    service.getAllWorkspaces(userId, model, [&service, model](bool success){
        if (success)
        {
//            service.createProject("myOwnProject", model->getItem(0)->id);
        }
    });

    service.getAllProjects(userId, projectModel, [&service, projectModel] (bool success) {
        if (success)
        {
//            auto project = projectModel->getItem(0);
//            if (project)
//                service.createTask(project->id);
        }
    });

//    service.getAllTasks(userId, taskModel);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
