#include "archimedes.h"

#include "util/markov_generator.h"

#include "util/assert_util.h"
#include "util/random.h"

namespace archimedes {

markov_generator::markov_generator(const size_t chain_size) : chain_size(chain_size)
{
	assert_throw(this->chain_size > 0);
}

void markov_generator::add_word(const std::string &word)
{
	assert_throw(this->chain_size > 0);

	for (size_t i = 0; i <= word.size(); ++i) {
		const size_t start = i > this->chain_size ? i - this->chain_size : 0;
		std::string prefix = word.substr(start, i - start);
		const char c = i != word.size() ? word[i] : 0;
		this->prefixes[std::move(prefix)].push_back(c);
	}
}

std::string markov_generator::generate_word() const
{
	assert_throw(!this->prefixes.empty());
	assert_throw(this->chain_size > 0);

	std::string prefix;
	std::string word;

	while (word.size() < this->max_length) {
		const auto it = this->prefixes.find(prefix);
		if (it == this->prefixes.end() || it->second.empty()) {
			return word;
		}

		const char c = it->second[random::get()->generate(it->second.size())];
		if (c == 0) {
			return word;
		}

		word.push_back(c);
		prefix.push_back(c);
		while (prefix.size() > this->chain_size) {
			prefix.erase(prefix.begin());
		}
	}

	//try to make names which reached the maximum length have an appropriate ending
	std::string original_word = word;
	while (!word.empty()) {
		const size_t prefix_length = this->chain_size < word.size() ? this->chain_size : word.size();
		prefix = word.substr(word.size() - prefix_length, prefix_length);

		const auto it = this->prefixes.find(prefix);
		if (it == this->prefixes.end() || it->second.empty()) {
			return word;
		}
		if (it->second.find(static_cast<char>(0)) != std::string::npos) {
			return word;
		}

		word.pop_back();
	}

	return original_word;
}

}
