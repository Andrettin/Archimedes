#pragma once

namespace archimedes::string_view {

inline std::vector<std::string_view> split(const std::string_view &str, const char delimiter)
{
	std::vector<std::string_view> string_list;

	size_t start_pos = 0;
	size_t find_pos = 0;
	while ((find_pos = str.find(delimiter, start_pos)) != std::string::npos) {
		std::string_view string_element = str.substr(start_pos, find_pos - start_pos);

		string_list.push_back(std::move(string_element));

		start_pos = find_pos + 1;
	}

	std::string_view string_element = str.substr(start_pos, str.length() - start_pos);

	string_list.push_back(std::move(string_element));

	return string_list;
}

}
