#include "archimedes.h"

#include "util/point_util.h"

#include "util/number_util.h"
#include "util/rect_util.h"

namespace archimedes::point {

int square_distance_to(const QPoint &point, const QPoint &other_point)
{
	return number::square_distance_between(point.x(), point.y(), other_point.x(), other_point.y());
}

int distance_to(const QPoint &point, const QPoint &other_point)
{
	return number::distance_between(point.x(), point.y(), other_point.x(), other_point.y());
}

int distance_to(const QPoint &point, const QRect &rect)
{
	const QRect point_rect(point, QSize(1, 1));

	return rect::distance_to(point_rect, rect);
}

}
