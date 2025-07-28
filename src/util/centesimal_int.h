#pragma once

#include "util/fractional_int.h"

namespace archimedes {

extern template class fractional_int<2>;

using centesimal_int = fractional_int<2>;

}

Q_DECLARE_METATYPE(archimedes::centesimal_int)
Q_DECLARE_METATYPE(std::optional<archimedes::centesimal_int>)
