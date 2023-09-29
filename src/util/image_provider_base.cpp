#include "archimedes.h"

#include "util/image_provider_base.h"

#include "util/assert_util.h"
#include "util/exception_util.h"
#include "util/log_util.h"
#include "util/thread_pool.h"

namespace archimedes {

class image_response final : public QQuickImageResponse
{
public:
	explicit image_response(image_provider_base *provider, const std::string &id)
		: provider(provider), id(id)
	{
	}

	virtual QQuickTextureFactory *textureFactory() const override
	{
		assert_throw(image != nullptr);
		return QQuickTextureFactory::textureFactoryForImage(*image);
	}

	QCoro::Task<void> run()
	{
		this->image = co_await this->provider->get_image(id);
		assert_throw(image != nullptr);
		assert_throw(!image->isNull());
		emit finished();
	}

private:
	image_provider_base *provider = nullptr;
	std::string id;
	const QImage *image = nullptr;
};

QQuickImageResponse *image_provider_base::requestImageResponse(const QString &id, const QSize &requested_size)
{
	Q_UNUSED(requested_size);

	try {
		const std::string id_str = id.toStdString();
		image_response *response = new image_response(this, id_str);
		response->run();
		return response;
	} catch (...) {
		exception::report(std::current_exception());
		std::terminate();
	}
}

QCoro::Task<const QImage *> image_provider_base::get_image(const std::string &id)
{
	{
		std::lock_guard lock(this->mutex);

		const auto find_iterator = this->image_map.find(id);

		if (find_iterator != this->image_map.end()) {
			co_return &find_iterator->second;
		}
	}

	log_trace(std::format("Loading image for identifier \"{}\".", id));
	co_await this->load_image(id);

	{
		std::lock_guard lock(this->mutex);

		const auto find_iterator = this->image_map.find(id);
		assert_throw(find_iterator != this->image_map.end());
		co_return &find_iterator->second;
	}
}

void image_provider_base::set_image(const std::string &id, QImage &&image)
{
	std::lock_guard lock(this->mutex);

	if (this->image_map.contains(id)) {
		return;
	}

	this->image_map[id] = std::move(image);
}

}
