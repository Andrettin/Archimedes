#include "archimedes.h"

#include "language/name_generator.h"

#include "util/gender.h"
#include "util/vector_random_util.h"
#include "util/vector_util.h"

namespace archimedes {

bool name_generator::is_name_valid(const std::string &name) const
{
	for (const name_variant &name_variant : this->names) {
		if (get_name_variant_string(name_variant) == name) {
			return true;
		}
	}

	return false;
}

void name_generator::add_names(const std::vector<name_variant> &names)
{
	vector::merge(this->names, names);
}

void name_generator::add_names(const std::vector<std::string> &names)
{
	vector::merge(this->names, names);
}

std::string name_generator::generate_name() const
{
	const name_variant &name_variant = vector::get_random(this->names);
	return get_name_variant_string(name_variant);
}

}
