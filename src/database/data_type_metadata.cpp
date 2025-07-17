#include "archimedes.h"

#include "database/data_type_metadata.h"

#include "database/database.h"

namespace archimedes {

data_type_metadata::data_type_metadata(
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

const std::string &data_type_metadata::get_class_identifier() const
{
	return this->class_identifier;
}

bool data_type_metadata::has_database_dependency_on(const std::unique_ptr<data_type_metadata> &metadata) const
{
	if (this->database_dependencies.contains(metadata->get_class_identifier())) {
		return true;
	}

	for (const std::string &dependency_identifier : this->database_dependencies) {
		const data_type_metadata *dependency_metadata = database::get()->get_metadata(dependency_identifier);
		if (dependency_metadata->has_database_dependency_on(metadata)) {
			return true;
		}
	}

	return false;
}

size_t data_type_metadata::get_database_dependency_count() const
{
	size_t count = 0;

	for (const std::string &dependency_identifier : this->database_dependencies) {
		++count;

		const data_type_metadata *dependency_metadata = database::get()->get_metadata(dependency_identifier);
		count += dependency_metadata->get_database_dependency_count();
	}

	return count;
}

bool data_type_metadata::has_history_database_dependency_on(const std::string &class_identifier) const
{
	return this->history_database_dependencies.find(class_identifier) != this->history_database_dependencies.end();
}

bool data_type_metadata::has_history_database_dependency_on(const data_type_metadata *metadata) const
{
	return this->has_history_database_dependency_on(metadata->get_class_identifier());
}

size_t data_type_metadata::get_history_database_dependency_count() const
{
	return this->history_database_dependencies.size();
}

const data_type_metadata::parsing_function_type &data_type_metadata::get_parsing_function() const
{
	return this->parsing_function;
}

const data_type_metadata::processing_function_type &data_type_metadata::get_processing_function() const
{
	return this->processing_function;
}

const std::function<void()> &data_type_metadata::get_initialization_function() const
{
	return this->initialization_function;
}

const std::function<void()> &data_type_metadata::get_text_processing_function() const
{
	return this->text_processing_function;
}

const std::function<void()> &data_type_metadata::get_checking_function() const
{
	return this->checking_function;
}

const std::function<void()> &data_type_metadata::get_clearing_function() const
{
	return this->clearing_function;
}

const data_type_metadata::history_loading_function_type &data_type_metadata::get_history_loading_function() const
{
	return this->history_loading_function;
}

}
