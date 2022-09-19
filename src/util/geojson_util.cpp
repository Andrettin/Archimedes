#include "archimedes.h"

#include "util/geojson_util.h"

#ifdef USE_GEOJSON
#pragma warning(push, 0)
#include <QtLocation/private/qgeojson_p.h>
#pragma warning(pop)
#endif

namespace archimedes::geojson {

std::vector<QVariantList> parse_folder(const std::filesystem::path &path)
{
#ifdef USE_GEOJSON
	std::vector<QVariantList> geojson_data_list;

	std::filesystem::recursive_directory_iterator dir_iterator(path);

	for (const std::filesystem::directory_entry &dir_entry : dir_iterator) {
		if (!dir_entry.is_regular_file() || dir_entry.path().extension() != ".geojson") {
			continue;
		}

		std::ifstream ifstream(dir_entry.path());

		if (!ifstream) {
			throw std::runtime_error("Failed to open file: " + dir_entry.path().string());
		}

		const std::string raw_geojson_data(std::istreambuf_iterator<char>{ifstream}, std::istreambuf_iterator<char>{});
		const QByteArray raw_geojson_byte_array = QByteArray::fromStdString(raw_geojson_data);

		QJsonParseError json_error;
		const QJsonDocument json = QJsonDocument::fromJson(raw_geojson_byte_array, &json_error);

		if (json.isNull()) {
			throw std::runtime_error("JSON parsing failed: " + json_error.errorString().toStdString() + ".");
		}

		QVariantList geojson_data = QGeoJson::importGeoJson(json);
		geojson_data_list.push_back(std::move(geojson_data));
	}

	return geojson_data_list;

#else
	throw std::runtime_error("GeoJSON support not enabled.");
#endif
}

}
