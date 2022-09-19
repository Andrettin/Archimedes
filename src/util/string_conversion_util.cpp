#include "archimedes.h"

#include "util/string_conversion_util.h"

#include "util/string_util.h"

namespace archimedes::string {

QDateTime to_date(const std::string &date_str)
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

	QDateTime date(QDate(years, months, days), QTime(hours, 0), Qt::UTC);

	if (!date.isValid()) {
		throw std::runtime_error("Date \"" + date_str + "\" is not a valid date.");
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
		throw std::runtime_error("Time \"" + time_str + "\" is not a valid time.");
	}

	return time;
}

}
