#include "archimedes.h"

#include "language/name_generator.h"

#include "util/assert_util.h"
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
	assert_throw(!this->names.empty());
	const name_variant &name_variant = vector::get_random(this->names);
	return get_name_variant_string(name_variant);
}

std::string name_generator::generate_name(const std::map<std::string, int> &used_name_counts) const
{
	if (this->names.empty()) {
		return std::string();
	}

	std::vector<name_variant> available_names;
	int max_count = 0;

	while (available_names.empty()) {
		available_names = this->names;
		std::erase_if(available_names, [&used_name_counts, max_count](const name_variant &name_variant) {
			const auto find_iterator = used_name_counts.find(get_name_variant_string(name_variant));
			if (find_iterator != used_name_counts.end() && find_iterator->second > max_count) {
				return true;
			}

			return false;
		});

		++max_count;
	}

	const name_variant &name_variant = vector::get_random(available_names);
	return get_name_variant_string(name_variant);
}

}
