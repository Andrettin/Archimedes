#pragma once

#include "util/qvariant_util.h"

namespace archimedes::map {

static inline const std::map<std::string, int> empty_string_to_int_map;

template <typename T>
inline std::vector<typename T::key_type> get_keys(const T &map)
{
	std::vector<typename T::key_type> keys;

	for (const auto &kv_pair : map) {
		keys.push_back(kv_pair.first);
	}

	return keys;
}

template <typename T>
inline std::vector<typename T::mapped_type> get_values(const T &map)
{
	std::vector<typename T::mapped_type> values;

	for (const auto &kv_pair : map) {
		values.push_back(kv_pair.second);
	}

	return values;
}

template <typename T>
inline void remove_value(T &map, const typename T::value_type &value)
{
	for (auto it = map.begin(); it != map.end();) {
		if ((*it).second == value) {
			it = map.erase(it);
		} else {
			it++;
		}
	}
}

template <typename T, typename function_type>
inline void remove_value_if(T &map, const function_type &function)
{
	for (auto it = map.begin(); it != map.end();) {
		if (function((*it).second)) {
			it = map.erase(it);
		} else {
			it++;
		}
	}
}

template <typename T>
inline QVariantList to_qvariant_list(const T &map)
{
	QVariantList qvariant_list;

	for (const auto &[key, value] : map) {
		QVariantMap qvariant_map;

		qvariant_map["key"] = qvariant::from_value<typename T::key_type>(key);
		qvariant_map["value"] = qvariant::from_value<typename T::mapped_type>(value);

		qvariant_list.push_back(std::move(qvariant_map));
	}

	return qvariant_list;
}

template <typename T>
inline QVariantList to_value_sorted_qvariant_list(const T &map)
{
	QVariantList qvariant_list = map::to_qvariant_list(map);

	std::sort(qvariant_list.begin(), qvariant_list.end(), [](const QVariant &lhs, const QVariant &rhs) {
		const QVariant lhs_value_variant = lhs.toMap()["value"];
		const QVariant rhs_value_variant = rhs.toMap()["value"];

		return lhs_value_variant.value<typename T::mapped_type>() > rhs_value_variant.value<typename T::mapped_type>();
	});

	return qvariant_list;
}

template <typename T>
inline QVariantMap to_qvariant_map(const T &map)
{
	QVariantMap qvariant_map;

	for (const auto &[key, value] : map) {
		QString key_qstr;

		if constexpr (std::is_same_v<typename T::key_type, std::string>) {
			key_qstr = QString::fromStdString(key);
		} else if constexpr (std::is_pointer_v<typename T::key_type>) {
			key_qstr = key->get_identifier_qstring();
		} else {
			key_qstr = key;
		}

		qvariant_map[key_qstr] = qvariant::from_value<typename T::mapped_type>(value);
	}

	return qvariant_map;
}

template <typename T>
inline std::vector<typename T::key_type> to_weighted_vector(const T &map)
{
	static_assert(std::is_integral_v<typename T::mapped_type>);

	std::vector<typename T::key_type> weighted_vector;

	for (const auto &[key, value] : map) {
		for (typename T::mapped_type i = 0; i < value; ++i) {
			weighted_vector.push_back(key);
		}
	}

	return weighted_vector;
}

}
