#include "archimedes.h"

#include "util/string_conversion_util.h"

#include "util/assert_util.h"
#include "util/string_util.h"
#include "util/time_util.h"

namespace archimedes::string {

QDateTime to_date_time(const std::string &date_str)
{
	const std::vector<std::string> date_string_list = string::split(date_str, '.');

	int years = 0;
	int months = 1;
	int days = 1;
	int hours = 12;

	if (date_string_list.size() >= 1) {
		years = std::stoi(date_string_list[0]);

		if (date_string_list.size() >= 2) {
			months = std::stoi(date_string_list[1]);

			if (date_string_list.size() >= 3) {
				days = std::stoi(date_string_list[2]);

				if (date_string_list.size() >= 4) {
					hours = std::stoi(date_string_list[3]);
				}
			}
		}
	}

	QDateTime date(QDate(years, months, days), QTime(hours, 0), QTimeZone(QTimeZone::UTC));

	if (!date.isValid()) {
		throw std::runtime_error(std::format("Date \"{}\" is not a valid date.", date_str));
	}

	return date;
}

QDate to_date(const std::string &date_str)
{
	const std::vector<std::string> date_string_list = string::split(date_str, '.');

	int years = 0;
	int months = 1;
	int days = 1;

	if (date_string_list.size() >= 1) {
		years = std::stoi(date_string_list[0]);

		if (date_string_list.size() >= 2) {
			months = std::stoi(date_string_list[1]);

			if (date_string_list.size() >= 3) {
				days = std::stoi(date_string_list[2]);
			}
		}
	}

	QDate date(years, months, days);

	if (!date.isValid()) {
		throw std::runtime_error(std::format("Date \"{}\" is not a valid date.", date_str));
	}

	return date;
}

QTime to_time(const std::string &time_str)
{
	const std::vector<std::string> time_string_list = string::split(time_str, '.');

	int hours = 0;
	int minutes = 0;
	int seconds = 0;
	int milliseconds = 0;

	if (time_string_list.size() >= 1) {
		hours = std::stoi(time_string_list[0]);

		if (time_string_list.size() >= 2) {
			minutes = std::stoi(time_string_list[1]);

			if (time_string_list.size() >= 3) {
				seconds = std::stoi(time_string_list[2]);

				if (time_string_list.size() >= 4) {
					milliseconds = std::stoi(time_string_list[3]);
				}
			}
		}
	}

	QTime time(hours, minutes, seconds, milliseconds);

	if (!time.isValid()) {
		throw std::runtime_error(std::format("Time \"{}\" is not a valid time.", time_str));
	}

	return time;
}

std::chrono::seconds to_duration(const std::string &str)
{
	const auto [number_str, unit_str] = string::to_number_string_and_unit_string(str);

	const int64_t number = std::stoll(number_str);

	assert_throw(!unit_str.empty());

	const auto find_iterator = time::time_units_by_short_name.find(unit_str);
	assert_throw(find_iterator != time::time_units_by_short_name.end());

	const time::time_unit time_unit = find_iterator->second;

	switch (time_unit) {
		case time::time_unit::seconds:
			return std::chrono::seconds(number);
		case time::time_unit::minutes:
			return std::chrono::minutes(number);
		case time::time_unit::hours:
			return std::chrono::hours(number);
		default:
			assert_throw(false);
			break;
	}

	return {};
}

std::pair<std::string, std::string> to_number_string_and_unit_string(const std::string &str)
{
	size_t suffix_pos = std::string::npos;
	bool has_suffix = false;

	for (int i = (static_cast<int>(str.size()) - 1); i >= 0; --i) {
		const char c = str[i];

		if (!std::isdigit(c)) {
			has_suffix = true;
			continue;
		}

		if (!has_suffix) {
			break;
		}

		suffix_pos = i + 1;
		break;
	}

	if (suffix_pos == std::string::npos) {
		return { str, "" };
	}

	const std::string number_str = str.substr(0, suffix_pos);
	const std::string suffix = str.substr(suffix_pos);

	return { number_str, suffix };
}

std::pair<int, int> to_numerator_and_denominator(const std::string &str)
{
	const size_t separator_pos = str.find("/");
	const int numerator = std::stoi(str.substr(0, separator_pos));
	const int denominator = std::stoi(str.substr(separator_pos + 1, str.size() - separator_pos - 1));
	return { numerator, denominator };
}

}
