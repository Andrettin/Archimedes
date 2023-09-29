#pragma once

#pragma warning(push, 0)
#include <QQuickAsyncImageProvider>
#pragma warning(pop)

namespace archimedes {

class image_provider_base : public QQuickAsyncImageProvider
{
public:
	virtual QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requested_size) override;

	[[nodiscard]]
	QCoro::Task<const QImage *> get_image(const std::string &id);

	[[nodiscard]]
	virtual QCoro::Task<void> load_image(const std::string id) = 0;

	void set_image(const std::string &id, QImage &&image);

	void clear_images()
	{
		std::lock_guard lock(this->mutex);
		this->image_map.clear();
	}

private:
	std::mutex mutex;
	std::map<std::string, QImage> image_map;
};

}
