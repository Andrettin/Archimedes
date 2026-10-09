#include "archimedes.h"

#include "database/data_type.h"

#include "database/data_entry.h"
#include "database/database.h"
#include "database/gsml_data.h"
#include "database/gsml_operator.h"

namespace archimedes {

std::map<int, std::vector<data_entry *>> data_type_base::instances;
std::map<int, std::map<std::string, qunique_ptr<data_entry>>> data_type_base::instances_by_identifier;
std::map<int, std::map<std::string, data_entry *>> data_type_base::instances_by_alias;

data_entry *data_type_base::get(const std::string &identifier, const data_type_metadata *metadata)
{
	if (identifier == "none") {
		return nullptr;
	}

	data_entry *instance = data_type_base::try_get(identifier, metadata->get_meta_type());

	if (instance == nullptr) {
		throw std::runtime_error("Invalid " + metadata->get_class_identifier() + " instance: \"" + identifier + "\".");
	}

	return instance;
}

data_entry *data_type_base::get(const std::string &identifier, const std::string &class_identifier)
{
	return data_type_base::get(identifier, database::get()->get_metadata(class_identifier));
}

data_entry *data_type_base::try_get(const std::string &identifier, const QMetaType &meta_type)
{
	if (identifier == "none") {
		return nullptr;
	}

	const auto find_iterator = data_type_base::instances_by_identifier[meta_type.id()].find(identifier);
	if (find_iterator != data_type_base::instances_by_identifier[meta_type.id()].end()) {
		return find_iterator->second.get();
	}

	const auto alias_find_iterator = data_type_base::instances_by_alias[meta_type.id()].find(identifier);
	if (alias_find_iterator != data_type_base::instances_by_alias[meta_type.id()].end()) {
		return alias_find_iterator->second;
	}

	return nullptr;
}

const std::vector<data_entry *> &data_type_base::get_all(const QMetaType &meta_type)
{
	return data_type_base::instances[meta_type.id()];
}

std::vector<data_entry *> &data_type_base::get_all_modifiable(const QMetaType &meta_type)
{
	return data_type_base::instances[meta_type.id()];
}

bool data_type_base::exists(const QMetaType &meta_type, const std::string &identifier)
{
	return data_type_base::instances_by_identifier[meta_type.id()].contains(identifier) || data_type_base::instances_by_alias[meta_type.id()].contains(identifier);
}

data_entry *data_type_base::add(const std::string &identifier, const data_module *data_module, const data_type_metadata *metadata)
{
	if (identifier.empty()) {
		throw std::runtime_error("Tried to add a " + metadata->get_class_identifier() + " instance with an empty string identifier.");
	}

	if (data_type_base::exists(metadata->get_meta_type(), identifier)) {
		throw std::runtime_error("Tried to add a " + metadata->get_class_identifier() + " instance with the already-used \"" + identifier + "\" string identifier.");
	}

	auto instance = metadata->get_instance_creation_function()(identifier);
	data_entry *instance_ptr = instance.get();
	data_type_base::instances_by_identifier[metadata->get_meta_type().id()][identifier] = std::move(instance);

	data_type_base::instances[metadata->get_meta_type().id()].push_back(instance_ptr);
	instance_ptr->moveToThread(QApplication::instance()->thread());
	instance_ptr->set_module(data_module);

	//for backwards compatibility, change instances of "_" in the identifier with "-" and add that as an alias, and do the opposite as well
	if (identifier.find("_") != std::string::npos) {
		std::string alias = identifier;
		std::replace(alias.begin(), alias.end(), '_', '-');
		data_type_base::add_instance_alias(metadata->get_meta_type(), instance_ptr, alias, metadata->get_class_identifier());
	}

	if (identifier.find("-") != std::string::npos) {
		std::string alias = identifier;
		std::replace(alias.begin(), alias.end(), '-', '_');
		data_type_base::add_instance_alias(metadata->get_meta_type(), instance_ptr, alias, metadata->get_class_identifier());
	}

	return instance_ptr;
}

data_entry *data_type_base::add(const std::string &identifier, const data_module *data_module, const std::string &class_identifier)
{
	return data_type_base::add(identifier, data_module, database::get()->get_metadata(class_identifier));
}

void data_type_base::add_instance_alias(const QMetaType &meta_type, data_entry *instance, const std::string &alias, const std::string_view &class_identifier)
{
	if (alias.empty()) {
		throw std::runtime_error("Tried to add a " + std::string(class_identifier) + " instance empty alias.");
	}

	if (data_type_base::exists(meta_type, alias)) {
		throw std::runtime_error("Tried to add a " + std::string(class_identifier) + " alias with the already-used \"" + alias + "\" string identifier.");
	}

	data_type_base::instances_by_alias[meta_type.id()][alias] = instance;
	instance->add_alias(alias);
}

void data_type_base::remove(const QMetaType &meta_type, data_entry *instance)
{
	data_type_base::instances[meta_type.id()].erase(std::remove(data_type_base::instances[meta_type.id()].begin(), data_type_base::instances[meta_type.id()].end(), instance), data_type_base::instances[meta_type.id()].end());

	data_type_base::instances_by_identifier[meta_type.id()].erase(instance->get_identifier());
}

void data_type_base::clear(const QMetaType &meta_type)
{
	data_type_base::instances[meta_type.id()].clear();
	data_type_base::instances_by_alias[meta_type.id()].clear();
	data_type_base::instances_by_identifier[meta_type.id()].clear();
}

QCoro::Task<std::vector<gsml_data>> data_type_base::parse_database(const std::filesystem::path &data_path, const data_type_metadata *metadata)
{
	std::vector<gsml_data> gsml_data_to_process;

	if (metadata->get_database_folder().empty()) {
		co_return gsml_data_to_process;
	}

	const std::filesystem::path database_path(data_path / metadata->get_database_folder());

	if (!std::filesystem::exists(database_path)) {
		co_return gsml_data_to_process;
	}

	co_await database_util::parse_folder(database_path, gsml_data_to_process);
	co_return gsml_data_to_process;
}

void data_type_base::process_database(const bool definition, const data_module_map<std::vector<gsml_data>> &gsml_data_to_process, const data_type_metadata *metadata)
{
	if (metadata->get_database_folder().empty()) {
		return;
	}

	std::vector<std::exception_ptr> exceptions;

	for (const auto &kv_pair : gsml_data_to_process) {
		const data_module *data_module = kv_pair.first;

		database_util::set_current_module(data_module);

		const std::vector<gsml_data> &gsml_data_list = kv_pair.second;
		for (const gsml_data &data : gsml_data_list) {
			data.for_each_child([definition, data_module, &exceptions, metadata](const gsml_data &data_entry) {
				try {
					const std::string &identifier = data_entry.get_tag();

					archimedes::data_entry *instance = nullptr;
					if (definition) {
						if (data_entry.get_operator() != gsml_operator::addition) {
							//addition operators for data entry scopes mean modifying already-defined entries
							instance = data_type_base::add(identifier, data_module, metadata);
						} else {
							instance = data_type_base::get(identifier, metadata);
						}

						for (const gsml_property *alias_property : data_entry.try_get_properties("aliases")) {
							if (alias_property->get_operator() != gsml_operator::addition) {
								throw std::runtime_error("Only the addition operator is supported for data entry aliases.");
							}

							const std::string &alias = alias_property->get_value();
							data_type_base::add_instance_alias(metadata->get_meta_type(), instance, alias, metadata->get_class_identifier());

							//for backwards compatibility, change instances of "_" in the identifier with "-" and add that as a further alias, and do the opposite as well
							if (alias.find("_") != std::string::npos) {
								std::string other_alias = alias;
								std::replace(other_alias.begin(), other_alias.end(), '_', '-');
								data_type_base::add_instance_alias(metadata->get_meta_type(), instance, other_alias, metadata->get_class_identifier());
							} else if (alias.find("-") != std::string::npos) {
								std::string other_alias = alias;
								std::replace(other_alias.begin(), other_alias.end(), '-', '_');
								data_type_base::add_instance_alias(metadata->get_meta_type(), instance, other_alias, metadata->get_class_identifier());
							}
						}
					} else {
						try {
							instance = data_type_base::get(identifier, metadata);
							instance->process_gsml_data(data_entry);
							instance->set_defined(true);
						} catch (...) {
							std::throw_with_nested(std::runtime_error("Error processing or loading data for " + metadata->get_class_identifier() + " instance \"" + identifier + "\"."));
						}
					}
				} catch (...) {
					exceptions.push_back(std::current_exception());
				}
			});
		}
	}

	if (!exceptions.empty()) {
		throw aggregate_exception(std::format("The database processing for {} instances failed.", metadata->get_class_identifier()), std::move(exceptions));
	}

	database_util::set_current_module(nullptr);
}

}
