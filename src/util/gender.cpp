#include "archimedes.h"

#include "util/gender.h"

namespace archimedes {

template class enum_converter<gender>;

template <>
const std::string enum_converter<gender>::property_class_identifier = "archimedes::gender";

template <>
const std::map<std::string, gender> enum_converter<gender>::string_to_enum_map = {
	{ "none", gender::none },
	{ "male", gender::male },
	{ "female", gender::female }
};

template <>
const bool enum_converter<gender>::initialized = enum_converter::initialize();

}
