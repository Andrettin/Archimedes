#pragma once

namespace archimedes::enumeration {

template <typename enum_type>
inline std::underlying_type_t<enum_type> to_underlying(const enum_type value)
{
	return static_cast<std::underlying_type_t<enum_type>>(value);
}

}
