#pragma once

namespace archimedes {

class markov_generator final
{
public:
	explicit markov_generator(const size_t chain_size);

	void add_word(const std::string &word);
	std::string generate_word() const;

	size_t get_possible_word_count();
	size_t calculate_possible_word_count();
	size_t calculate_possible_word_count(const std::string &prefix, const size_t word_length);

private:
	size_t chain_size = 2;
	size_t max_length = 0;
	std::map<std::string, std::string> prefixes;
	size_t possible_word_count = 0;
};

}
