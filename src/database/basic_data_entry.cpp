#include "archimedes.h"

#include "database/basic_data_entry.h"

#include "database/database.h"

namespace archimedes {

void basic_data_entry::process_gsml_property(const gsml_property &property)
{
	database::get()->process_gsml_property_for_object(this, property);
}

void basic_data_entry::process_gsml_scope(const gsml_data &scope)
{
	database::get()->process_gsml_scope_for_object(this, scope);
}

}
