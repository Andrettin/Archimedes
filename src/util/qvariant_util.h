#pragma once

#include "util/container_util.h"
#include "util/path_util.h"
#include "util/type_traits.h"

namespace archimedes {

namespace container {
	template <typename T>
	static QVariantList to_qvariant_list(const T &container);
}
	
namespace qvariant {

template <typename T>
static QVariant from_value(const T &value)
{
	if constexpr (std::is_same_v<T, std::filesystem::path>) {
		return QVariant::fromValue(path::to_qstring(value));
	} else if constexpr (std::is_same_v<T, std::string>) {
		return QVariant::fromValue(QString::fromStdString(value));
	} else if constexpr (std::is_pointer_v<T>) {
		using mutable_type = std::add_pointer_t<std::remove_const_t<std::remove_pointer_t<T>>>;

		if constexpr (std::is_same_v<T, mutable_type>) {
			return QVariant::fromValue(value);
		} else {
			return QVariant::fromValue(const_cast<mutable_type>(value));
		}
	} else if constexpr (is_specialization_of_v<T, std::unique_ptr>) {
		return QVariant::fromValue(value.get());
	} else if constexpr (is_specialization_of_v<T, std::vector>) {
		return container::to_qvariant_list(value);
	} else {
		return QVariant::fromValue(value);
	}
}

template <typename T>
static T *to_object(const QVariant &variant)
{
	QObject *object = qvariant_cast<QObject *>(variant);
	return qobject_cast<T *>(object);
}

}

}
