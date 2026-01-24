#include "archimedes.h"

#include "database/data_entry_history.h"

#include "database/database.h"

namespace archimedes {

void data_entry_history::process_gsml_property(const gsml_property &property, const QDate &date)
{
	Q_UNUSED(date);

	database::get()->process_gsml_property_for_object(this, property);
}

void data_entry_history::process_gsml_scope(const gsml_data &scope, const QDate &date)
{
	Q_UNUSED(date);

	database::get()->process_gsml_scope_for_object(this, scope);
}

}
