#include "archimedes.h"

#include "util/time_util.h"

#include "util/fractional_int.h"

namespace archimedes::time {

decimillesimal_int to_number(const QTime &time)
{
	int64_t n = time.hour() * decimillesimal_int::divisor;
	n += time.minute() * decimillesimal_int::divisor / 60;
	n += (time.second() * decimillesimal_int::divisor + time.msec() * (decimillesimal_int::divisor / 1000)) / 60 / 60;

	return decimillesimal_int::from_value(n);
}

}
