#include "archimedes.h"

#include "util/assert_util.h"

#include "util/log_util.h"

namespace archimedes {

#ifdef __cpp_lib_source_location
void assert_log(const bool check, const std::source_location &location)
#else
void assert_log(const bool check)
#endif
{
	if (check) {
		return;
	}

#ifdef __cpp_lib_source_location
	log::log_error(get_assert_message(location));
#else
	log::log_error("Assert failed.");
#endif
}

}
