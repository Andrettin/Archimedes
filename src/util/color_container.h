#pragma once

namespace std {
	template <>
	struct hash<QColor>
	{
		size_t operator()(const QColor &color) const;
	};
}

namespace archimedes {

struct color_compare final
{
	bool operator()(const QColor &color, const QColor &other_color) const;
};

using color_set = std::set<QColor, color_compare>;

extern bool operator<(const color_set &lhs, const color_set &rhs);

template <typename T>
using color_map = std::map<QColor, T, color_compare>;

template <typename T>
using color_unordered_map = std::unordered_map<QColor, T>;

}
