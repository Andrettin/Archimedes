#pragma once

#include "database/data_module_container.h"
#include "database/data_type_metadata.h"
#include "database/database_util.h"
#include "database/gsml_data.h"
#include "database/gsml_operator.h"
#include "util/aggregate_exception.h"
#include "util/qunique_ptr.h"

namespace archimedes {

class data_entry;
class data_module;
class game_rules_base;

class data_type_base
{
public:
	static constexpr bool history_enabled = false;

	static inline const std::set<std::string> database_dependencies; //the other classes on which this one depends, i.e. after which this class' database can be processed
	static inline const std::set<std::string> history_database_dependencies;

protected:
	static data_entry *try_get(const QMetaType &meta_type, const std::string &identifier);

	static const std::vector<data_entry *> &get_all(const QMetaType &meta_type);
	static std::vector<data_entry *> &get_all_modifiable(const QMetaType &meta_type);

	static bool exists(const QMetaType &meta_type, const std::string &identifier);

	static data_entry *add(const QMetaType &meta_type, const std::string &identifier, const data_module *data_module, const std::string_view &class_identifier, qunique_ptr<data_entry> &&instance);

	static void add_instance_alias(const QMetaType &meta_type, data_entry *instance, const std::string &alias, const std::string_view &class_identifier);

	static void remove(const QMetaType &meta_type, data_entry *instance);
	static void clear(const QMetaType &meta_type);

private:
	static std::map<int, std::vector<data_entry *>> instances;
	static std::map<int, std::map<std::string, qunique_ptr<data_entry>>> instances_by_identifier;
	static std::map<int, std::map<std::string, data_entry *>> instances_by_alias;
};

template <typename T>
class data_type : public data_type_base
{
public:
	data_type()
	{
		//this check is required for class_initialized variable and, correspondingly,
		//the data_type::initialize_class() call to not to be initialized away
		if (!data_type::class_initialized) {
			throw std::runtime_error("Never reached.");
		}
	}

	static T *get(const std::string &identifier)
	{
		if (identifier == "none") {
			return nullptr;
		}

		T *instance = T::try_get(identifier);

		if (instance == nullptr) {
			throw std::runtime_error("Invalid " + std::string(T::class_identifier) + " instance: \"" + identifier + "\".");
		}

		return instance;
	}

	static T *try_get(const std::string &identifier)
	{
		return static_cast<T *>(data_type_base::try_get(QMetaType::fromType<T>(), identifier));
	}

	static T *get_or_add(const std::string &identifier, const data_module *data_module)
	{
		T *instance = T::try_get(identifier);
		if (instance != nullptr) {
			return instance;
		}

		return T::add(identifier, data_module);
	}

	static const std::vector<T *> &get_all()
	{
		return reinterpret_cast<const std::vector<T *> &>(data_type_base::get_all(QMetaType::fromType<T>()));
	}

	static std::vector<const T *> get_all_const()
	{
		std::vector<const T *> const_instances;
		for (const T *instance : T::get_all()) {
			const_instances.push_back(instance);
		}
		return const_instances;
	}

	static bool exists(const std::string &identifier)
	{
		return data_type_base::exists(QMetaType::fromType<T>(), identifier);
	}

	static T *add(const std::string &identifier, const data_module *data_module)
	{
		auto instance = make_qunique<T>(identifier);
		return static_cast<T *>(data_type_base::add(QMetaType::fromType<T>(), identifier, data_module, T::class_identifier, std::move(instance)));
	}

	static void remove(data_entry *instance)
	{
		data_type_base::remove(QMetaType::fromType<T>(), instance);
	}

	static void remove(const std::string &identifier)
	{
		T::remove(T::get(identifier));
	}

	static void clear()
	{
		data_type_base::clear(QMetaType::fromType<T>());
	}

	template <typename function_type>
	static void sort_instances(const function_type &function)
	{
		std::vector<T *> &instances = reinterpret_cast<std::vector<T *> &>(data_type_base::get_all_modifiable(QMetaType::fromType<T>()));
		std::sort(instances.begin(), instances.end(), function);
	}

	[[nodiscard]]
	static QCoro::Task<std::vector<gsml_data>> parse_database(const std::filesystem::path &data_path)
	{
		std::vector<gsml_data> gsml_data_to_process;

		if (std::string(T::database_folder).empty()) {
			co_return gsml_data_to_process;
		}

		const std::filesystem::path database_path(data_path / T::database_folder);

		if (!std::filesystem::exists(database_path)) {
			co_return gsml_data_to_process;
		}

		co_await database_util::parse_folder(database_path, gsml_data_to_process);
		co_return gsml_data_to_process;
	}

	static void process_database(const bool definition, const data_module_map<std::vector<gsml_data>> &gsml_data_to_process)
	{
		if (std::string(T::database_folder).empty()) {
			return;
		}

		std::vector<std::exception_ptr> exceptions;

		for (const auto &kv_pair : gsml_data_to_process) {
			const data_module *data_module = kv_pair.first;

			database_util::set_current_module(data_module);

			const std::vector<gsml_data> &gsml_data_list = kv_pair.second;
			for (const gsml_data &data : gsml_data_list) {
				data.for_each_child([&](const gsml_data &data_entry) {
					try {
						const std::string &identifier = data_entry.get_tag();

						T *instance = nullptr;
						if (definition) {
							if (data_entry.get_operator() != gsml_operator::addition) {
								//addition operators for data entry scopes mean modifying already-defined entries
								instance = T::add(identifier, data_module);
							} else {
								instance = T::get(identifier);
							}

							for (const gsml_property *alias_property : data_entry.try_get_properties("aliases")) {
								if (alias_property->get_operator() != gsml_operator::addition) {
									throw std::runtime_error("Only the addition operator is supported for data entry aliases.");
								}

								const std::string &alias = alias_property->get_value();
								T::add_instance_alias(QMetaType::fromType<T>(), instance, alias, T::class_identifier);

								//for backwards compatibility, change instances of "_" in the identifier with "-" and add that as a further alias, and do the opposite as well
								if (alias.find("_") != std::string::npos) {
									std::string other_alias = alias;
									std::replace(other_alias.begin(), other_alias.end(), '_', '-');
									T::add_instance_alias(QMetaType::fromType<T>(), instance, other_alias, T::class_identifier);
								} else if (alias.find("-") != std::string::npos) {
									std::string other_alias = alias;
									std::replace(other_alias.begin(), other_alias.end(), '-', '_');
									T::add_instance_alias(QMetaType::fromType<T>(), instance, other_alias, T::class_identifier);
								}
							}
						} else {
							try {
								instance = T::get(identifier);
								instance->process_gsml_data(data_entry);
								instance->set_defined(true);
							} catch (...) {
								std::throw_with_nested(std::runtime_error("Error processing or loading data for " + std::string(T::class_identifier) + " instance \"" + identifier + "\"."));
							}
						}
					} catch (...) {
						exceptions.push_back(std::current_exception());
					}
				});
			}
		}

		if (!exceptions.empty()) {
			throw aggregate_exception(std::format("The database processing for {} instances failed.", T::class_identifier), std::move(exceptions));
		}

		database_util::set_current_module(nullptr);
	}

	static void load_history_database(const QDate &start_date, const timeline *timeline, const game_rules_base *game_rules)
	{
		if constexpr (T::history_enabled) {
			try {
				for (T *instance : T::get_all()) {
					instance->reset_history();
				}

				for (T *instance : T::get_all()) {
					try {
						instance->load_history(start_date, timeline, game_rules);
					} catch (...) {
						std::throw_with_nested(std::runtime_error("Error loading history for the " + std::string(T::class_identifier) + " instance \"" + instance->get_identifier() + "\"."));
					}
				}
			} catch (...) {
				std::throw_with_nested(std::runtime_error("Error loading history for the " + std::string(T::class_identifier) + " class."));
			}
		}
	}

	static void initialize_all()
	{
		for (T *instance : T::get_all()) {
			if (instance->is_initialized()) {
				continue; //the instance might have been initialized already, e.g. in the initialization function of another instance which needs it to be initialized
			}

			try {
				instance->initialize();
			} catch (...) {
				std::throw_with_nested(std::runtime_error("Failed to initialize the " + std::string(T::class_identifier) + " instance \"" + instance->get_identifier() + "\"."));
			}

			if (!instance->is_initialized()) {
				throw std::runtime_error("The " + std::string(T::class_identifier) + " instance \"" + instance->get_identifier() + "\" is not marked as initialized despite the initialization function having been called for it.");
			}
		}
	}

	static void process_all_text()
	{
		for (T *instance : T::get_all()) {
			try {
				instance->process_text();
			} catch (...) {
				std::throw_with_nested(std::runtime_error("Failed to process text for the " + std::string(T::class_identifier) + " instance \"" + instance->get_identifier() + "\"."));
			}
		}
	}

	static void check_all()
	{
		std::vector<std::exception_ptr> exceptions;

		for (const T *instance : T::get_all()) {
			try {
				try {
					instance->check();
				} catch (...) {
					std::throw_with_nested(std::runtime_error(std::format("The validity check for the {} instance \"{}\" failed.", T::class_identifier, instance->get_identifier())));
				}
			} catch (...) {
				exceptions.push_back(std::current_exception());
			}
		}

		if (!exceptions.empty()) {
			throw aggregate_exception(std::format("The validity check for {} instances failed.", T::class_identifier), std::move(exceptions));
		}
	}

	static std::vector<T *> get_encyclopedia_entries()
	{
		std::vector<T *> entries;

		for (T *instance : T::get_all()) {
			if (instance->has_encyclopedia_entry()) {
				entries.push_back(instance);
			}
		}

		T::sort_encyclopedia_entries(entries);

		return entries;
	}

	static void sort_encyclopedia_entries(std::vector<T *> &entries)
	{
		std::sort(entries.begin(), entries.end(), T::compare_encyclopedia_entries);
	}

private:
	static inline bool initialize_class()
	{
		//initialize the metadata (including database parsing/processing functions) for this data type
		auto metadata = std::make_unique<data_type_metadata>(T::class_identifier, T::database_dependencies, T::history_database_dependencies, T::parse_database, T::process_database, T::initialize_all, T::process_all_text, T::check_all, T::clear, T::load_history_database);
		database_util::register_metadata(std::move(metadata));

		database_util::register_string_to_qvariant_conversion(T::property_class_identifier, [](const std::string &value) {
			return QVariant::fromValue(T::get(value));
		});

		database_util::register_string_to_qvariant_conversion(std::format("const {}", T::property_class_identifier), [](const std::string &value) {
			return QVariant::fromValue(T::get(value));
		});

		const auto list_property_function = [](QObject *object, const std::string &method_name, const std::string &value_str) {
			T *value = T::get(value_str);
			return QMetaObject::invokeMethod(object, method_name.c_str(), Qt::ConnectionType::DirectConnection, QArgument<T *>((std::string(T::class_identifier) + " *").c_str(), value));
		};

		database_util::register_list_property_function(std::format("std::vector<{}>", T::property_class_identifier), list_property_function);
		database_util::register_list_property_function(std::format("std::vector<{},std::allocator<{}>>", T::property_class_identifier, T::property_class_identifier), list_property_function);

		const auto const_list_property_function = [](QObject *object, const std::string &method_name, const std::string &value_str) {
			const T *value = T::get(value_str);
			return QMetaObject::invokeMethod(object, method_name.c_str(), Qt::ConnectionType::DirectConnection, QArgument<const T *>(("const " + std::string(T::class_identifier) + " *").c_str(), value));
		};

		database_util::register_list_property_function(std::format("std::vector<const {}>", T::property_class_identifier), const_list_property_function);
		database_util::register_list_property_function(std::format("std::vector<const {},std::allocator<const {}>>", T::property_class_identifier, T::property_class_identifier), const_list_property_function);

		return true;
	}

	static inline bool class_initialized = data_type::initialize_class();

public:
	virtual std::string get_link_string(const std::string &link_text = "", const bool highlight_as_fallback = false) const
	{
		const T *underlying = this->to_underlying();

		std::string link_name;
		if (!link_text.empty()) {
			link_name = link_text;
		} else {
			link_name = underlying->get_link_name();
		}

		if (!underlying->has_encyclopedia_entry()) {
			//don't write a link if the entry cannot have an encyclopedia entry
			if (highlight_as_fallback) {
				return "~<" + link_name + "~>";
			} else {
				return link_name;
			}
		}

		std::string link = "<a href='";
		link += T::class_identifier;
		link += ":";
		link += underlying->get_identifier();
		link += "'>";
		link += link_name;
		link += "</a>";
		return link;
	}

private:
	const T *to_underlying() const
	{
		return static_cast<const T *>(this);
	}
};

}
