#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "logiccore.h"
#include "translationservice.h"

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
    TranslationService translationService;
    rootContext->setContextProperty("core", &core);
    rootContext->setContextProperty("translator", &translationService);

    translationService.loadTranslation();

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
