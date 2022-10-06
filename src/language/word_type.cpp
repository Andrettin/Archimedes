#include "archimedes.h"

#include "language/word_type.h"

namespace archimedes {

template class enum_converter<word_type>;

template <>
const std::string enum_converter<word_type>::property_class_identifier = "archimedes::word_type";

template <>
const std::map<std::string, word_type> enum_converter<word_type>::string_to_enum_map = {
	{ "none", word_type::none },
	{ "noun", word_type::noun },
	{ "verb", word_type::verb },
	{ "adjective", word_type::adjective },
	{ "pronoun", word_type::pronoun },
	{ "adverb", word_type::adverb },
	{ "conjunction", word_type::conjunction },
	{ "adposition", word_type::adposition },
	{ "article", word_type::article },
	{ "numeral", word_type::numeral },
	{ "affix", word_type::affix }
};

template <>
const bool enum_converter<word_type>::initialized = enum_converter::initialize();

}
