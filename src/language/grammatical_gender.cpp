#include "archimedes.h"

#include "language/grammatical_gender.h"

namespace archimedes {

template class enum_converter<grammatical_gender>;

template <>
const std::string enum_converter<grammatical_gender>::property_class_identifier = "archimedes::grammatical_gender";

template <>
const std::map<std::string, grammatical_gender> enum_converter<grammatical_gender>::string_to_enum_map = {
	{ "none", grammatical_gender::none },
	{ "masculine", grammatical_gender::masculine },
	{ "feminine", grammatical_gender::feminine },
	{ "neuter", grammatical_gender::neuter }
};

template <>
const bool enum_converter<grammatical_gender>::initialized = enum_converter::initialize();

}
