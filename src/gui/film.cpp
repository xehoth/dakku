#include <gui/film.h>
#include <core/bounds.h>
#include <core/film.h>

namespace dakku {

QPixmap FilmImageProvider::requestPixmap(const QString &, QSize *size,
                                         const QSize &requestedSize) {
  if (size) {
    *size = QSize(width, height);
    return pixmap;
  }
  return pixmap.scaled(requestedSize, Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);
}

void FilmImageProvider::update(const Film &film) {
  std::vector<std::uint8_t> data;
  film.writeImageTo(data);
  image = QImage(data.data(), film.getCroppedPixelBounds().diagonal().x(),
                 film.getCroppedPixelBounds().diagonal().y(),
                 3 * film.getCroppedPixelBounds().diagonal().x(),
                 QImage::Format::Format_RGB888);
  pixmap.convertFromImage(image);

  Q_EMIT updateGui(currentIndex++);
}
}  // namespace dakku