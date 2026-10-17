#pragma once

#include "script/special_target_type.h"

namespace archimedes {

enum class special_target_type;

template <typename scope_type>
using target_variant = std::variant<std::monostate, scope_type *, special_target_type, std::string>;

template <typename scope_type>
inline target_variant<scope_type> string_to_target_variant(const std::string &str)
{
	static const std::string saved_scope_prefix = "saved_scope:";

	if (magic_enum::enum_contains<special_target_type>(str)) {
		return magic_enum::enum_cast<special_target_type>(str).value();
	} else if (str.starts_with(saved_scope_prefix)) {
		return str.substr(saved_scope_prefix.size(), str.size() - saved_scope_prefix.size());
	} else {
		return std::remove_const_t<scope_type>::get(str);
	}
}

}
