#pragma once

namespace archimedes {

enum class day_of_the_week {
	monday = 1,
	tuesday = 2,
	wednesday = 3,
	thursday = 4,
	friday = 5,
	saturday = 6,
	sunday = 7
};

inline day_of_the_week string_to_day_of_the_week(const std::string &str)
{
	if (str == "monday") {
		return day_of_the_week::monday;
	} else if (str == "tuesday") {
		return day_of_the_week::tuesday;
	} else if (str == "wednesday") {
		return day_of_the_week::wednesday;
	} else if (str == "thursday") {
		return day_of_the_week::thursday;
	} else if (str == "friday") {
		return day_of_the_week::friday;
	} else if (str == "saturday") {
		return day_of_the_week::saturday;
	} else if (str == "sunday") {
		return day_of_the_week::sunday;
	}

	throw std::runtime_error("Invalid day of the week: \"" + str + "\".");
}

}
