#pragma once

namespace archimedes {

class markov_generator final
{
public:
	static constexpr size_t minimum_prefix_count = 5;

	explicit markov_generator(const size_t chain_size);

	bool has_enough_data() const
	{
		return this->prefixes.size() >= minimum_prefix_count;
	}

	void add_word(const std::string &word);
	std::string generate_word() const;

private:
	size_t chain_size = 2;
	size_t max_length = 12;
	std::map<std::string, std::string> prefixes;
};

}
