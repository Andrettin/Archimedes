#pragma once

namespace archimedes {

struct point_compare final
{
	bool operator()(const QPoint &point, const QPoint &other_point) const;
};

using point_set = std::set<QPoint, point_compare>;

template <typename T>
using point_map = std::map<QPoint, T, point_compare>;

}
