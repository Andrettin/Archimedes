#pragma once

namespace archimedes {

class markov_generator final
{
public:
	explicit markov_generator(const size_t chain_size);

	void add_word(const std::string &word);
	std::string generate_word() const;

private:
	size_t chain_size = 2;
	size_t max_length = 0;
	std::map<std::string, std::string> prefixes;
};

}
