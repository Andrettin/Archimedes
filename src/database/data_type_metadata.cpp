#include "archimedes.h"

#include "database/data_type_metadata.h"

#include "database/database.h"

namespace archimedes {

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

}
