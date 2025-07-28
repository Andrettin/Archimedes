#pragma once

#include "util/fractional_int.h"

namespace archimedes {

extern template class fractional_int<4>;

using decimillesimal_int = fractional_int<4>;

}

Q_DECLARE_METATYPE(archimedes::decimillesimal_int)
