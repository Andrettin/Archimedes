#include "archimedes.h"

#include "util/color_container.h"

namespace std {
	size_t hash<QColor>::operator()(const QColor &color) const
	{
		return std::hash<unsigned>()(color.rgba());
	}
}

namespace archimedes {

bool color_compare::operator()(const QColor &color, const QColor &other_color) const
{
	return color.rgba() < other_color.rgba();
}

bool operator<(const color_set &lhs, const color_set &rhs)
{
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), color_compare());
}

}
