#pragma once

namespace archimedes {

enum class gender;

enum class grammatical_gender {
	none = -1,
	masculine,
	feminine,
	neuter,
	common //common gender, which exists in languages which have a common-neuter gender divide (the common gender being used instead of the masculine and feminine genders)
};

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
		case grammatical_gender::common:
			return "Common";
		default:
			break;
	}

	throw std::runtime_error(std::format("Invalid grammatical gender: \"{}\".", std::to_underlying(gender)));
}

extern gender grammatical_gender_to_gender(const grammatical_gender grammatical_gender);

}

Q_DECLARE_METATYPE(archimedes::grammatical_gender)
