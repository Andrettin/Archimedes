#pragma once

inline constexpr const QSize &operator *=(QSize &lhs, const QSize &rhs)
{
	lhs.setWidth(lhs.width() * rhs.width());
	lhs.setHeight(lhs.height() * rhs.height());
	return lhs;
}

inline constexpr QSize operator *(const QSize &lhs, const QSize &rhs)
{
	QSize res(lhs);
	res *= rhs;
	return res;
}

inline constexpr const QSize &operator /=(QSize &lhs, const QSize &rhs)
{
	lhs.setWidth(lhs.width() / rhs.width());
	lhs.setHeight(lhs.height() / rhs.height());
	return lhs;
}

inline constexpr const QSize &operator /=(QSize &lhs, const int rhs)
{
	lhs.setWidth(lhs.width() / rhs);
	lhs.setHeight(lhs.height() / rhs);
	return lhs;
}

inline constexpr const QSize &operator /=(QSize &lhs, const int64_t rhs)
{
	return lhs /= static_cast<int>(rhs);
}

inline constexpr QSize operator /(const QSize &lhs, const QSize &rhs)
{
	QSize res(lhs);
	res /= rhs;
	return res;
}

inline constexpr QSize operator /(const QSize &lhs, const int rhs)
{
	QSize res(lhs);
	res /= rhs;
	return res;
}

inline constexpr QSize operator /(const QSize &lhs, const int64_t rhs)
{
	return lhs / static_cast<int>(rhs);
}
