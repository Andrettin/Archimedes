#pragma once

namespace archimedes {

//a fractional number with fixed decimal precision, implemented via an underlying integer
template <int N>
class fractional_int final
{
public:
	static const int64_t divisor;

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

	constexpr int64_t get_fractional_value() const;
	fractional_int abs() const;

	constexpr int to_int() const
	{
		const int64_t ret = this->to_int64();
		return static_cast<int>(ret);
	}

	constexpr int to_rounded_int() const;
	constexpr int64_t to_int64() const;
	constexpr uint64_t to_uint64() const;
	constexpr double to_double() const;
	constexpr QTime to_time() const;
	std::string to_string(const bool show_as_fraction = false) const;
	std::string to_percent_string() const;
	std::string to_signed_string(const bool show_as_fraction = false) const;

	constexpr fractional_int<N> operator -() const;

	constexpr bool operator ==(const fractional_int<N> &other) const;
	constexpr bool operator ==(const int other) const;
	constexpr bool operator !=(const fractional_int<N> &other) const;
	constexpr bool operator !=(const int other) const;
	constexpr bool operator <(const fractional_int<N> &other) const;
	constexpr bool operator <(const int other) const;
	constexpr bool operator <=(const fractional_int<N> &other) const;
	constexpr bool operator <=(const int other) const;
	constexpr bool operator >(const fractional_int<N> &other) const;
	constexpr bool operator >(const int other) const;
	constexpr bool operator >=(const fractional_int<N> &other) const;
	constexpr bool operator >=(const int other) const;

	constexpr const fractional_int<N> &operator +=(const fractional_int<N> &other);
	constexpr const fractional_int<N> &operator +=(const int other);
	constexpr const fractional_int<N> &operator -=(const fractional_int<N> &other);
	constexpr const fractional_int<N> &operator -=(const int other);

	template <int N2>
	constexpr const fractional_int<N> &operator *=(const fractional_int<N2> &other)
	{
		this->value *= other.get_value();
		this->value /= fractional_int<N2>::divisor;
		return *this;
	}

	constexpr const fractional_int<N> &operator *=(const int other);
	constexpr const fractional_int<N> &operator *=(const int64_t other);
	constexpr const fractional_int<N> &operator *=(const uint64_t other);

	template <int N2>
	constexpr const fractional_int<N> &operator /=(const fractional_int<N2> &other)
	{
		this->value *= fractional_int<N2>::divisor;
		this->value /= other.get_value();
		return *this;
	}

	constexpr const fractional_int<N> &operator /=(const int other);
	constexpr const fractional_int<N> &operator /=(const int64_t other);
	constexpr const fractional_int<N> &operator /=(const uint64_t other);
	constexpr fractional_int<N> operator +(const fractional_int<N> &other) const;
	constexpr fractional_int<N> operator +(const int other) const;
	constexpr fractional_int<N> operator -(const fractional_int<N> &other) const;
	constexpr fractional_int<N> operator -(const int other) const;

	template <int N2>
	constexpr fractional_int<N> operator *(const fractional_int<N2> &other) const
	{
		fractional_int res(*this);
		res *= other;
		return res;
	}

	constexpr fractional_int<N> operator *(const int other) const;
	constexpr fractional_int<N> operator *(const int64_t other) const;
	constexpr fractional_int<N> operator *(const uint64_t other) const;
	constexpr QPoint operator *(const QPoint &rhs) const;
	constexpr QSize operator *(const QSize &rhs) const;

	template <int N2>
	constexpr fractional_int<N> operator /(const fractional_int<N2> &other) const
	{
		fractional_int res(*this);
		res /= other;
		return res;
	}

	constexpr fractional_int<N> operator /(const int other) const;
	constexpr fractional_int<N> operator /(const int64_t other) const;
	constexpr fractional_int<N> operator /(const uint64_t other) const;

	friend constexpr bool operator <(const int lhs, const fractional_int<N> &rhs)
	{
		return rhs > lhs;
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

}
