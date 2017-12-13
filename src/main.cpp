#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

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

//    service.createWorkspace("Study", userId, [&service, model, projectModel, userId](bool success, QString info){
//        if (success)
//        {
//            service.getAllWorkspaces(userId, model, [&service, model, projectModel, userId](bool success, QString info){
//                if (success)
//                {
//                    for(int i = 1; i < 5; i++ )
//                        service.createProject("TPU #" + QString::number(i), model->getItem(0)->id,[&service,projectModel, userId](bool success, QString info){
//                        });
//                }
//            });


//        }
//    });


//    service.getAllTasks(userId, taskModel);


    service.getAllWorkspaces(userId, model, [&service, model, projectModel, userId](bool success, QString info){
        if (success)
        {
//            for(int i = 1; i < 5; i++ )
//            service.createProject("project #" + QString::number(i), model->getItem(0)->id,[&service,projectModel, userId](bool success, QString info){
//            });

            service.getAllProjects(userId, projectModel, [&service, projectModel] (bool success, QString info) {
            });
        }
    });


    QQmlApplicationEngine engine;

    QQmlContext* rootContext = engine.rootContext();
    rootContext->setContextProperty("projectModel", projectModel.get());
    rootContext->setContextProperty("workspacesModel", model.get());

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
