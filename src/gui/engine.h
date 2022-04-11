#ifndef DAKKU_GUI_ENGINE_H_
#define DAKKU_GUI_ENGINE_H_
#include <QQmlApplicationEngine>
#include <gui/film.h>
#include <thread>
#include <chrono>

namespace dakku {
class GuiEngine : public QQmlApplicationEngine {
  Q_OBJECT

 public:
  GuiEngine();

 public Q_SLOTS:
  void updateFilmImage(int id);

 public:
  FilmImageProvider *filmImageProvider{};
};
}  // namespace dakku
#endif