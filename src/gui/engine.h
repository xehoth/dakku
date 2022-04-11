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
  [[nodiscard]] FilmImageProvider *getProvider() const;

 public Q_SLOTS:
  void updateFilmImage(int id);

 private:
  FilmImageProvider *filmImageProvider{};
};
}  // namespace dakku
#endif