#include "archimedes.h"

#include "util/fractional_int.h"

#include "util/number_util.h"

namespace archimedes {

template <int N>
const int64_t fractional_int<N>::divisor = number::pow(10, N);

template <int N>
fractional_int<N> fractional_int<N>::from_value(const int64_t value)
{
	fractional_int n;
	n.value = value;
	return n;
}

template <int N>
std::string fractional_int<N>::to_rest_string(int rest)
{
	std::string rest_str;

	rest = std::abs(rest);
	if (rest > 0) {
		rest_str.reserve(N + 1);
		rest_str += ".";

		int divisor = fractional_int::divisor / 10;
		for (int i = 0; i < N; ++i) {
			if (rest == 0) {
				break;
			}

			rest_str += std::to_string(rest / divisor);
			rest %= divisor;
			divisor /= 10;
		}
	}

	return rest_str;
}

template <int N>
const fractional_int<N> &fractional_int<N>::min(const fractional_int &lhs, const fractional_int &rhs)
{
	if (rhs < lhs) {
		return rhs;
	} else {
		return lhs;
	}
}

template <int N>
const fractional_int<N> &fractional_int<N>::min(const fractional_int &lhs, const int rhs)
{
	return fractional_int::min(lhs, fractional_int(rhs));
}

template <int N>
const fractional_int<N> &fractional_int<N>::max(const fractional_int &lhs, const fractional_int &rhs)
{
	if (rhs > lhs) {
		return rhs;
	} else {
		return lhs;
	}
}

template <int N>
const fractional_int<N> &fractional_int<N>::max(const fractional_int &lhs, const int rhs)
{
	return fractional_int::max(lhs, fractional_int(rhs));
}

template <int N>
constexpr fractional_int<N>::fractional_int(const uint64_t n) : fractional_int(static_cast<int64_t>(n))
{
}

template <int N>
constexpr fractional_int<N>::fractional_int(const double n)
{
	this->value = static_cast<int64_t>(std::round(n * fractional_int::divisor));
}

template <int N>
fractional_int<N>::fractional_int(const std::string &str)
{
	try {
		size_t decimal_point_pos = str.find('.');
		int integer = 0;
		int fraction = 0;
		if (decimal_point_pos != std::string::npos) {
			integer = std::stoi(str.substr(0, decimal_point_pos));
			const size_t decimal_pos = decimal_point_pos + 1;
			const size_t decimal_places = std::min(str.length() - decimal_pos, static_cast<size_t>(N));
			fraction = std::stoi(str.substr(decimal_pos, decimal_places));
			if (decimal_places < N) {
				for (int i = static_cast<int>(decimal_places); i < N; ++i) {
					fraction *= 10;
				}
			}
			const bool negative = str.front() == '-';
			if (negative) {
				fraction *= -1;
			}
		} else {
			integer = std::stoi(str);
		}

		for (int i = 0; i < N; ++i) {
			integer *= 10;
		}
		integer += fraction;

		this->value = integer;
	} catch (...) {
		std::throw_with_nested(std::runtime_error("Failed to convert the fractional number string \"" + str + "\" to an int."));
	}
}

template <int N>
constexpr int64_t fractional_int<N>::get_fractional_value() const
{
	return this->get_value() % fractional_int::divisor;
}

template <int N>
fractional_int<N> fractional_int<N>::abs() const
{
	return fractional_int::from_value(std::abs(this->get_value()));
}

template <int N>
constexpr int fractional_int<N>::to_rounded_int() const
{
	int value = std::abs(this->value);

	int divisor = 10;
	for (int i = 0; i < N; ++i) {
		const int number = value % divisor;
		if (number > 0) {
			if (number >= (5 * number::pow(10, i))) {
				value += divisor;
			}
			value -= number;
		}
		divisor *= 10;
	}

	value /= fractional_int::divisor;

	if (this->value < 0) {
		value *= -1;
	}

	return value;
}

template <int N>
constexpr int64_t fractional_int<N>::to_int64() const
{
	const int64_t ret = this->value / fractional_int::divisor;
	return ret;
}

template <int N>
constexpr uint64_t fractional_int<N>::to_uint64() const
{
	const int64_t ret = this->to_int64();
	return static_cast<uint64_t>(ret);
}

template <int N>
constexpr double fractional_int<N>::to_double() const
{
	return static_cast<double>(this->value) / fractional_int::divisor;
}

template <int N>
constexpr QTime fractional_int<N>::to_time() const
{
	const int hours = this->to_int();
	int64_t rest = this->get_fractional_value();
	const int minutes = rest * 60 / fractional_int::divisor;
	rest -= minutes * fractional_int::divisor / 60;
	const int seconds = rest * 60 * 60 / fractional_int::divisor;
	rest -= seconds * fractional_int::divisor / 60 / 60;

	int milliseconds = 0;
	if constexpr (fractional_int::divisor >= 1000) {
		milliseconds = rest * 60 * 60 / (fractional_int::divisor / 1000);
	}

	return QTime(hours, minutes, seconds, milliseconds);
}

template <int N>
std::string fractional_int<N>::to_string(const bool show_as_fraction) const
{
	const int64_t integer_value = this->to_int64();
	std::string number_str;
	if (integer_value == 0 && this->value < 0) {
		number_str = "-";
	}

	if (show_as_fraction) {
		if (integer_value == 0) {
			return number_str + std::format("1/{}", (centesimal_int(1) / *this).abs().to_string());
		}
	}

	number_str += std::to_string(integer_value);
	number_str += fractional_int::to_rest_string(this->get_fractional_value());
	return number_str;
}

template <int N>
std::string fractional_int<N>::to_percent_string() const
{
	static constexpr int N2 = (N > 2) ? (N - 2) : 0;

	const int percent_value = this->value * 100;
	std::string number_str = std::to_string(percent_value / fractional_int<N2>::divisor);
	number_str += fractional_int<N2>::to_rest_string(percent_value % fractional_int<N2>::divisor);
	return number_str;
}

template <int N>
std::string fractional_int<N>::to_signed_string(const bool show_as_fraction) const
{
	std::string number_str;
	if (this->get_value() >= 0) {
		number_str += "+";
	}
	number_str += this->to_string(show_as_fraction);
	return number_str;
}

template <int N>
constexpr fractional_int<N> fractional_int<N>::operator -() const
{
	return (*this) * -1;
}

template <int N>
constexpr bool fractional_int<N>::operator ==(const fractional_int<N> &other) const
{
	return this->value == other.value;
}

template <int N>
constexpr bool fractional_int<N>::operator ==(const int other) const
{
	return (this->value / fractional_int::divisor) == other && this->get_fractional_value() == 0;
}

template <int N>
constexpr bool fractional_int<N>::operator !=(const fractional_int<N> &other) const
{
	return this->value != other.value;
}

template <int N>
constexpr bool fractional_int<N>::operator !=(const int other) const
{
	return !(*this == other);
}

template <int N>
constexpr bool fractional_int<N>::operator <(const fractional_int<N> &other) const
{
	return this->value < other.value;
}

template <int N>
constexpr bool fractional_int<N>::operator <(const int other) const
{
	const int int_value = this->to_int();
	return int_value < other || (int_value == other && this->get_fractional_value() < 0);
}

template <int N>
constexpr bool fractional_int<N>::operator <=(const fractional_int<N> &other) const
{
	return this->value <= other.value;
}

template <int N>
constexpr bool fractional_int<N>::operator <=(const int other) const
{
	return *this < other || *this == other;
}

template <int N>
constexpr bool fractional_int<N>::operator >(const fractional_int<N> &other) const
{
	return this->value > other.value;
}

template <int N>
constexpr bool fractional_int<N>::operator >(const int other) const
{
	const int int_value = this->to_int();
	return int_value > other || (int_value == other && this->get_fractional_value() > 0);
}

template <int N>
constexpr bool fractional_int<N>::operator >=(const fractional_int<N> &other) const
{
	return this->value >= other.value;
}

template <int N>
constexpr bool fractional_int<N>::operator >=(const int other) const
{
	return *this > other || *this == other;
}

template <int N>
constexpr const fractional_int<N> &fractional_int<N>::operator +=(const fractional_int<N> &other)
{
	this->value += other.value;
	return *this;
}

template <int N>
constexpr const fractional_int<N> &fractional_int<N>::operator +=(const int other)
{
	this->value += other * fractional_int<N>::divisor;
	return *this;
}

template <int N>
constexpr const fractional_int<N> &fractional_int<N>::operator -=(const fractional_int<N> &other)
{
	this->value -= other.value;
	return *this;
}

template <int N>
constexpr const fractional_int<N> &fractional_int<N>::operator -=(const int other)
{
	this->value -= other * fractional_int<N>::divisor;
	return *this;
}

template <int N>
constexpr const fractional_int<N> &fractional_int<N>::operator *=(const int other)
{
	this->value *= other;
	return *this;
}

template <int N>
constexpr const fractional_int<N> &fractional_int<N>::operator *=(const int64_t other)
{
	this->value *= other;
	return *this;
}

template <int N>
constexpr const fractional_int<N> &fractional_int<N>::operator *=(const uint64_t other)
{
	this->value *= static_cast<int64_t>(other);
	return *this;
}

template <int N>
constexpr const fractional_int<N> &fractional_int<N>::operator /=(const int other)
{
	this->value /= other;
	return *this;
}

template <int N>
constexpr const fractional_int<N> &fractional_int<N>::operator /=(const int64_t other)
{
	this->value /= other;
	return *this;
}

template <int N>
constexpr const fractional_int<N> &fractional_int<N>::operator /=(const uint64_t other)
{
	this->value /= static_cast<int64_t>(other);
	return *this;
}

template <int N>
constexpr fractional_int<N> fractional_int<N>::operator +(const fractional_int<N> &other) const
{
	fractional_int res(*this);
	res += other;
	return res;
}

template <int N>
constexpr fractional_int<N> fractional_int<N>::operator +(const int other) const
{
	fractional_int res(*this);
	res += other;
	return res;
}

template <int N>
constexpr fractional_int<N> fractional_int<N>::operator -(const fractional_int<N> &other) const
{
	fractional_int res(*this);
	res -= other;
	return res;
}

template <int N>
constexpr fractional_int<N> fractional_int<N>::operator -(const int other) const
{
	fractional_int res(*this);
	res -= other;
	return res;
}

template <int N>
constexpr fractional_int<N> fractional_int<N>::operator *(const int other) const
{
	fractional_int res(*this);
	res *= other;
	return res;
}

template <int N>
constexpr fractional_int<N> fractional_int<N>::operator *(const int64_t other) const
{
	fractional_int res(*this);
	res *= other;
	return res;
}

template <int N>
constexpr fractional_int<N> fractional_int<N>::operator *(const uint64_t other) const
{
	fractional_int res(*this);
	res *= other;
	return res;
}

template <int N>
constexpr QPoint fractional_int<N>::operator *(const QPoint &rhs) const
{
	return rhs * this->get_value() / fractional_int::divisor;
}

template <int N>
constexpr QSize fractional_int<N>::operator *(const QSize &rhs) const
{
	return rhs * this->get_value() / fractional_int::divisor;
}

template <int N>
constexpr fractional_int<N> fractional_int<N>::operator /(const int other) const
{
	fractional_int res(*this);
	res /= other;
	return res;
}

template <int N>
constexpr fractional_int<N> fractional_int<N>::operator /(const int64_t other) const
{
	fractional_int res(*this);
	res /= other;
	return res;
}

template <int N>
constexpr fractional_int<N> fractional_int<N>::operator /(const uint64_t other) const
{
	fractional_int res(*this);
	res /= other;
	return res;
}

template class fractional_int<1>;
template class fractional_int<2>;
template class fractional_int<3>;
template class fractional_int<4>;

}
