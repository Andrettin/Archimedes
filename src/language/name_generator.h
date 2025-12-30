#pragma once

#include "language/name_variant.h"

namespace archimedes {

class markov_generator;
enum class gender;

class name_generator final
{
public:
	static constexpr size_t minimum_name_count = 10;

	name_generator();
	~name_generator();

	const std::vector<name_variant> &get_names() const
	{
		return this->names;
	}

	size_t get_name_count() const
	{
		return this->names.size();
	}

	bool has_enough_data() const;

	bool has_name(const std::string &name) const;

	void add_name(const name_variant &name);
	void add_names(const std::vector<name_variant> &names);
	void add_names(const std::vector<std::string> &names);

	std::string generate_name() const;
	std::string generate_name(const std::map<std::string, int> &used_name_counts) const;

	bool uses_markov_generation() const
	{
		return this->markov_generator != nullptr;
	}

	void set_markov_chain_size(const size_t size);

private:
	std::vector<name_variant> names; //name list for generation
	std::unique_ptr<archimedes::markov_generator> markov_generator;
};

}
