#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "nscanclient/nscanclient.hpp"

int main(int argc, char** argv)
{
  QGuiApplication app(argc, argv);

  qmlRegisterType<swatcher::NscanClient>("swatcher.NscanClient", 1, 0, "NscanClient");

  QQmlApplicationEngine engine;
  engine.load(QUrl{"qrc:/qml/main.qml"});

  return app.exec();
}