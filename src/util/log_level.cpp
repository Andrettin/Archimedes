#include "archimedes.h"

#include "util/log_level.h"

namespace archimedes {

template class enum_converter<log_level>;

template <>
const std::string enum_converter<log_level>::property_class_identifier = "archimedes::log_level";

template <>
const std::map<std::string, log_level> enum_converter<log_level>::string_to_enum_map = {
	{ "trace", log_level::trace },
	{ "debug", log_level::debug },
	{ "info", log_level::info },
	{ "warning", log_level::warning },
	{ "error", log_level::error }
};

template <>
const bool enum_converter<log_level>::initialized = enum_converter::initialize();

}
