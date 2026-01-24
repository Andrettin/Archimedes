#pragma once

namespace archimedes::number {

template <typename number_type>
inline constexpr number_type sign(const number_type &n)
{
	static_assert(!std::is_unsigned_v<number_type>);

	if (n == 0) {
		return number_type(0);
	} else if (n < 0) {
		return number_type(-1);
	} else {
		return number_type(1);
	}
}

extern std::string to_formatted_string(const int number);

inline std::string to_signed_string(const int number)
{
	std::string number_str;
	if (number >= 0) {
		number_str += "+";
	}
	number_str += number::to_formatted_string(number);
	return number_str;
}

inline std::string_view get_ordinal_number_suffix(const int number)
{
	if (number > 12) {
		return number::get_ordinal_number_suffix(number % 10);
	}

	switch (number) {
		case 1:
			return "st";
		case 2:
			return "nd";
		case 3:
			return "rd";
		default:
			return "th";
	}
}

extern std::string to_roman_numeral(int number);

inline int fast_abs(const int value)
{
	const int temp = value >> 31;
	return (value ^ temp) - temp;
}

template <typename number_type>
inline constexpr number_type pow(const number_type base, const number_type exp)
{
	static_assert(std::is_integral_v<number_type>);

	number_type value = 1;

	for (number_type i = 0; i < exp; ++i) {
		value *= base;
	}

	return value;
}

//integer square root
//method by John Halleck, see http://www.cc.utah.edu/~nahaj/factoring/isqrt.legalize.c.html
template <typename number_type>
inline constexpr number_type sqrt(const number_type num)
{
	static_assert(std::is_integral_v<number_type>);

	using unsigned_type = std::conditional_t<std::is_unsigned_v<number_type>, number_type, std::make_unsigned_t<number_type>>;

	if (num < 1) {
		return 0;
	}

	//
	//  Load the binary constant 01 00 00 ... 00, where the number
	//  of zero bits to the right of the single one bit
	//  is even, and the one bit is as far left as is consistent
	//  with that condition.)
	//
	number_type squared_bit = static_cast<number_type>((static_cast<unsigned_type>(~0L) >> 1) & ~(static_cast<unsigned_type>(~0L) >> 2));

	// Form bits of the answer.
	number_type remainder = num;
	number_type root = 0;

	while (squared_bit > 0) {
		if (remainder >= (squared_bit | root)) {
			remainder -= (squared_bit | root);
			root >>= 1;
			root |= squared_bit;
		} else {
			root >>= 1;
		}

		squared_bit >>= 2;
	}

	return root;
}

//integer cube root
template <typename number_type>
inline constexpr number_type cbrt(number_type n)
{
	static_assert(std::is_integral_v<number_type>);

	constexpr number_type bit_size = sizeof(number_type) * 8;

	number_type ret = 0;
	number_type b = 0;

	for (int s = bit_size - (bit_size % 3); s >= 0; s -= 3) {
		ret += ret;
		b = 3 * ret * (ret + 1) + 1;
		if ((n >> s) >= b) {
			n -= b << s;
			++ret;
		}
	}

	return ret;
}

template <typename number_type>
inline constexpr number_type square_distance_between(const number_type x1, const number_type y1, const number_type x2, const number_type y2)
{
	const number_type dx = x1 - x2;
	const number_type dy = y1 - y2;
	return dx * dx + dy * dy;
}

template <typename number_type>
inline constexpr number_type distance_between(const number_type x1, const number_type y1, const number_type x2, const number_type y2)
{
	return number::sqrt(number::square_distance_between(x1, y1, x2, y2));
}

}
