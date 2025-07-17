#pragma once

#include "database/data_module_container.h"

namespace archimedes {

class data_module;
class game_rules_base;
class gsml_data;
class timeline;

//the metadata for a data type, including e.g. its initialization function
class data_type_metadata final
{
public:
	using parsing_function_type = std::function<QCoro::Task<std::vector<gsml_data>>(const std::filesystem::path &)>;
	using processing_function_type = std::function<void(bool, const data_module_map<std::vector<gsml_data>> &)>;
	using history_loading_function_type = std::function<void(const QDate &, const timeline *, const game_rules_base *)>;

	explicit data_type_metadata(
		const std::string &class_identifier,
		const std::set<std::string> &database_dependencies,
		const std::set<std::string> &history_database_dependencies,
		const parsing_function_type &parsing_function,
		const processing_function_type &processing_function,
		const std::function<void()> &initialization_function,
		const std::function<void()> &text_processing_function,
		const std::function<void()> &checking_function,
		const std::function<void()> &clearing_function,
		const history_loading_function_type &history_loading_function
	) : class_identifier(class_identifier),
		database_dependencies(database_dependencies), 
		history_database_dependencies(history_database_dependencies),
		parsing_function(parsing_function),
		processing_function(processing_function),
		initialization_function(initialization_function),
		text_processing_function(text_processing_function),
		checking_function(checking_function),
		clearing_function(clearing_function),
		history_loading_function(history_loading_function)
	{
	}

	const std::string &get_class_identifier() const
	{
		return this->class_identifier;
	}

	bool has_database_dependency_on(const std::unique_ptr<data_type_metadata> &metadata) const;
	size_t get_database_dependency_count() const;

	bool has_history_database_dependency_on(const std::string &class_identifier) const
	{
		return this->history_database_dependencies.find(class_identifier) != this->history_database_dependencies.end();
	}

	bool has_history_database_dependency_on(const data_type_metadata *metadata) const
	{
		return this->has_history_database_dependency_on(metadata->get_class_identifier());
	}

	size_t get_history_database_dependency_count() const
	{
		return this->history_database_dependencies.size();
	}

	const parsing_function_type &get_parsing_function() const
	{
		return this->parsing_function;
	}

	const processing_function_type &get_processing_function() const
	{
		return this->processing_function;
	}

	const std::function<void()> &get_initialization_function() const
	{
		return this->initialization_function;
	}

	const std::function<void()> &get_text_processing_function() const
	{
		return this->text_processing_function;
	}

	const std::function<void()> &get_checking_function() const
	{
		return this->checking_function;
	}

	const std::function<void()> &get_clearing_function() const
	{
		return this->clearing_function;
	}

	const history_loading_function_type &get_history_loading_function() const
	{
		return this->history_loading_function;
	}

private:
	std::string class_identifier;
	const std::set<std::string> &database_dependencies;
	const std::set<std::string> &history_database_dependencies;
	parsing_function_type parsing_function;
	processing_function_type processing_function;
	std::function<void()> initialization_function; //functions to initialize entries
	std::function<void()> text_processing_function; //functions to process text for entries
	std::function<void()> checking_function; //functions to check if data entries are valid
	std::function<void()> clearing_function; //functions to clear the data entries
	history_loading_function_type history_loading_function;
};

}
