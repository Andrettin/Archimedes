#include "archimedes.h"

#include "language/name_variant.h"

#include "language/word.h"

namespace archimedes {

const std::string &get_name_variant_string(const name_variant &name_variant)
{
	return std::visit([](auto &&name_value) -> const std::string & {
		using name_type = std::decay_t<decltype(name_value)>;

		static_assert(std::is_same_v<name_type, std::string> || std::is_same_v<name_type, const word *>, "Invalid name variant type.");

		if constexpr (std::is_same_v<name_type, std::string>) {
			return name_value;
		} else if constexpr (std::is_same_v<name_type, const word *>) {
			return name_value->get_anglicized_name();
		}
	}, name_variant);
}

}
