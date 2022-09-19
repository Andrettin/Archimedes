#include "archimedes.h"

#include "util/date_util.h"

#include "util/number_util.h"

namespace archimedes::date {

std::string year_to_string(const int year)
{
	const int abs_year = std::abs(year);

	if (abs_year < 10000) {
		//do not use a formatted string (which can have a comma separator) if the year number is less than 10,000
		return std::to_string(abs_year);
	}

	return number::to_formatted_string(abs_year);
}

}
