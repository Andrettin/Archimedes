#include "archimedes.h"

#include "util/rect_util.h"

#include "util/number_util.h"
#include "util/point_util.h"

namespace archimedes::rect {

int distance_to(const QRect &rect, const QRect &other_rect)
{
	const QPoint &top_left_pos = rect.topLeft();
	const QPoint &other_top_left_pos = other_rect.topLeft();

	int dx;
	int dy;

	if (top_left_pos.x() + rect.width() <= other_top_left_pos.x()) {
		dx = std::max<int>(0, other_top_left_pos.x() - top_left_pos.x() - rect.width() + 1);
	} else {
		dx = std::max<int>(0, top_left_pos.x() - other_top_left_pos.x() - other_rect.width() + 1);
	}

	if (top_left_pos.y() + rect.height() <= other_top_left_pos.y()) {
		dy = other_top_left_pos.y() - top_left_pos.y() - rect.height() + 1;
	} else {
		dy = std::max<int>(0, top_left_pos.y() - other_top_left_pos.y() - other_rect.height() + 1);
	}

	return number::sqrt(dy * dy + dx * dx);
}

}
