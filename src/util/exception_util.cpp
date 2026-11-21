#include "archimedes.h"

#include "util/exception_util.h"

#include "util/aggregate_exception.h"
#include "util/log_util.h"

namespace archimedes::exception {

void report(const std::exception_ptr &exception)
{
	try {
		std::rethrow_exception(exception);
	} catch (const aggregate_exception &aggregate_exception) {
		for (const std::exception_ptr &contained_exception : aggregate_exception.get_exceptions()) {
			exception::report(contained_exception);
		}

		log::log_error(aggregate_exception.what());
	} catch (const std::exception &exception) {
		try {
			std::rethrow_if_nested(exception);
		} catch (...) {
			exception::report(std::current_exception());
		}

		log::log_error(exception.what());
	}
}

}
