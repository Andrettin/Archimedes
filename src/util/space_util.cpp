#include "archimedes.h"

#include "util/space_util.h"

namespace archimedes::space {

const std::map<std::string, length_unit> length_units_by_short_name = {
	{ "ft", length_unit::feet },
	{ "yd", length_unit::yards }
};

}
