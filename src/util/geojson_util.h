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

}
