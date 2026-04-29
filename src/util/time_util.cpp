#include "archimedes.h"

#include "util/time_util.h"

#include "util/decimillesimal_int.h"

namespace archimedes::time {

const std::map<std::string, time_unit> time_units_by_short_name = {
	{ "s", time_unit::seconds },
	{ "min", time_unit::minutes },
	{ "h", time_unit::hours },
	{ "d", time_unit::days },
	{ "mo", time_unit::months },
	{ "y", time_unit::years }
};

decimillesimal_int to_number(const QTime &time)
{
	int64_t n = time.hour() * decimillesimal_int::divisor;
	n += time.minute() * decimillesimal_int::divisor / 60;
	n += (time.second() * decimillesimal_int::divisor + time.msec() * (decimillesimal_int::divisor / 1000)) / 60 / 60;

	return decimillesimal_int::from_value(n);
}

}
