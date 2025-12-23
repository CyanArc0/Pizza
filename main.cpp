#include "Bridge.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/Pizza.ico"));

    QQmlApplicationEngine engine;

    Bridge bridge;
    engine.rootContext()->setContextProperty("bridge",&bridge);

    QObject::connect(&engine,&QQmlApplicationEngine::objectCreationFailed,&app,[](){QCoreApplication::exit(-1);},Qt::QueuedConnection);
    engine.loadFromModule("Pizza", "UI");

    return app.exec();
}
