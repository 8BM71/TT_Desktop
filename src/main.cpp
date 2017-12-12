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
    QGuiApplication app(argc, argv);

    std::shared_ptr<Enteties::WorkspacesModel> model(new Enteties::WorkspacesModel);
    std::shared_ptr<Enteties::ProjectsModel> projectModel(new Enteties::ProjectsModel);
    std::shared_ptr<Enteties::TasksModel> taskModel(new Enteties::TasksModel);

    QString userId = "42cfb602-f544-4b1b-b01d-63cd6a0b644f";

    WebService service;
    service.getAllWorkspaces(userId, model);

    service.getAllProjects(userId, projectModel);

    service.getAllTasks(userId, taskModel);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
