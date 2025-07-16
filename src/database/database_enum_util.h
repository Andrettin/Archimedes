#pragma once

#include <magic_enum/magic_enum.hpp>

namespace archimedes::database_util {
	template <typename enum_type>
	void register_enum()
	{
		database::get()->register_string_to_qvariant_conversion(QMetaType::fromType<enum_type>().name(), [](const std::string &value) {
			return QVariant::fromValue(magic_enum::enum_cast<enum_type>(value).value());
		});
	}
}
