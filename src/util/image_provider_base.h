#pragma once

#pragma warning(push, 0)
#include <QQuickImageProvider>
#pragma warning(pop)

namespace archimedes {

class image_provider_base : public QQuickImageProvider
{
public:
	image_provider_base() : QQuickImageProvider(QQuickImageProvider::Image)
	{
	}

	virtual QImage requestImage(const QString &id, QSize *size, const QSize &requested_size) override;

	const QImage &get_image(const std::string &id);

	[[nodiscard]]
	virtual boost::asio::awaitable<void> load_image(const std::string &id) = 0;

	void set_image(const std::string &id, QImage &&image)
	{
		this->image_map[id] = std::move(image);
	}

	void clear_images()
	{
		this->image_map.clear();
	}

private:
	std::map<std::string, QImage> image_map;
};

}
