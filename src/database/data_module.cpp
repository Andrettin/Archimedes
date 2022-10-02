#include "archimedes.h"

#include "database/data_module.h"

#include "database/database.h"
#include "util/container_util.h"

namespace archimedes {

void data_module::process_gsml_property(const gsml_property &property)
{
	database::get()->process_gsml_property_for_object(this, property);
}

void data_module::process_gsml_scope(const gsml_data &scope)
{
	const std::string &tag = scope.get_tag();
	const std::vector<std::string> &values = scope.get_values();

	if (tag == "dependencies") {
		for (const std::string &value : values) {
			this->add_dependency(database::get()->get_module(value));
		}
	} else {
		database::get()->process_gsml_scope_for_object(this, scope);
	}
}

}
