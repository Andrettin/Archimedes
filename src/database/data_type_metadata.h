#pragma once

#include "database/data_module_container.h"
#include "util/qunique_ptr.h"

namespace archimedes {

class data_entry;
class data_module;
class game_rules_base;
class gsml_data;
class timeline;

//the metadata for a data type, including e.g. its initialization function
class data_type_metadata final
{
public:
	using instance_creation_function_type = std::function<qunique_ptr<data_entry>(const std::string &)>;
	using parsing_function_type = std::function<QCoro::Task<std::vector<gsml_data>>(const std::filesystem::path &, const data_type_metadata *)>;
	using processing_function_type = std::function<void(bool, const data_module_map<std::vector<gsml_data>> &, const data_type_metadata *)>;
	using history_loading_function_type = std::function<void(const QDate &, const timeline *, const game_rules_base *)>;

	explicit data_type_metadata(
		const std::string &class_identifier,
		const QMetaType &meta_type,
		const std::string &database_folder,
		const std::set<std::string> &database_dependencies,
		const std::set<std::string> &history_database_dependencies,
		const instance_creation_function_type &instance_creation_function,
		const parsing_function_type &parsing_function,
		const processing_function_type &processing_function,
		const std::function<void(const data_type_metadata *)> &initialization_function,
		const std::function<void(const data_type_metadata *)> &text_processing_function,
		const std::function<void(const data_type_metadata *)> &checking_function,
		const std::function<void()> &clearing_function,
		const history_loading_function_type &history_loading_function
	);

	const std::string &get_class_identifier() const;
	const QMetaType &get_meta_type() const;
	const std::string &get_database_folder() const;

	bool has_database_dependency_on(const std::unique_ptr<data_type_metadata> &metadata) const;
	size_t get_database_dependency_count() const;
	bool has_history_database_dependency_on(const std::string &class_identifier) const;
	bool has_history_database_dependency_on(const data_type_metadata *metadata) const;
	size_t get_history_database_dependency_count() const;

	const instance_creation_function_type &get_instance_creation_function() const;
	const parsing_function_type &get_parsing_function() const;
	const processing_function_type &get_processing_function() const;
	const std::function<void(const data_type_metadata *)> &get_initialization_function() const;
	const std::function<void(const data_type_metadata *)> &get_text_processing_function() const;
	const std::function<void(const data_type_metadata *)> &get_checking_function() const;
	const std::function<void()> &get_clearing_function() const;
	const history_loading_function_type &get_history_loading_function() const;

private:
	std::string class_identifier;
	QMetaType meta_type;
	std::string database_folder;
	const std::set<std::string> &database_dependencies;
	const std::set<std::string> &history_database_dependencies;
	instance_creation_function_type instance_creation_function;
	parsing_function_type parsing_function;
	processing_function_type processing_function;
	std::function<void(const data_type_metadata *)> initialization_function; //functions to initialize entries
	std::function<void(const data_type_metadata *)> text_processing_function; //functions to process text for entries
	std::function<void(const data_type_metadata *)> checking_function; //functions to check if data entries are valid
	std::function<void()> clearing_function; //functions to clear the data entries
	history_loading_function_type history_loading_function;
};

}
