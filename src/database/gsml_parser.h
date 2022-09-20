#pragma once

namespace archimedes {

class gsml_data;
enum class gsml_operator;

class gsml_parser final
{
public:
	gsml_parser();

	gsml_data parse(const std::filesystem::path &filepath);
	gsml_data parse(const std::string &gsml_string);

private:
	void parse(std::istream &istream, gsml_data &gsml_data);
	void parse_line(const std::string &line);
	bool parse_escaped_character(std::string &current_string, const char c);
	void parse_tokens();
	void reset();

private:
	std::vector<std::string> tokens;
	gsml_data *current_gsml_data = nullptr;
	std::string current_key;
	gsml_operator current_property_operator;
};

}
