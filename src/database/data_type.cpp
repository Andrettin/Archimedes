#include "archimedes.h"

#include "database/data_type.h"

#include "database/data_entry.h"

namespace archimedes {

std::map<int, std::vector<data_entry *>> data_type_base::instances;
std::map<int, std::map<std::string, qunique_ptr<data_entry>>> data_type_base::instances_by_identifier;
std::map<int, std::map<std::string, data_entry *>> data_type_base::instances_by_alias;

data_entry *data_type_base::try_get(const QMetaType &meta_type, const std::string &identifier)
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

data_entry *data_type_base::add(const QMetaType &meta_type, const std::string &identifier, const data_module *data_module, const std::string_view &class_identifier, qunique_ptr<data_entry> &&instance)
{
	if (identifier.empty()) {
		throw std::runtime_error("Tried to add a " + std::string(class_identifier) + " instance with an empty string identifier.");
	}

	if (data_type_base::exists(meta_type, identifier)) {
		throw std::runtime_error("Tried to add a " + std::string(class_identifier) + " instance with the already-used \"" + identifier + "\" string identifier.");
	}

	data_entry *instance_ptr = instance.get();
	data_type_base::instances_by_identifier[meta_type.id()][identifier] = std::move(instance);

	data_type_base::instances[meta_type.id()].push_back(instance_ptr);
	instance_ptr->moveToThread(QApplication::instance()->thread());
	instance_ptr->set_module(data_module);

	//for backwards compatibility, change instances of "_" in the identifier with "-" and add that as an alias, and do the opposite as well
	if (identifier.find("_") != std::string::npos) {
		std::string alias = identifier;
		std::replace(alias.begin(), alias.end(), '_', '-');
		data_type_base::add_instance_alias(meta_type, instance_ptr, alias, class_identifier);
	}

	if (identifier.find("-") != std::string::npos) {
		std::string alias = identifier;
		std::replace(alias.begin(), alias.end(), '-', '_');
		data_type_base::add_instance_alias(meta_type, instance_ptr, alias, class_identifier);
	}

	return instance_ptr;
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

}
