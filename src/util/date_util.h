#pragma once

namespace archimedes {
	class timeline;
}

namespace archimedes::date {

static constexpr int months_per_year = 12;
static constexpr int days_per_year = 365;
static constexpr int hours_per_day = 24;

extern std::string year_to_string(const int year);

inline std::string year_to_labeled_string(const int year)
{
	std::string str = date::year_to_string(year);

	if (year < 0) {
		str += " BC";
	}

	return str;
}

inline std::string to_string(const QDate &date)
{
	if (!date.isValid()) {
		throw std::runtime_error("Date is not valid, and cannot be converted to a string.");
	}

	return date::year_to_string(date.year()) + '.' + std::to_string(date.month()) + '.' + std::to_string(date.day());
}

inline std::string to_string(const QDateTime &date_time)
{
	if (!date_time.isValid()) {
		throw std::runtime_error("Date time is not valid, and cannot be converted to a string.");
	}

	const QDate date = date_time.date();
	const QTime time = date_time.time();

	return date::to_string(date) + '.' + std::to_string(time.hour());
}

inline int get_days_in_month(const int month, const int year)
{
	return QDate(year, month, 1).daysInMonth();
}

extern bool contains_date(const QDate &date, const timeline *timeline, const QDate &other_date, const archimedes::timeline *other_timeline = nullptr);

inline std::string_view get_month_season_string(const int month)
{
	const int season = month / 3 + 1;

	switch (season) {
		case 1:
			return "Winter";
		case 2:
			return "Spring";
		case 3:
			return "Summer";
		case 4:
			return "Autumn";
	}

	throw std::runtime_error(std::format("Could not get season string for month {}.", month));
}

}
