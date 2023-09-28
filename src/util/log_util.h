#pragma once

#include "log_level.h"

namespace archimedes::log {

constexpr const char date_string_format[] = "yyyy.MM.dd hh:mm:ss";
constexpr uintmax_t max_size = 1000000; //1 MB
extern log_level min_log_level;

extern void log(const log_level level, const std::string_view &message);
extern void log_error(const std::string_view &error_message);
extern void log_qt_message(QtMsgType type, const QMessageLogContext &context, const QString &msg);

}

#define log_trace(message) \
	if (log::min_log_level <= log_level::trace) { \
		log::log(log_level::trace, message); \
	}

#define log_debug(message) \
	if (log::min_log_level <= log_level::debug) { \
		log::log(log_level::debug, message); \
	}

#define log_info(message) \
	if (log::min_log_level <= log_level::info) { \
		log::log(log_level::info, message); \
	}

#define log_warning(message) \
	if (log::min_log_level <= log_level::warning) { \
		log::log(log_level::warning, message); \
	}
