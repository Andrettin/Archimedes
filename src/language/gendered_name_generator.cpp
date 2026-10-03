#include "archimedes.h"

#include "language/gendered_name_generator.h"

#include "language/name_generator.h"
#include "util/assert_util.h"
#include "util/gender.h"

namespace archimedes {

gendered_name_generator::gendered_name_generator()
{
}

gendered_name_generator::~gendered_name_generator()
{
}

void gendered_name_generator::create_name_generator(const gender gender)
{
	auto name_generator = std::make_unique<archimedes::name_generator>();
	if (this->markov_chain_size != 0) {
		name_generator->set_markov_chain_size(this->markov_chain_size);
	}
	this->name_generators[gender] = std::move(name_generator);
}

bool gendered_name_generator::has_enough_data() const
{
	return this->has_enough_data(gender::female) && this->has_enough_data(gender::male);
}

bool gendered_name_generator::has_enough_data(const gender gender) const
{
	const name_generator *name_generator = this->get_name_generator(gender);

	if (name_generator != nullptr) {
		return name_generator->has_enough_data();
	}

	return false;
}

void gendered_name_generator::add_name(const gender gender, const name_variant &name)
{
	if (!this->name_generators.contains(gender)) {
		this->create_name_generator(gender);
	}

	this->name_generators[gender]->add_name(name);
}

void gendered_name_generator::add_additional_name(const gender gender, const name_variant &name)
{
	if (!this->name_generators.contains(gender)) {
		this->create_name_generator(gender);
	}

	this->name_generators[gender]->add_additional_name(name);
}

void gendered_name_generator::add_names(const gender gender, const std::vector<std::string> &names)
{
	if (!this->name_generators.contains(gender)) {
		this->create_name_generator(gender);
	}

	this->name_generators[gender]->add_names(names);
}

void gendered_name_generator::add_names_from(const std::unique_ptr<gendered_name_generator> &source_name_generator)
{
	for (const auto &kv_pair : source_name_generator->name_generators) {
		this->add_names_from(kv_pair.first, kv_pair.second);
	}

	//propagate ungendered names
	this->propagate_ungendered_names_from(source_name_generator);
}

void gendered_name_generator::add_names_from(const gender gender, const std::unique_ptr<name_generator> &source_name_generator)
{
	if (!this->name_generators.contains(gender)) {
		this->create_name_generator(gender);
	}

	this->name_generators[gender]->add_names_from(source_name_generator);
}

void gendered_name_generator::propagate_ungendered_names_from(const gendered_name_generator *source_name_generator)
{
	const auto find_iterator = source_name_generator->name_generators.find(gender::none);
	if (find_iterator == source_name_generator->name_generators.end()) {
		return;
	}

	this->add_names_from(gender::male, find_iterator->second);
	this->add_names_from(gender::female, find_iterator->second);
}

void gendered_name_generator::set_markov_chain_size(const size_t markov_chain_size)
{
	assert_throw(markov_chain_size > 0);

	this->markov_chain_size = markov_chain_size;

	for (const auto &[gender, name_generator] : this->name_generators) {
		name_generator->set_markov_chain_size(this->markov_chain_size);
	}
}

}
