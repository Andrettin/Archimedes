#pragma once

namespace archimedes {

struct point_compare final
{
	bool operator()(const QPoint &point, const QPoint &other_point) const;
};

struct point_hash final
{
	size_t operator()(const QPoint &point) const
	{
		return qHash(point);
	}
};

using point_set = std::set<QPoint, point_compare>;

template <typename T>
using point_map = std::map<QPoint, T, point_compare>;

template <typename T>
using point_unordered_set = std::unordered_set<QPoint, point_hash>;

template <typename T>
using point_unordered_map = std::unordered_map<QPoint, T, point_hash>;

}
