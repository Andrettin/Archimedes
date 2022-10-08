#include "archimedes.h"

#include "util/image_provider_base.h"

#include "util/assert_util.h"

namespace archimedes {

QImage image_provider_base::requestImage(const QString &id, QSize *size, const QSize &requested_size)
{
	Q_UNUSED(requested_size);

	const std::string id_str = id.toStdString();

	const QImage &image = this->get_image(id_str);

	assert_throw(!image.isNull());

	if (size != nullptr) {
		*size = image.size();
	}

	return image;
}

const QImage &image_provider_base::get_image(const std::string &id)
{
	auto find_iterator = this->image_map.find(id);

	if (find_iterator != this->image_map.end()) {
		return find_iterator->second;
	}

	this->load_image(id);

	find_iterator = this->image_map.find(id);
	assert_throw(find_iterator != this->image_map.end());
	return find_iterator->second;
}

}
