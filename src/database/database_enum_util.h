#pragma once

#include <magic_enum/magic_enum.hpp>

namespace archimedes::database_util {
	template <typename enum_type>
	void register_enum()
	{
		database::get()->register_string_to_qvariant_conversion(QMetaType::fromType<enum_type>().name(), [](const std::string &value) {
			try {
				return QVariant::fromValue(magic_enum::enum_cast<enum_type>(value).value());
			} catch (...) {
				std::throw_with_nested(std::runtime_error(std::format("Failed to convert value \"{}\" for enum \"{}\".", value, QMetaType::fromType<enum_type>().name())));
			}
		});
	}
}
