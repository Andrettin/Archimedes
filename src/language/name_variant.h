#pragma once

namespace archimedes {

class word;

using name_variant = std::variant<std::string, const word *>;

extern std::string get_name_variant_string(const name_variant &name_variant);

}
