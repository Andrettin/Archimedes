#pragma once

#include "log_level.h"

namespace archimedes::log {

constexpr const char date_string_format[] = "yyyy.MM.dd hh:mm:ss";
constexpr uintmax_t max_size = 1000000; //1 MB
constexpr log_level min_log_level = log_level::trace;

extern void log(const log_level level, const std::string_view &message);
extern void log_error(const std::string_view &error_message);
extern void log_qt_message(QtMsgType type, const QMessageLogContext &context, const QString &msg);

}

#define log_trace(message) \
	if (log::min_log_level <= log_level::trace) { \
		log::log(log_level::trace, message); \
	}
