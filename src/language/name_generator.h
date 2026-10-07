#pragma once

#include "language/name_variant.h"

namespace archimedes {

class markov_generator;
enum class gender;

class name_generator final
{
public:
	static constexpr size_t minimum_name_count = 100;
	static constexpr size_t default_markov_chain_size = 2;

	name_generator();
	~name_generator();

	const std::vector<name_variant> &get_names() const
	{
		return this->names;
	}

	size_t get_name_count(const bool include_additional) const
	{
		size_t count = this->unique_name_count;
		if (include_additional) {
			count += this->unique_additional_name_count;
		}
		return count;
	}

	bool has_data() const
	{
		return this->get_name_count(true) > 0;
	}

	bool has_enough_base_data(const bool include_additional) const;
	bool has_enough_data() const;

	bool has_name(const std::string &name) const;

	void add_name(const name_variant &name);
	void add_additional_name(const name_variant &name);
	void add_name_to_markov_generator(const name_variant &name);
	void add_names(const std::vector<std::string> &names);
	void add_names_from(const std::unique_ptr<name_generator> &source_name_generator);

	std::string generate_name() const;
	std::string generate_name(const std::map<std::string, int> &used_name_counts) const;

	void set_markov_chain_size(const size_t markov_chain_size);

private:
	std::vector<name_variant> names; //name list for generation
	std::vector<name_variant> additional_names; //names which should only be used if there is not sufficient base name variety
	size_t unique_name_count = 0;
	size_t unique_additional_name_count = 0;
	std::unique_ptr<archimedes::markov_generator> markov_generator;
};

}
