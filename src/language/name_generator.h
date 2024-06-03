#pragma once

#include "language/name_variant.h"

namespace archimedes {

enum class gender;

class name_generator final
{
public:
	static constexpr size_t minimum_name_count = 10;

	const std::vector<name_variant> &get_names() const
	{
		return this->names;
	}

	size_t get_name_count() const
	{
		return this->names.size();
	}

	bool is_name_valid(const std::string &name) const;

	void add_name(const name_variant &name)
	{
		this->names.push_back(name);
	}

	void add_names(const std::vector<name_variant> &names);
	void add_names(const std::vector<std::string> &names);

	std::string generate_name() const;
	std::string generate_name(const std::set<std::string> &used_names) const;

private:
	std::vector<name_variant> names; //name list for generation
};

}
