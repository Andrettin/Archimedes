#pragma once

namespace archimedes {
	template <int N>
	class fractional_int;
}

namespace archimedes::angle {

constexpr int precision = 4;
using number_type = fractional_int<precision>;

extern const number_type pi;
extern const number_type half_pi;

extern number_type degrees_to_radians(const number_type &degrees);
extern double degrees_to_radians(const double degrees);
extern number_type radians_to_degrees(const number_type &radians);
extern double radians_to_degrees(const double radians);

extern number_type gudermannian(const number_type &n);
extern number_type gudermannian_inverse(const number_type &degrees);

}
