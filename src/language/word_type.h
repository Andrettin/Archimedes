#pragma once

#include "util/enum_converter.h"

namespace archimedes {

enum class word_type {
	none = -1,
	noun,
	verb,
	adjective,
	pronoun,
	adverb,
	conjunction,
	adposition,
	article,
	numeral,
	affix
};

extern template class enum_converter<word_type>;

inline std::string word_type_to_name(const word_type type)
{
	switch (type) {
		case word_type::none:
			return "None";
		case word_type::noun:
			return "Noun";
		case word_type::verb:
			return "Verb";
		case word_type::adjective:
			return "Adjective";
		case word_type::pronoun:
			return "Pronoun";
		case word_type::adverb:
			return "Adverb";
		case word_type::conjunction:
			return "Conjunction";
		case word_type::adposition:
			return "Adposition";
		case word_type::article:
			return "Article";
		case word_type::numeral:
			return "Numeral";
		case word_type::affix:
			return "Affix";
		default:
			break;
	}

	throw std::runtime_error("Invalid word type: \"" + std::to_string(static_cast<int>(type)) + "\".");
}

}

Q_DECLARE_METATYPE(archimedes::word_type)
