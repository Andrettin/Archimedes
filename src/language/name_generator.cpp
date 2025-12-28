#include "archimedes.h"

#include "language/name_generator.h"

#include "util/assert_util.h"
#include "util/gender.h"
#include "util/markov_generator.h"
#include "util/string_util.h"
#include "util/vector_random_util.h"
#include "util/vector_util.h"

namespace archimedes {

name_generator::name_generator()
{
}

name_generator::~name_generator()
{
}

bool name_generator::has_enough_data() const
{
	return this->get_name_count() >= name_generator::minimum_name_count;
}

bool name_generator::is_name_valid(const std::string &name) const
{
	for (const name_variant &name_variant : this->names) {
		if (get_name_variant_string(name_variant) == name) {
			return true;
		}
	}

	return false;
}

void name_generator::add_name(const name_variant &name)
{
	this->names.push_back(name);

	if (this->markov_generator != nullptr) {
		const std::string &name_str = get_name_variant_string(name);
		if (name_str.contains(' ')) {
			const std::vector<std::string> words = string::split(name_str, ' ');
			for (const std::string &word : words) {
				assert_throw(!word.empty());
				if (!std::isupper(word.at(0))) {
					//ignore prepositions
					continue;
				}

				this->markov_generator->add_word(word);
			}
		} else {
			this->markov_generator->add_word(name_str);
		}
	}
}

void name_generator::add_names(const std::vector<name_variant> &names)
{
	for (const auto &name_variant : names) {
		this->add_name(name_variant);
	}
}

void name_generator::add_names(const std::vector<std::string> &names)
{
	for (const std::string &name : names) {
		this->add_name(name);
	}
}

std::string name_generator::generate_name() const
{
	assert_throw(!this->names.empty());

	if (this->markov_generator != nullptr) {
		return this->markov_generator->generate_word();
	}

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

void name_generator::set_markov_chain_size(const size_t size)
{
	this->markov_generator = std::make_unique<archimedes::markov_generator>(size);

	for (const auto &name_variant : this->names) {
		this->markov_generator->add_word(get_name_variant_string(name_variant));
	}
}

}
