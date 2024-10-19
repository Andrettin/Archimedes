#pragma once

#include "util/enum_converter.h"

namespace archimedes {

enum class special_target_type {
	root,
	source,
	previous
};

extern template class enum_converter<special_target_type>;

}

Q_DECLARE_METATYPE(archimedes::special_target_type)
