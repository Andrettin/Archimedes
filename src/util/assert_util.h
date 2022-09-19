#pragma once

namespace archimedes {

inline std::string get_source_relative_filepath_string(const std::string &filepath)
{
	size_t pos = filepath.find("src");
	if (pos != std::string::npos) {
		pos += 4;
		return filepath.substr(pos, filepath.size() - pos);
	}

	return filepath;
}

#ifdef __cpp_lib_source_location
inline std::string get_assert_message(const std::source_location &location)
{
	return "Assert failed at " + get_source_relative_filepath_string(location.file_name()) + ": " + std::to_string(location.line()) + ", " + location.function_name() + ".";
}
#endif

#ifdef __cpp_lib_source_location
inline void assert_throw(const bool check, const std::source_location &location = std::source_location::current())
#else
inline void assert_throw(const bool check)
#endif
{
	if (check) {
		return;
	}

#ifdef __cpp_lib_source_location
	throw std::runtime_error(get_assert_message(location));
#else
	throw std::runtime_error("Assert failed.");
#endif
}

#ifdef __cpp_lib_source_location
extern void assert_log(const bool check, const std::source_location &location = std::source_location::current());
#else
extern void assert_log(const bool check);
#endif

}
