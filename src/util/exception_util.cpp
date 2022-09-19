#include "archimedes.h"

#include "util/exception_util.h"

#include "util/log_util.h"

namespace archimedes::exception {

void report(const std::exception &exception)
{
	try {
		std::rethrow_if_nested(exception);
	} catch (const std::exception &nested_exception) {
		exception::report(nested_exception);
	}

	log::log_error(exception.what());
}

std::string to_string(const std::exception &exception)
{
	std::string str;

	try {
		std::rethrow_if_nested(exception);
	} catch (const std::exception &nested_exception) {
		str += exception::to_string(nested_exception) + '\n';
	}

	str += exception.what();

	return str;
}

}
