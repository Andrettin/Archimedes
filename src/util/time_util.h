#pragma once

namespace archimedes {
	template <int N>
	class fractional_int;

	using decimillesimal_int = fractional_int<4>;
}

namespace archimedes::time {

enum class time_unit
{
	seconds,
	minutes,
	hours
};

extern const std::map<std::string, time_unit> time_units_by_short_name;

extern decimillesimal_int to_number(const QTime &time);

}
