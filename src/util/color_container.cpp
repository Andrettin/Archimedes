#include "archimedes.h"

#include "util/color_container.h"

namespace archimedes {

bool color_compare::operator()(const QColor &color, const QColor &other_color) const
{
	return color.rgba() < other_color.rgba();
}

}
