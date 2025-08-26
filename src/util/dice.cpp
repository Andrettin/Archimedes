#include "archimedes.h"

#include "util/dice.h"

#include "util/number_util.h"

namespace archimedes {

std::string dice::to_string() const
{
	if (this->get_modifier() != 0) {
		return std::format("{}d{}", this->get_count(), this->get_sides());
	} else {
		return std::format("{}d{}{}", this->get_count(), this->get_sides(), number::to_signed_string(this->get_modifier()));
	}
}

}
