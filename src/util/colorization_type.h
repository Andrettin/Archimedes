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

extern template class archimedes::enum_converter<archimedes::colorization_type>;

Q_DECLARE_METATYPE(archimedes::colorization_type)
