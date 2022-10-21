#include "archimedes.h"

#include "map/direction.h"

#include "util/point_util.h"

namespace archimedes {

direction offset_to_direction(const QPoint &offset)
{
	if (offset.x() < 0) {
		if (offset.y() < 0) {
			return direction::northwest;
		} else if (offset.y() == 0) {
			return direction::west;
		} else if (offset.y() > 0) {
			return direction::southwest;
		}
	} else if (offset.x() == 0) {
		if (offset.y() < 0) {
			return direction::north;
		} else if (offset.y() > 0) {
			return direction::south;
		}
	} else if (offset.x() > 0) {
		if (offset.y() < 0) {
			return direction::northeast;
		} else if (offset.y() == 0) {
			return direction::east;
		} else if (offset.y() > 0) {
			return direction::southeast;
		}
	}

	throw std::runtime_error("No direction for offset " + point::to_string(offset) + ".");
}

}
