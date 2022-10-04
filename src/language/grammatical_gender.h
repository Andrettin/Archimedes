#pragma once

#include "util/enum_converter.h"

namespace archimedes {

enum class grammatical_gender {
	none = -1,
	masculine,
	feminine,
	neuter
};

extern template class enum_converter<grammatical_gender>;

inline std::string grammatical_gender_to_name(const grammatical_gender gender)
{
	switch (gender) {
		case grammatical_gender::none:
			return "None";
		case grammatical_gender::masculine:
			return "Masculine";
		case grammatical_gender::feminine:
			return "Feminine";
		case grammatical_gender::neuter:
			return "Neuter";
		default:
			break;
	}

	throw std::runtime_error("Invalid grammatical gender: \"" + std::to_string(static_cast<int>(gender)) + "\".");
}

}

Q_DECLARE_METATYPE(archimedes::grammatical_gender)
