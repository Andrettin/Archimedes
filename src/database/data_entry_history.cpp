#include "archimedes.h"

#include "database/data_entry_history.h"

#include "database/database.h"

namespace archimedes {

void data_entry_history::process_gsml_property(const gsml_property &property)
{
	database::get()->process_gsml_property_for_object(this, property);
}

void data_entry_history::process_gsml_scope(const gsml_data &scope)
{
	database::get()->process_gsml_scope_for_object(this, scope);
}

}
