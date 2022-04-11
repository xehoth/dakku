#ifndef DAKKU_GUI_FILM_H_
#define DAKKU_GUI_FILM_H_
#include <core/fwd.h>
#include <QQuickImageProvider>
#include <span>

namespace dakku {

class FilmImageProvider : public QQuickImageProvider {
  Q_OBJECT
 public:
  FilmImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

  QPixmap requestPixmap(const QString &, QSize *size,
                        const QSize &requestedSize) override;

  void setImage(const Film &film);
  void setImage(std::span<const float> data, int width, int height);

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