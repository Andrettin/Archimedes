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

	this->possible_word_count = 0; //recalculation needed
}

std::string markov_generator::generate_word() const
{
	assert_throw(!this->prefixes.empty());
	assert_throw(this->chain_size > 0);

	std::string prefix;
	std::string word;

	while (word.size() <= this->max_length) {
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

		if (word.size() > this->max_length) {
			//start over if the word ended up being too long
			word.clear();
			prefix.clear();
		}
	}

	return "";
}

size_t markov_generator::get_possible_word_count()
{
	if (this->possible_word_count == 0) {
		this->possible_word_count = this->calculate_possible_word_count();
		assert_throw(this->possible_word_count > 0);
	}

	return this->possible_word_count;
}

size_t markov_generator::calculate_possible_word_count()
{
	return this->calculate_possible_word_count("", 0);
}

size_t markov_generator::calculate_possible_word_count(const std::string &prefix, const size_t word_length)
{
	assert_throw(!this->prefixes.empty());
	assert_throw(this->chain_size > 0);

	if (word_length > this->max_length) {
		return 0;
	}

	const auto it = this->prefixes.find(prefix);
	if (it == this->prefixes.end() || it->second.empty()) {
		return 1;
	}

	size_t word_count = 0;

	std::array<bool, std::numeric_limits<unsigned char>::max()> checked_values{};
	checked_values.fill(false);

	for (const char c : it->second) {
		if (checked_values[static_cast<unsigned char>(c)]) {
			continue;
		}

		checked_values[static_cast<unsigned char>(c)] = true;

		if (c == 0) {
			++word_count;
			continue;
		}

		std::string new_prefix = prefix;

		new_prefix.push_back(c);
		while (new_prefix.size() > this->chain_size) {
			new_prefix.erase(new_prefix.begin());
		}

		word_count += this->calculate_possible_word_count(new_prefix, word_length + 1);
	}

	return word_count;
}

}
