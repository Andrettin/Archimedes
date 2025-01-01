#include "archimedes.h"

#include "text_processor_base.h"

#include "language/word.h"
#include "util/assert_util.h"
#include "util/queue_util.h"
#include "util/string_util.h"

namespace archimedes {

std::queue<std::string> text_processor_base::get_subtokens(const std::string &token)
{
	std::queue<std::string> subtokens = string::split_to_queue(token, ':');

	if (subtokens.size() > 2) {
		throw std::runtime_error("There can only be at most 2 subtokens.");
	}

	return subtokens;
}

std::string text_processor_base::process_text(std::string &&text, const bool process_in_game_data) const
{
	size_t find_pos = 0;
	while ((find_pos = text.find('[', find_pos)) != std::string::npos) {
		const size_t end_pos = text.find(']', find_pos);
		if (end_pos == std::string::npos) {
			return text;
		}

		const std::string substring = text.substr(find_pos + 1, end_pos - (find_pos + 1));
		std::queue<std::string> tokens = string::split_to_queue(substring, '.');

		bool processed = true;
		const std::string replacement_str = this->process_tokens(std::move(tokens), process_in_game_data, processed);

		if (processed) {
			text.replace(find_pos, end_pos + 1 - find_pos, replacement_str);
		}

		++find_pos;
	}

	return text;
}

std::string text_processor_base::process_text(const std::string &text, const bool process_in_game_data) const
{
	std::string result = text;
	return this->process_text(std::move(result), process_in_game_data);
}

std::string text_processor_base::process_string_tokens(std::string &&str, std::queue<std::string> &&tokens) const
{
	if (tokens.empty()) {
		throw std::runtime_error("No tokens provided when processing string tokens.");
	}

	const std::string token = queue::take(tokens);

	if (token == "capitalized") {
		string::capitalize(str);
	} else if (token == "lowered") {
		string::to_lower(str);
	} else if (token == "normalized") {
		string::normalize(str);
	} else {
		throw std::runtime_error("Failed to process string token \"" + token + "\".");
	}

	if (!tokens.empty()) {
		return this->process_string_tokens(std::move(str), std::move(tokens));
	}

	return str;
}

std::string text_processor_base::process_named_data_entry_token(const named_data_entry *data_entry, const std::string &token) const
{
	if (data_entry == nullptr) {
		throw std::runtime_error("No data entry provided when processing a named data entry token.");
	}

	if (token == "name") {
		return data_entry->get_name();
	}

	throw std::runtime_error(std::format("Failed to process named data entry token \"{}\".", token));
}

std::string text_processor_base::process_named_data_entry_tokens(const named_data_entry *data_entry, std::queue<std::string> &tokens) const
{
	if (data_entry == nullptr) {
		throw std::runtime_error("No data entry provided when processing named data entry tokens.");
	}

	if (tokens.empty()) {
		throw std::runtime_error("No tokens provided when processing named data entry tokens.");
	}

	const std::string token = queue::take(tokens);
	return this->process_named_data_entry_token(data_entry, token);
}

std::string text_processor_base::process_word_tokens(const archimedes::word *word, std::queue<std::string> &tokens) const
{
	if (word == nullptr) {
		throw std::runtime_error("No word provided when processing word tokens.");
	}

	if (tokens.empty()) {
		throw std::runtime_error("No tokens provided when processing word tokens.");
	}

	const std::string token = queue::take(tokens);

	if (token == "meanings") {
		return this->process_word_meaning_tokens(word, tokens);
	} else {
		return this->process_named_data_entry_token(word, token);
	}
}

std::string text_processor_base::process_word_meaning_tokens(const archimedes::word *word, std::queue<std::string> &tokens) const
{
	if (word == nullptr) {
		throw std::runtime_error("No word provided when processing word meaning tokens.");
	}

	if (tokens.empty()) {
		throw std::runtime_error("No tokens provided when processing word meaning tokens.");
	}

	const std::string token = queue::take(tokens);

	return word->get_meanings().at(std::stoi(token) - 1);
}

}
