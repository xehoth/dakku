#include <gui/engine.h>
#include <QQmlContext>

namespace dakku {
GuiEngine::GuiEngine() {
  filmImageProvider = new FilmImageProvider;
  this->addImageProvider(QLatin1String("film"), filmImageProvider);
  QObject::connect(filmImageProvider, &FilmImageProvider::updateGui, this,
                   &GuiEngine::updateFilmImage);
}

void GuiEngine::updateFilmImage(int id) {
  printf("update");
  rootObjects().back()->findChild<QObject *>("film")->setProperty(
      "source", QString("image://film/") + QString::number(id));
}
}  // namespace dakku