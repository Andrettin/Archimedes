#pragma once

#include "util/enum_converter.h"

namespace archimedes {

enum class log_level {
	trace,
	debug,
	info,
	warning,
	error
};

extern template class enum_converter<log_level>;

inline std::string_view get_log_level_name(const log_level level)
{
	switch (level) {
		case log_level::trace:
			return "Trace";
		case log_level::debug:
			return "Debug";
		case log_level::info:
			return "Info";
		case log_level::warning:
			return "Warning";
		case log_level::error:
			return "Error";
		default:
			break;
	}

	throw std::runtime_error(std::format("Invalid log level: \"{}\".", std::to_string(static_cast<int>(level))));
}

}

Q_DECLARE_METATYPE(archimedes::log_level)
