#pragma once

#include "util/enum_converter.h"

namespace archimedes {

enum class colorization_type {
	none,
	blue,
	gray,
	green,
	red,
	white,
	yellow
};

}

Q_DECLARE_METATYPE(archimedes::colorization_type)
