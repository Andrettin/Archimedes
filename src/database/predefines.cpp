#include "archimedes.h"

#include "database/predefines.h"

#include "database/database.h"
#include "database/gsml_data.h"
#include "database/gsml_parser.h"

namespace archimedes {

void predefines::load(const std::filesystem::path &data_path)
{
	std::filesystem::path predefines_path(data_path / "predefines.txt");

	if (!std::filesystem::exists(predefines_path)) {
		return;
	}

	gsml_parser parser;
	const gsml_data data = parser.parse(predefines_path);
	database::process_gsml_data(this, data);
}

void predefines::process_gsml_property(const gsml_property &property)
{
	database::get()->process_gsml_property_for_object(this, property);
}

void predefines::process_gsml_scope(const gsml_data &scope)
{
	database::get()->process_gsml_scope_for_object(this, scope);
}

}
