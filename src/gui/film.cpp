#include <gui/film.h>
#include <core/bounds.h>
#include <core/film.h>
#include <imageio/fwd.h>

namespace dakku {

QPixmap FilmImageProvider::requestPixmap(const QString &, QSize *size,
                                         const QSize &requestedSize) {
  if (size) {
    if (width == 0 && height == 0) {
      *size = QSize(1, 1);
      return QPixmap{1, 1};
    }
    *size = QSize(width, height);
    return pixmap;
  }
  return pixmap.scaled(requestedSize, Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);
}

void FilmImageProvider::setImage(const Film &film) {
  std::vector<std::uint8_t> data;
  film.writeImageTo(data);
  image = QImage(data.data(), film.getCroppedPixelBounds().diagonal().x(),
                 film.getCroppedPixelBounds().diagonal().y(),
                 3 * film.getCroppedPixelBounds().diagonal().x(),
                 QImage::Format::Format_RGB888);
  pixmap.convertFromImage(image);
  this->width = film.getCroppedPixelBounds().diagonal().x();
  this->height = film.getCroppedPixelBounds().diagonal().y();

  Q_EMIT updateGui(currentIndex++);
}

void FilmImageProvider::setImage(std::span<const float> data, int width,
                                 int height) {
  std::vector<std::uint8_t> img(3 * width * height);
  std::transform(data.begin(), data.end(), img.begin(), [](float v) {
    return static_cast<std::uint8_t>(
        std::clamp(gammaCorrect(v) * 256, 0.0f, 255.0f));
  });
  this->width = width;
  this->height = height;
  image = QImage(img.data(), width, height, 3 * width,
                 QImage::Format::Format_RGB888);
  pixmap.convertFromImage(image);

  Q_EMIT updateGui(currentIndex++);
}
}  // namespace dakku