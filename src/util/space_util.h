#pragma once

namespace archimedes::space {

enum class length_unit
{
	feet,
	yards
};

inline constexpr std::string_view get_length_unit_short_name(const length_unit unit)
{
	switch (unit) {
		case length_unit::feet:
			return "ft";
		case length_unit::yards:
			return "yd";
		default:
			break;
	}

	throw std::runtime_error(std::format("Invalid length_unit: \"{}\".", std::to_underlying(unit)));
}

extern const std::map<std::string, length_unit> length_units_by_short_name;

}
