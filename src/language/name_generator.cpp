#include "archimedes.h"

#include "language/name_generator.h"

#include "util/assert_util.h"
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

bool name_generator::has_enough_base_data(const bool include_additional) const
{
	return this->get_name_count(include_additional) >= name_generator::minimum_name_count;
}

bool name_generator::has_enough_data() const
{
	return this->has_enough_base_data(true) || (this->markov_generator != nullptr && this->markov_generator->get_possible_word_count() >= name_generator::minimum_name_count);
}

bool name_generator::has_name(const std::string &name) const
{
	for (const name_variant &name_variant : this->names) {
		if (get_name_variant_string(name_variant) == name) {
			return true;
		}
	}

	for (const name_variant &name_variant : this->additional_names) {
		if (get_name_variant_string(name_variant) == name) {
			return true;
		}
	}

	return false;
}

void name_generator::add_name(const name_variant &name)
{
	if (!vector::contains(this->names, name)) {
		++this->unique_name_count;
	}

	this->names.push_back(name);

	this->add_name_to_markov_generator(name);
}

void name_generator::add_additional_name(const name_variant &name)
{
	if (!vector::contains(this->additional_names, name)) {
		++this->unique_additional_name_count;
	}

	this->additional_names.push_back(name);

	this->add_name_to_markov_generator(name);
}

void name_generator::add_name_to_markov_generator(const name_variant &name)
{
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

void name_generator::add_names(const std::vector<std::string> &names)
{
	for (const std::string &name : names) {
		this->add_name(name);
	}
}

void name_generator::add_names_from(const std::unique_ptr<name_generator> &source_name_generator)
{
	for (const auto &name_variant : source_name_generator->names) {
		this->add_name(name_variant);
	}

	for (const auto &name_variant : source_name_generator->additional_names) {
		this->add_additional_name(name_variant);
	}
}

std::string name_generator::generate_name() const
{
	assert_throw(this->has_data());

	//only use markov generation if there is not enough base data to have sufficient name diversity
	if (this->markov_generator != nullptr && !this->has_enough_base_data(true)) {
		return this->markov_generator->generate_word();
	}

	const bool include_additional = !this->has_enough_base_data(false);
	const size_t name_count = this->get_name_count(include_additional);

	const size_t random_name_index = random::get()->generate(name_count);
	const name_variant &name_variant = random_name_index < this->names.size() ? this->names.at(random_name_index) : this->additional_names.at(random_name_index - this->names.size());
	return get_name_variant_string(name_variant);
}

std::string name_generator::generate_name(const std::map<std::string, int> &used_name_counts) const
{
	if (!this->has_data()) {
		return std::string();
	}

	std::vector<name_variant> available_names;
	int max_count = 0;

	while (available_names.empty()) {
		available_names = this->names;
		if (!this->has_enough_base_data(false)) {
			vector::merge(available_names, this->additional_names);
		}
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

void name_generator::set_markov_chain_size(const size_t markov_chain_size)
{
	assert_throw(markov_chain_size > 0);
	assert_throw(this->markov_generator == nullptr);

	this->markov_generator = std::make_unique<archimedes::markov_generator>(markov_chain_size);

	for (const auto &name_variant : this->names) {
		this->markov_generator->add_word(get_name_variant_string(name_variant));
	}
	for (const auto &name_variant : this->additional_names) {
		this->markov_generator->add_word(get_name_variant_string(name_variant));
	}
}

}
