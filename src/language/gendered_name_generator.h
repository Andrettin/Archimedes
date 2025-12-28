#pragma once

#include "language/name_variant.h"

namespace archimedes {

class name_generator;
enum class gender;

class gendered_name_generator final
{
public:
	gendered_name_generator();
	~gendered_name_generator();

	const name_generator *get_name_generator(const gender gender) const
	{
		const auto find_iterator = this->name_generators.find(gender);
		if (find_iterator != this->name_generators.end()) {
			return find_iterator->second.get();
		}

		return nullptr;
	}

	void create_name_generator(const gender gender);
	void add_name(const gender gender, const name_variant &name);
	void add_names(const gender gender, const std::vector<name_variant> &names);
	void add_names(const gender gender, const std::vector<std::string> &names);
	void add_names_from(const std::unique_ptr<gendered_name_generator> &source_name_generator);

	void propagate_ungendered_names_from(const gendered_name_generator *source_name_generator);

	void propagate_ungendered_names_from(const std::unique_ptr<gendered_name_generator> &source_name_generator)
	{
		this->propagate_ungendered_names_from(source_name_generator.get());
	}

	void propagate_ungendered_names()
	{
		this->propagate_ungendered_names_from(this);
	}

	void set_markov_chain_size(const size_t size);

private:
	std::map<gender, std::unique_ptr<name_generator>> name_generators;
	size_t markov_chain_size = 0;
};

}
