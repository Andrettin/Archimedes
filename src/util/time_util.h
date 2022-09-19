#pragma once

namespace archimedes {
	template <int N>
	class fractional_int;

	using decimillesimal_int = fractional_int<4>;
}

namespace archimedes::time {

extern decimillesimal_int to_number(const QTime &time);

}
