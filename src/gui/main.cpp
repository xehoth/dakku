#include <QGuiApplication>
#include <gui/film.h>
#include <gui/engine.h>
#include <thread>
#include <chrono>

using namespace dakku;

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  GuiEngine engine;
  const QUrl url(u"qrc:/gui/main.qml"_qs);
  QObject::connect(
      &engine, &GuiEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);
  return app.exec();
}
