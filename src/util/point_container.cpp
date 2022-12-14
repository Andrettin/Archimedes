#include "util/point_container.h"

namespace archimedes {

bool point_compare::operator()(const QPoint &point, const QPoint &other_point) const
{
	if (point.x() != other_point.x()) {
		return point.x() < other_point.x();
	}

	return point.y() < other_point.y();
}

}
