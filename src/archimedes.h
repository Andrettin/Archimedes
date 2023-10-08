#pragma once

#include "pch.h"

//forward declarations

namespace archimedes {
	class calendar;
	class language;
	class language_family;
	class map_projection;
	class timeline;
	class word;

	template <int N>
	class fractional_int;

	using decimal_int = fractional_int<1>;
	using centesimal_int = fractional_int<2>;
	using millesimal_int = fractional_int<3>;
	using decimillesimal_int = fractional_int<4>;
}
