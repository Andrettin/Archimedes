#include "archimedes.h"

#include "language/word_type.h"

namespace archimedes {

template class enum_converter<word_type>;

template <>
const enum_converter<word_type> enum_converter<word_type>::instance = enum_converter(
	"archimedes::word_type",
	{
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
	}
);

}
