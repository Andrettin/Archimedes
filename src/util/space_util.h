#pragma once

namespace archimedes::space {

enum class length_unit
{
	inches,
	feet,
	yards,
	centimeters,
	meters
};

inline constexpr std::string_view get_length_unit_short_name(const length_unit unit)
{
	switch (unit) {
		case length_unit::inches:
			return "in";
		case length_unit::feet:
			return "ft";
		case length_unit::yards:
			return "yd";
		case length_unit::centimeters:
			return "cm";
		case length_unit::meters:
			return "m";
		default:
			break;
	}

	throw std::runtime_error(std::format("Invalid length unit: \"{}\".", std::to_underlying(unit)));
}

extern const std::map<std::string, length_unit> length_units_by_short_name;

enum class weight_unit
{
	ounces,
	pounds,
	stones,
	tons,
	grams,
	kilograms
};

inline constexpr std::string_view get_weight_unit_short_name(const weight_unit unit)
{
	switch (unit) {
		case weight_unit::ounces:
			return "oz";
		case weight_unit::pounds:
			return "lb";
		case weight_unit::stones:
			return "st";
		case weight_unit::tons:
			return "tn";
		case weight_unit::grams:
			return "g";
		case weight_unit::kilograms:
			return "kg";
		default:
			break;
	}

	throw std::runtime_error(std::format("Invalid weight unit: \"{}\".", std::to_underlying(unit)));
}

extern const std::map<std::string, weight_unit> weight_units_by_short_name;

}
