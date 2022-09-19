#pragma once

namespace archimedes {

struct color_compare
{
	bool operator()(const QColor &color, const QColor &other_color) const;
};

using color_set = std::set<QColor, color_compare>;

inline bool operator<(const color_set &lhs, const color_set &rhs)
{
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), color_compare());
}

template <typename T>
using color_map = std::map<QColor, T, color_compare>;

}
