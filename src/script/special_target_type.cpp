#include "archimedes.h"

#include "script/special_target_type.h"

namespace archimedes {

template class enum_converter<special_target_type>;

template <>
const std::string enum_converter<special_target_type>::property_class_identifier = "archimedes::special_target_type";

template <>
const std::map<std::string, special_target_type> enum_converter<special_target_type>::string_to_enum_map = {
	{ "root", special_target_type::root },
	{ "source", special_target_type::source },
	{ "previous", special_target_type::previous }
};

template <>
const bool enum_converter<special_target_type>::initialized = enum_converter::initialize();

}
