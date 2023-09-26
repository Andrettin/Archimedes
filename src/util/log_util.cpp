#include "util/log_util.h"

#include "util/log_level.h"

namespace archimedes::log {

void log(const log_level level, const std::string_view &message)
{
	std::ostream *ostream = nullptr;

	switch (level) {
		case log_level::trace:
		case log_level::debug:
		case log_level::info:
			ostream = &std::cout;
			break;
		case log_level::warning:
		case log_level::error:
			ostream = &std::cerr;
			break;
	}

	*ostream << "[" << QDateTime::currentDateTime().toString(log::date_string_format).toStdString() << "] " << "[" << get_log_level_name(level) << "] " << message << '\n';
}

void log_error(const std::string_view &error_message)
{
	log(log_level::error, error_message);
}

void log_qt_message(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	std::string log_message;

	static const std::string default_category_name = "default";
	if (context.category != nullptr && context.category != default_category_name) {
		log_message += context.category;
		log_message += ": ";
	}

	log_message += msg.toStdString();

	if (context.file != nullptr) {
		log_message += " (";
		log_message += context.file;
		log_message += ": ";
		log_message += context.line;

		if (context.function != nullptr) {
			log_message += ", ";
			log_message += context.function;
		}

		log_message += ")";
	}

	switch (type) {
		case QtDebugMsg:
			log::log(log_level::debug, log_message);
			break;
		case QtInfoMsg:
			log::log(log_level::info, log_message);
			break;
		case QtWarningMsg:
		case QtCriticalMsg:
		case QtFatalMsg:
			log::log_error(log_message);
			break;
	}
}

}
