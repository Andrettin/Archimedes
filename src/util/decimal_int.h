#pragma once

#include "util/fractional_int.h"

namespace archimedes {

extern template class fractional_int<1>;

using decimal_int = fractional_int<1>;

}

Q_DECLARE_METATYPE(archimedes::decimal_int)
