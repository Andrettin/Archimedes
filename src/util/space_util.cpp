#include "archimedes.h"

#include "util/space_util.h"

#include <magic_enum/magic_enum_utility.hpp>

namespace archimedes::space {

const std::map<std::string, length_unit> length_units_by_short_name = []() {
	std::map<std::string, length_unit> map;

	magic_enum::enum_for_each<length_unit>([&map](const length_unit unit) {
		map[std::string(get_length_unit_short_name(unit))] = unit;
	});

	return map;
}();

}
