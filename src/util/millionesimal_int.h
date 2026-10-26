#pragma once

#include "util/fractional_int.h"

namespace archimedes {

extern template class fractional_int<6>;

using millionesimal_int = fractional_int<6>;

}

Q_DECLARE_METATYPE(archimedes::millionesimal_int)
