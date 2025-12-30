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

	this->max_length = std::max(this->max_length, word.size());

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

		if (word.size() >= this->max_length) {
			//start over if the word ended up being too long
			word.clear();
			prefix.clear();
		}
	}

	return "";
}

}
