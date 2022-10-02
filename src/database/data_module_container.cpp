#include "archimedes.h"

#include "database/data_module_container.h"

#include "database/data_module.h"

namespace archimedes {

bool data_module_compare::operator()(const data_module *data_module, const archimedes::data_module *other_data_module) const
{
	if ((data_module == nullptr) != (other_data_module == nullptr)) {
		return data_module == nullptr;
	}

	if (data_module->depends_on(other_data_module)) {
		return false;
	} else if (other_data_module->depends_on(data_module)) {
		return true;
	}

	if (data_module->get_dependency_count() != other_data_module->get_dependency_count()) {
		return data_module->get_dependency_count() < other_data_module->get_dependency_count();
	}

	return data_module->get_identifier() < other_data_module->get_identifier();
}

}
