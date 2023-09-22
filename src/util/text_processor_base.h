#pragma once

namespace archimedes {

class named_data_entry;
class word;

class text_processor_base
{
public:
	virtual ~text_processor_base()
	{
	}

	static std::queue<std::string> get_subtokens(const std::string &token);

	std::string process_text(std::string &&text, const bool process_in_game_data) const;
	std::string process_text(const std::string &text, const bool process_in_game_data) const;
	virtual std::string process_tokens(std::queue<std::string> &&tokens, const bool process_in_game_data, bool &processed) const = 0;
	std::string process_string_tokens(std::string &&str, std::queue<std::string> &&tokens) const;
	std::string process_named_data_entry_token(const named_data_entry *data_entry, const std::string &token) const;
	std::string process_named_data_entry_tokens(const named_data_entry *data_entry, std::queue<std::string> &tokens) const;
	std::string process_word_tokens(const word *word, std::queue<std::string> &tokens) const;
	std::string process_word_meaning_tokens(const word *word, std::queue<std::string> &tokens) const;
};

}
