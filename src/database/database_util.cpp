#include "archimedes.h"

#include "database/database_util.h"

#include "database/data_type.h"
#include "database/database.h"

namespace archimedes::database_util {

void set_current_module(const data_module *data_module)
{
	database::get()->set_current_module(data_module);
}

void register_data_type(std::unique_ptr<data_type_metadata> &&metadata, const std::string &property_class_identifier, const std::function<bool(QObject *object, const std::string &, const std::string &)> &list_property_function, const std::function<bool(QObject *object, const std::string &, const std::string &)> &const_list_property_function)
{
	database_util::register_metadata(std::move(metadata));

	database_util::register_list_property_function(std::format("std::vector<{}>", property_class_identifier), list_property_function);
	database_util::register_list_property_function(std::format("std::vector<{},std::allocator<{}>>", property_class_identifier, property_class_identifier), list_property_function);

	database_util::register_list_property_function(std::format("std::vector<const {}>", property_class_identifier), const_list_property_function);
	database_util::register_list_property_function(std::format("std::vector<const {},std::allocator<const {}>>", property_class_identifier, property_class_identifier), const_list_property_function);
}

void register_metadata(std::unique_ptr<data_type_metadata> &&metadata)
{
	database::get()->register_metadata(std::move(metadata));
}

void register_string_to_qvariant_conversion(const std::string &class_name, std::function<QVariant(const std::string &)> &&function)
{
	database::get()->register_string_to_qvariant_conversion(class_name, std::move(function));
}

void register_list_property_function(const std::string &class_name, const std::function<bool(QObject *object, const std::string &, const std::string &)> &function)
{
	database::get()->register_list_property_function(class_name, function);
}

QCoro::Task<void> parse_folder(const std::filesystem::path &path, std::vector<gsml_data> &gsml_data_list)
{
	co_await database::parse_folder(path, gsml_data_list);
}

}
