#pragma once

namespace archimedes::string {

inline bool to_bool(const std::string &str)
{
	if (str == "true" || str == "yes" || str == "1") {
		return true;
	} else if (str == "false" || str == "no" || str == "0") {
		return false;
	}

	throw std::runtime_error("Invalid string used for conversion to boolean: \"" + str + "\".");
}

inline std::string from_bool(const bool value)
{
	if (value) {
		return "true";
	} else {
		return "false";
	}
}

inline char to_character(const std::string &str)
{
	if (str.size() != 1) {
		throw std::runtime_error("Character string \"" + str + "\" has a string size different than 1.");
	}

	return str.front();
}

extern QDateTime to_date(const std::string &date_str);
extern QTime to_time(const std::string &time_str);

}
