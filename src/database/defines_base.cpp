#include "archimedes.h"

#include "database/defines_base.h"

#include "database/database.h"
#include "database/gsml_data.h"
#include "database/gsml_parser.h"

namespace archimedes {

void defines_base::load(const std::filesystem::path &data_path)
{
	std::filesystem::path defines_path(data_path / "defines.txt");

	if (!std::filesystem::exists(defines_path)) {
		return;
	}

	gsml_parser parser;
	const gsml_data data = parser.parse(defines_path);
	database::process_gsml_data(this, data);
}

void defines_base::process_gsml_property(const gsml_property &property)
{
	database::get()->process_gsml_property_for_object(this, property);
}

void defines_base::process_gsml_scope(const gsml_data &scope)
{
	database::get()->process_gsml_scope_for_object(this, scope);
}

}
