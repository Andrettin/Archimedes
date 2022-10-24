#pragma once

#pragma warning(push, 0)
#include <QQuickImageProvider>
#pragma warning(pop)

namespace archimedes {

class empty_image_provider final : public QQuickImageProvider
{
public:
	empty_image_provider() : QQuickImageProvider(QQuickImageProvider::Image)
	{
	}

	static QImage get_empty_image()
	{
		QImage image(QSize(1, 1), QImage::Format_ARGB32);
		image.fill(qRgba(0, 0, 0, 0));
		return image;
	}

	virtual QImage requestImage(const QString &id, QSize *size, const QSize &requested_size) override
	{
		Q_UNUSED(id);
		Q_UNUSED(requested_size);

		QImage image = empty_image_provider::get_empty_image();

		if (size != nullptr) {
			*size = image.size();
		}

		return image;
	}
};

}
