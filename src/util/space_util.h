#pragma once

namespace archimedes::space {

enum class length_unit
{
	feet,
	yards
};

extern const std::map<std::string, length_unit> length_units_by_short_name;

}
