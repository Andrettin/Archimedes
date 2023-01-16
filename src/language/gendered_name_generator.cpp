#include "archimedes.h"

#include "language/gendered_name_generator.h"

#include "language/name_generator.h"
#include "util/gender.h"

namespace archimedes {

gendered_name_generator::gendered_name_generator()
{
}

gendered_name_generator::~gendered_name_generator()
{
}

void gendered_name_generator::add_name(const gender gender, const name_variant &name)
{
	if (this->name_generators.find(gender) == this->name_generators.end()) {
		this->name_generators[gender] = std::make_unique<name_generator>();
	}

	this->name_generators[gender]->add_name(name);
}

void gendered_name_generator::add_names(const gender gender, const std::vector<name_variant> &names)
{
	if (this->name_generators.find(gender) == this->name_generators.end()) {
		this->name_generators[gender] = std::make_unique<name_generator>();
	}

	this->name_generators[gender]->add_names(names);
}

void gendered_name_generator::add_names(const gender gender, const std::vector<std::string> &names)
{
	if (this->name_generators.find(gender) == this->name_generators.end()) {
		this->name_generators[gender] = std::make_unique<name_generator>();
	}

	this->name_generators[gender]->add_names(names);
}

void gendered_name_generator::add_names_from(const std::unique_ptr<gendered_name_generator> &source_name_generator)
{
	for (const auto &kv_pair : source_name_generator->name_generators) {
		this->add_names(kv_pair.first, kv_pair.second->get_names());
	}

	//propagate ungendered names
	this->propagate_ungendered_names_from(source_name_generator);
}

void gendered_name_generator::propagate_ungendered_names_from(const gendered_name_generator *source_name_generator)
{
	const auto find_iterator = source_name_generator->name_generators.find(gender::none);
	if (find_iterator == source_name_generator->name_generators.end()) {
		return;
	}

	this->add_names(gender::male, find_iterator->second->get_names());
	this->add_names(gender::female, find_iterator->second->get_names());
}

}
