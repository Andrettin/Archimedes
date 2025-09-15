#include "archimedes.h"

#include "util/dice.h"

#include "util/number_util.h"

namespace archimedes {

std::string dice::to_string() const
{
	if (this->get_modifier() != 0) {
		return std::format("{}d{}{}", this->get_count(), this->get_sides(), number::to_signed_string(this->get_modifier()));
	} else {
		return std::format("{}d{}", this->get_count(), this->get_sides());
	}
}

std::string dice::to_display_string() const
{
	const int min_result = this->get_minimum_result();
	const int max_result = this->get_maximum_result();

	return std::format("{}-{}", min_result, max_result);
}

}
