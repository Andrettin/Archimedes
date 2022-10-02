#pragma once

namespace archimedes {

class data_module;

struct data_module_compare
{
	bool operator()(const data_module *data_module, const archimedes::data_module *other_data_module) const;
};

using data_module_set = std::set<const data_module *, data_module_compare>;

template <typename T>
using data_module_map = std::map<const data_module *, T, data_module_compare>;

}
