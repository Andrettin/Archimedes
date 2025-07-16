#pragma once

#include "util/qunique_ptr.h"
#include "util/type_traits.h"

namespace archimedes {
	class data_module;
	class data_type_metadata;
	class gsml_data;
}

namespace archimedes::database_util {
	extern void set_current_module(const data_module *data_module);
	extern void register_metadata(std::unique_ptr<data_type_metadata> &&metadata);
	extern void register_string_to_qvariant_conversion(const std::string &class_name, std::function<QVariant(const std::string &)> &&function);
	extern void register_list_property_function(const std::string &class_name, std::function<bool(QObject *object, const std::string &, const std::string &)> &&function);

	[[nodiscard]]
	extern QCoro::Task<void> parse_folder(const std::filesystem::path &path, std::vector<gsml_data> &gsml_data_list);
}
