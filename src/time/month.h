#pragma once

namespace archimedes {

enum class month {
	january = 1,
	february = 2,
	march = 3,
	april = 4,
	may = 5,
	june = 6,
	july = 7,
	august = 8,
	september = 9,
	october = 10,
	november = 11,
	december = 12
};

inline month string_to_month(const std::string &str)
{
	if (str == "january") {
		return month::january;
	} else if (str == "february") {
		return month::february;
	} else if (str == "march") {
		return month::march;
	} else if (str == "april") {
		return month::april;
	} else if (str == "may") {
		return month::may;
	} else if (str == "june") {
		return month::june;
	} else if (str == "july") {
		return month::july;
	} else if (str == "august") {
		return month::august;
	} else if (str == "september") {
		return month::september;
	} else if (str == "october") {
		return month::october;
	} else if (str == "november") {
		return month::november;
	} else if (str == "december") {
		return month::december;
	}

	throw std::runtime_error("Invalid month: \"" + str + "\".");
}

}
