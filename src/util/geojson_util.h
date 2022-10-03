#pragma once

namespace archimedes::geojson {

extern std::vector<QVariantList> parse_folder(const std::filesystem::path &path);

template <typename function_type>
inline void process_features(const std::vector<QVariantList> &geojson_data_list, const function_type &processing_function)
{
	for (const QVariantList &geojson_data : geojson_data_list) {
		const QVariantMap feature_collection = geojson_data.front().toMap();
		const QVariantList feature_collection_data = feature_collection.value("data").toList();

		for (const QVariant &feature_variant : feature_collection_data) {
			const QVariantMap feature = feature_variant.toMap();
			processing_function(feature);
		}
	}
}

template <typename function_type>
inline void process_feature_data(const QVariantMap &feature_data, const function_type &processing_function)
{
	const QString type_str = feature_data.value("type").toString();

	if (type_str == "MultiLineString" || type_str == "MultiPolygon" || type_str == "MultiPoint") {
		for (const QVariant &subfeature_variant : feature_data.value("data").toList()) {
			const QVariantMap subfeature = subfeature_variant.toMap();
			geojson::process_feature_data(subfeature, processing_function);
		}
	} else if (type_str == "LineString" || type_str == "Polygon" || type_str == "Point") {
		processing_function(feature_data);
	} else {
		throw std::runtime_error("Invalid GeoJSON feature type string: \"" + type_str.toStdString() + "\".");
	}
}

template <typename map_type, typename key_function_type, typename geopath_width_function_type>
inline map_type create_geodata_map(const std::vector<QVariantList> &geojson_data_list, const key_function_type &key_function, const geopath_width_function_type &geopath_width_function)
{
	map_type geodata_map{};

	geojson::process_features(geojson_data_list, [&](const QVariantMap &feature) {
		const QVariantMap properties = feature.value("properties").toMap();

		const map_type::key_type key = key_function(properties);

		geojson::process_feature_data(feature, [&](const QVariantMap &feature_data) {
			const QString type_str = feature_data.value("type").toString();

			std::unique_ptr<QGeoShape> geoshape;

			if (type_str == "LineString") {
				const QGeoPath geopath = feature_data.value("data").value<QGeoPath>();
				auto geopath_copy = std::make_unique<QGeoPath>(geopath);

				if constexpr (!std::is_same_v<geopath_width_function_type, std::nullptr_t>) {
					if (geopath_width_function != nullptr) {
						const int geopath_width = geopath_width_function(key);
						if (geopath_width != -1) {
							geopath_copy->setWidth(geopath_width);
						}
					}
				}

				geoshape = std::move(geopath_copy);
			} else if (type_str == "Polygon") {
				const QGeoPolygon geopolygon = feature_data.value("data").value<QGeoPolygon>();
				geoshape = std::make_unique<QGeoPolygon>(geopolygon);
			} else if (type_str == "Point") {
				const QGeoCircle geocircle = feature_data.value("data").value<QGeoCircle>();
				geoshape = std::make_unique<QGeoCircle>(geocircle);
			} else {
				throw std::runtime_error("Invalid GeoJSON feature type string: \"" + type_str.toStdString() + "\".");
			}

			geodata_map[key].push_back(std::move(geoshape));
		});
	});

	return geodata_map;
}

}
