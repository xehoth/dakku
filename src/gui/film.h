#ifndef DAKKU_GUI_FILM_H_
#define DAKKU_GUI_FILM_H_
#include <core/fwd.h>
#include <QQuickImageProvider>

namespace dakku {

class FilmImageProvider : public QQuickImageProvider {
  Q_OBJECT
 public:
  FilmImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

  QPixmap requestPixmap(const QString &, QSize *size,
                        const QSize &requestedSize);

  void update(const Film &film);

 Q_SIGNALS:
  void updateGui(int id);

 private:
  QImage image;
  QPixmap pixmap;
  int width{};
  int height{};
  int currentIndex{};
};

}  // namespace dakku
#endif