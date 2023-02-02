#pragma once

#include "util/enum_converter.h"

namespace archimedes {

enum class gender {
	none,
	male,
	female,

	count
};

extern template class enum_converter<gender>;

inline std::string get_gender_name(const gender gender)
{
	switch (gender) {
		case gender::none:
			return "None";
		case gender::male:
			return "Male";
		case gender::female:
			return "Female";
		default:
			break;
	}

	throw std::runtime_error("Invalid gender: \"" + std::to_string(static_cast<int>(gender)) + "\".");
}

inline std::string get_gender_personal_pronoun(const gender gender)
{
	switch (gender) {
		case gender::none:
			return "it";
		case gender::male:
			return "he";
		case gender::female:
			return "she";
		default:
			break;
	}

	throw std::runtime_error("Invalid gender: \"" + std::to_string(static_cast<int>(gender)) + "\".");
}

inline std::string get_gender_possessive_pronoun(const gender gender)
{
	switch (gender) {
		case gender::none:
			return "its";
		case gender::male:
			return "his";
		case gender::female:
			return "her";
		default:
			break;
	}

	throw std::runtime_error("Invalid gender: \"" + std::to_string(static_cast<int>(gender)) + "\".");
}

inline std::string get_gender_oblique_pronoun(const gender gender)
{
	switch (gender) {
		case gender::none:
			return "it";
		case gender::male:
			return "him";
		case gender::female:
			return "her";
		default:
			break;
	}

	throw std::runtime_error("Invalid gender: \"" + std::to_string(static_cast<int>(gender)) + "\".");
}

}

Q_DECLARE_METATYPE(archimedes::gender)
