#pragma once

#include "util/qvariant_util.h"

namespace archimedes::container {

template <typename T, typename U>
inline bool intersects_with(const T &container, const U &other_container)
{
	for (const typename T::value_type &element : container) {
		for (const typename U::value_type &other_element : other_container) {
			if (element == other_element) {
				return true;
			}
		}
	}

	return false;
}

template <typename T>
inline QVariantList to_qvariant_list(const T &container)
{
	QVariantList list;

	for (const typename T::value_type &element : container) {
		list.append(qvariant::from_value<typename T::value_type>(element));
	}

	return list;
}

template <typename T>
QStringList to_qstring_list(const T &string_container)
{
	static_assert(std::is_same_v<typename T::value_type, std::string> || std::is_same_v<typename T::value_type, std::filesystem::path>);

	QStringList qstring_list;

	if constexpr (std::is_same_v<typename T::value_type, std::filesystem::path>) {
		for (const std::filesystem::path &path : string_container) {
			qstring_list.push_back(path::to_qstring(path));
		}
	} else {
		for (const std::string &str : string_container) {
			qstring_list.push_back(QString::fromStdString(str));
		}
	}

	return qstring_list;
}

template <typename T, typename set_type = std::set<typename T::value_type>>
inline set_type to_set(const T &container)
{
	set_type set{};

	for (const typename T::value_type &element : container) {
		set.insert(element);
	}

	return set;
}

template <typename T>
inline std::queue<typename T::value_type> to_queue(const T &container)
{
	std::queue<typename T::value_type> queue;

	for (const typename T::value_type &element : container) {
		queue.push(element);
	}

	return queue;
}

template <typename T>
inline std::vector<typename T::value_type> to_vector(const T &container)
{
	std::vector<typename T::value_type> vector;
	vector.reserve(container.size());

	for (const typename T::value_type &element : container) {
		vector.push_back(element);
	}

	return vector;
}

}
