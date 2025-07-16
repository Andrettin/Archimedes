#pragma once

#include "database/gsml_data.h"
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


	template <typename T>
	void process_gsml_data(T *instance, const gsml_data &data)
	{
		data.for_each_element([&](const gsml_property &property) {
			instance->process_gsml_property(property);
		}, [&](const gsml_data &scope) {
			instance->process_gsml_scope(scope);
		});
	}

	template <typename T>
	void process_gsml_data(T &instance, const gsml_data &data)
	{
		if constexpr (is_specialization_of_v<T, std::unique_ptr>) {
			database_util::process_gsml_data(instance.get(), data);
		} else {
			database_util::process_gsml_data(&instance, data);
		}
	}

	template <typename T>
	void process_gsml_data(const std::unique_ptr<T> &instance, const gsml_data &data)
	{
		database_util::process_gsml_data(instance.get(), data);
	}

	template <typename T>
	void process_gsml_data(const qunique_ptr<T> &instance, const gsml_data &data)
	{
		database_util::process_gsml_data(instance.get(), data);
	}
}
