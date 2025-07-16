#pragma once

#include "util/number_util.h"

namespace archimedes {

//a fractional number with fixed decimal precision, implemented via an underlying integer
template <int N>
class fractional_int final
{
public:
	static constexpr int64_t divisor = number::pow(10, N);

	static fractional_int from_value(const int64_t value);
	static std::string to_rest_string(int rest);

	static const fractional_int &min(const fractional_int &lhs, const fractional_int &rhs);
	static const fractional_int &min(const fractional_int &lhs, const int rhs);
	static const fractional_int &max(const fractional_int &lhs, const fractional_int &rhs);
	static const fractional_int &max(const fractional_int &lhs, const int rhs);

	constexpr fractional_int()
	{
	}

	template <int N2>
	explicit constexpr fractional_int(const fractional_int<N2> &other)
	{
		this->value = other.get_value();
		if constexpr (N != N2) {
			this->value *= fractional_int<N>::divisor;
			this->value /= fractional_int<N2>::divisor;
		}
	}

	explicit constexpr fractional_int(const int64_t n)
	{
		this->value = n * fractional_int::divisor;
	}

	explicit constexpr fractional_int(const uint64_t n);

	explicit constexpr fractional_int(const int n) : fractional_int(static_cast<int64_t>(n))
	{
	}

	explicit constexpr fractional_int(const double n);
	explicit fractional_int(const std::string &str);

	constexpr int64_t get_value() const
	{
		return this->value;
	}

	constexpr int64_t get_fractional_value() const
	{
		return this->get_value() % fractional_int::divisor;
	}

	fractional_int abs() const
	{
		return fractional_int::from_value(std::abs(this->get_value()));
	}

	constexpr int to_int() const
	{
		const int64_t ret = this->to_int64();
		return static_cast<int>(ret);
	}

	constexpr int to_rounded_int() const;

	constexpr int64_t to_int64() const
	{
		const int64_t ret = this->value / fractional_int::divisor;
		return ret;
	}

	constexpr uint64_t to_uint64() const
	{
		const int64_t ret = this->to_int64();
		return static_cast<uint64_t>(ret);
	}

	constexpr double to_double() const
	{
		return static_cast<double>(this->value) / fractional_int::divisor;
	}

	constexpr QTime to_time() const;
	std::string to_string(const bool show_as_fraction = false) const;
	std::string to_percent_string() const;
	std::string to_signed_string(const bool show_as_fraction = false) const;

	constexpr fractional_int<N> operator -() const
	{
		return (*this) * -1;
	}

	constexpr bool operator ==(const fractional_int<N> &other) const
	{
		return this->value == other.value;
	}

	constexpr bool operator ==(const int other) const
	{
		return (this->value / fractional_int::divisor) == other && this->get_fractional_value() == 0;
	}

	constexpr bool operator !=(const fractional_int<N> &other) const
	{
		return this->value != other.value;
	}

	constexpr bool operator !=(const int other) const
	{
		return !(*this == other);
	}

	constexpr bool operator <(const fractional_int<N> &other) const
	{
		return this->value < other.value;
	}

	constexpr bool operator <(const int other) const
	{
		const int int_value = this->to_int();
		return int_value < other || (int_value == other && this->get_fractional_value() < 0);
	}

	constexpr bool operator <=(const fractional_int<N> &other) const
	{
		return this->value <= other.value;
	}

	constexpr bool operator <=(const int other) const
	{
		return *this < other || *this == other;
	}

	constexpr bool operator >(const fractional_int<N> &other) const
	{
		return this->value > other.value;
	}

	constexpr bool operator >(const int other) const
	{
		const int int_value = this->to_int();
		return int_value > other || (int_value == other && this->get_fractional_value() > 0);
	}

	constexpr bool operator >=(const fractional_int<N> &other) const
	{
		return this->value >= other.value;
	}

	constexpr bool operator >=(const int other) const
	{
		return *this > other || *this == other;
	}

	constexpr const fractional_int<N> &operator +=(const fractional_int<N> &other)
	{
		this->value += other.value;
		return *this;
	}

	constexpr const fractional_int<N> &operator +=(const int other)
	{
		this->value += other * fractional_int<N>::divisor;
		return *this;
	}

	constexpr const fractional_int<N> &operator -=(const fractional_int<N> &other)
	{
		this->value -= other.value;
		return *this;
	}

	constexpr const fractional_int<N> &operator -=(const int other)
	{
		this->value -= other * fractional_int<N>::divisor;
		return *this;
	}

	template <int N2>
	constexpr const fractional_int<N> &operator *=(const fractional_int<N2> &other)
	{
		this->value *= other.get_value();
		this->value /= fractional_int<N2>::divisor;
		return *this;
	}

	constexpr const fractional_int<N> &operator *=(const int other)
	{
		this->value *= other;
		return *this;
	}

	constexpr const fractional_int<N> &operator *=(const int64_t other)
	{
		this->value *= other;
		return *this;
	}

	constexpr const fractional_int<N> &operator *=(const uint64_t other)
	{
		this->value *= static_cast<int64_t>(other);
		return *this;
	}

	template <int N2>
	constexpr const fractional_int<N> &operator /=(const fractional_int<N2> &other)
	{
		this->value *= fractional_int<N2>::divisor;
		this->value /= other.get_value();
		return *this;
	}

	constexpr const fractional_int<N> &operator /=(const int other)
	{
		this->value /= other;
		return *this;
	}

	constexpr const fractional_int<N> &operator /=(const int64_t other)
	{
		this->value /= other;
		return *this;
	}

	constexpr const fractional_int<N> &operator /=(const uint64_t other)
	{
		this->value /= static_cast<int64_t>(other);
		return *this;
	}

	constexpr fractional_int<N> operator +(const fractional_int<N> &other) const
	{
		fractional_int res(*this);
		res += other;
		return res;
	}

	constexpr fractional_int<N> operator +(const int other) const
	{
		fractional_int res(*this);
		res += other;
		return res;
	}

	constexpr fractional_int<N> operator -(const fractional_int<N> &other) const
	{
		fractional_int res(*this);
		res -= other;
		return res;
	}

	constexpr fractional_int<N> operator -(const int other) const
	{
		fractional_int res(*this);
		res -= other;
		return res;
	}

	template <int N2>
	constexpr fractional_int<N> operator *(const fractional_int<N2> &other) const
	{
		fractional_int res(*this);
		res *= other;
		return res;
	}

	constexpr fractional_int<N> operator *(const int other) const
	{
		fractional_int res(*this);
		res *= other;
		return res;
	}

	constexpr fractional_int<N> operator *(const int64_t other) const
	{
		fractional_int res(*this);
		res *= other;
		return res;
	}

	constexpr fractional_int<N> operator *(const uint64_t other) const
	{
		fractional_int res(*this);
		res *= other;
		return res;
	}

	constexpr QPoint operator *(const QPoint &rhs) const
	{
		return rhs * this->get_value() / fractional_int::divisor;
	}

	constexpr QSize operator *(const QSize &rhs) const
	{
		return rhs * this->get_value() / fractional_int::divisor;
	}

	template <int N2>
	constexpr fractional_int<N> operator /(const fractional_int<N2> &other) const
	{
		fractional_int res(*this);
		res /= other;
		return res;
	}

	constexpr fractional_int<N> operator /(const int other) const
	{
		fractional_int res(*this);
		res /= other;
		return res;
	}

	constexpr fractional_int<N> operator /(const int64_t other) const
	{
		fractional_int res(*this);
		res /= other;
		return res;
	}

	constexpr fractional_int<N> operator /(const uint64_t other) const
	{
		fractional_int res(*this);
		res /= other;
		return res;
	}

	friend constexpr const QSize &operator *=(QSize &lhs, const fractional_int<N> &rhs)
	{
		lhs = rhs * lhs;
		return lhs;
	}

	friend constexpr const QSize &operator /=(QSize &lhs, const fractional_int<N> &rhs)
	{
		lhs *= fractional_int<N>::divisor;
		lhs /= rhs.get_value();
		return lhs;
	}

	friend constexpr fractional_int<N> operator -(const int lhs, const fractional_int<N> &rhs)
	{
		fractional_int res(lhs);
		res -= rhs;
		return res;
	}

	friend constexpr fractional_int<N> operator *(const int lhs, const fractional_int<N> &rhs)
	{
		fractional_int res(rhs);
		res *= lhs;
		return res;
	}

	friend constexpr QPoint operator *(const QPoint &lhs, const fractional_int<N> &rhs)
	{
		return rhs * lhs;
	}

	friend constexpr QSize operator *(const QSize &lhs, const fractional_int<N> &rhs)
	{
		return rhs * lhs;
	}

	friend constexpr fractional_int<N> operator /(const int lhs, const fractional_int<N> &rhs)
	{
		fractional_int res(lhs);
		res /= rhs;
		return res;
	}

	friend constexpr QPoint operator /(const QPoint &lhs, const fractional_int<N> &rhs)
	{
		return lhs * fractional_int<N>::divisor / rhs.get_value();
	}

	friend constexpr QSize operator /(const QSize &lhs, const fractional_int<N> &rhs)
	{
		return lhs * fractional_int<N>::divisor / rhs.get_value();
	}

private:
	int64_t value = 0;
};

extern template class fractional_int<1>;
extern template class fractional_int<2>;
extern template class fractional_int<3>;
extern template class fractional_int<4>;

using decimal_int = fractional_int<1>;
using centesimal_int = fractional_int<2>;
using millesimal_int = fractional_int<3>;
using decimillesimal_int = fractional_int<4>;

}

Q_DECLARE_METATYPE(archimedes::decimal_int)
Q_DECLARE_METATYPE(archimedes::centesimal_int)
Q_DECLARE_METATYPE(archimedes::decimillesimal_int)
Q_DECLARE_METATYPE(std::optional<archimedes::centesimal_int>)
