#include "archimedes.h"

#include "util/colorization_type.h"

namespace archimedes {

template class enum_converter<colorization_type>;

template <>
const std::string enum_converter<colorization_type>::property_class_identifier = "archimedes::colorization_type";

template <>
const std::map<std::string, colorization_type> enum_converter<colorization_type>::string_to_enum_map = {
	{ "none", colorization_type::none },
	{ "blue", colorization_type::blue },
	{ "gray", colorization_type::gray },
	{ "green", colorization_type::green },
	{ "red", colorization_type::red },
	{ "white", colorization_type::white },
	{ "yellow", colorization_type::yellow }

};

template <>
const bool enum_converter<colorization_type>::initialized = enum_converter::initialize();

}
