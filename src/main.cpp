#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "logiccore.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setApplicationName("TimeTracker");
    QCoreApplication::setOrganizationName("TTTPU");
    QCoreApplication::setOrganizationDomain("tttpu.tk");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QQmlContext* rootContext = engine.rootContext();

    LogicCore core;
    rootContext->setContextProperty("core", &core);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
