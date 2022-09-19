#pragma once

inline constexpr const QPoint &operator *=(QPoint &lhs, const QPoint &rhs)
{
	lhs.setX(lhs.x() * rhs.x());
	lhs.setY(lhs.y() * rhs.y());
	return lhs;
}

inline constexpr const QPoint &operator /=(QPoint &lhs, const int rhs)
{
	lhs.setX(lhs.x() / rhs);
	lhs.setY(lhs.y() / rhs);
	return lhs;
}

inline constexpr const QPoint &operator /=(QPoint &lhs, const int64_t rhs)
{
	return lhs /= static_cast<int>(rhs);
}

inline constexpr QPoint operator *(const QPoint &lhs, const QPoint &rhs)
{
	QPoint res(lhs);
	res *= rhs;
	return res;
}

inline constexpr QPoint operator *(const QPoint &lhs, const int64_t rhs)
{
	return lhs * static_cast<int>(rhs);
}

inline constexpr QPoint operator /(const QPoint &lhs, const int rhs)
{
	QPoint res(lhs);
	res /= rhs;
	return res;
}

inline constexpr QPoint operator /(const QPoint &lhs, const int64_t rhs)
{
	return lhs / static_cast<int>(rhs);
}
