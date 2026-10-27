#include "archimedes.h"

#include "util/dice.h"

#include "util/decimillesimal_int.h"
#include "util/number_util.h"

namespace archimedes {

decimillesimal_int dice::get_average(const decimillesimal_int &multiplier) const
{
	if (this->get_min_value().has_value() && this->get_count() == 1 && this->get_sides() > 0) {
		decimillesimal_int average;
		for (int i = 1; i <= this->get_sides(); ++i) {
			average += decimillesimal_int::max((i + this->get_modifier()) * multiplier, this->get_min_value().value());
		}
		average /= this->get_sides();
		return average;
	}

	return ((this->get_count() * this->get_sides()) + this->get_count()) * multiplier / 2 + (this->get_modifier() * multiplier);
}

int dice::get_average(const int multiplier) const
{
	return this->get_average(decimillesimal_int(multiplier)).to_int();
}

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
