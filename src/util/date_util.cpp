#include "archimedes.h"

#include "util/date_util.h"

#include "time/timeline.h"
#include "util/assert_util.h"
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

std::string year_to_labeled_string(const int year, const std::string_view &year_label, const std::string_view &negative_year_label)
{
	std::string str = date::year_to_string(year);

	if (year < 0) {
		str += " ";
		str += negative_year_label;
	} else if (!year_label.empty()) {
		str += " ";
		str += year_label;
	}

	return str;
}

std::string year_range_to_labeled_string(const int start_year, const int end_year, const std::string_view &year_label, const std::string_view &negative_year_label)
{
	assert_throw(end_year >= start_year);

	if (start_year < 0 && end_year >= 0) {
		return std::format("{} - {}", date::year_to_labeled_string(start_year, year_label, negative_year_label), date::year_to_labeled_string(end_year, year_label, negative_year_label));
	}

	std::string str = std::format("{}-{}", date::year_to_string(start_year), date::year_to_string(end_year));

	if (start_year < 0) {
		str += " ";
		str += negative_year_label;
	} else if (!year_label.empty()) {
		str += " ";
		str += year_label;
	}

	return str;
}

bool contains_date(const QDate &date, const timeline *timeline, const QDate &other_date, const archimedes::timeline *other_timeline)
{
	if (timeline == other_timeline) {
		return other_date <= date;
	} else if (timeline == nullptr) {
		return false;
	}

	return timeline->contains_timeline_date(other_timeline, other_date);
}

}
