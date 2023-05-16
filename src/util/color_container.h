#pragma once

namespace std {
	template <> struct hash<QColor>
	{
		size_t operator()(const QColor &color) const
		{
			return std::hash<unsigned>()(color.rgba());
		}
	};
}

namespace archimedes {

struct color_compare final
{
	bool operator()(const QColor &color, const QColor &other_color) const
	{
		return color.rgba() < other_color.rgba();
	}
};

using color_set = std::set<QColor, color_compare>;

inline bool operator<(const color_set &lhs, const color_set &rhs)
{
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), color_compare());
}

template <typename T>
using color_map = std::map<QColor, T, color_compare>;

}
