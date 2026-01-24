#pragma once

namespace archimedes {
	class timeline;
}

namespace archimedes::date {

static constexpr int months_per_year = 12;
static constexpr int days_per_year = 365;
static constexpr int hours_per_day = 24;

extern std::string year_to_string(const int year);
extern std::string year_to_labeled_string(const int year, const std::string_view &year_label = "", const std::string_view &negative_year_label = "BC");

inline std::string to_string(const QDate &date)
{
	if (!date.isValid()) {
		throw std::runtime_error("Date is not valid, and cannot be converted to a string.");
	}

	return std::format("{}.{}.{}", date.year(), date.month(), date.day());
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

inline std::string_view get_month_name(const int month)
{
	switch (month) {
		case 1:
			return "January";
		case 2:
			return "February";
		case 3:
			return "March";
		case 4:
			return "April";
		case 5:
			return "May";
		case 6:
			return "June";
		case 7:
			return "July";
		case 8:
			return "August";
		case 9:
			return "September";
		case 10:
			return "October";
		case 11:
			return "November";
		case 12:
			return "December";
	}

	throw std::runtime_error(std::format("Could not get name for month {}.", month));
}

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
