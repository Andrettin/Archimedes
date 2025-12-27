#include "archimedes.h"

#include "util/string_util.h"

#include "util/char_util.h"

namespace archimedes::string {


void normalize(std::string &str)
{
	//remove special characters from the string that shouldn't be displayed even in strings for which accented characters are acceptable

	//remove macrons
	string::replace(str, "\u0100", "A");
	string::replace(str, "\u0101", "a");
	string::replace(str, "\u0112", "E");
	string::replace(str, "\u0113", "e");
	string::replace(str, "\u012A", "I");
	string::replace(str, "\u012B", "i");
	string::replace(str, "\u014C", "O");
	string::replace(str, "\u014D", "o");
	string::replace(str, "\u016A", "U");
	string::replace(str, "\u016A", "u");

	//replace superscript versions of characters with their normal versions
	string::replace(str, "\u02B7", "w");
}


std::string get_indefinite_article(const std::string &str)
{
	if (!str.empty() && archimedes::char_t::is_vowel(str.front())) {
		const std::string lower_string = string::lowered(str);

		if (lower_string != "university") {
			return "an";
		}
	}

	return "a";
}

std::string to_code_page_437(const std::string &str)
{
	std::string result = str;
	string::replace(result, "\u00E1", "\xA0");
	string::replace(result, "\u00ED", "\xA1");
	string::replace(result, "\u00F3", "\xA2");
	string::replace(result, "\u00FA", "\xA3");
	string::replace(result, "\u00F1", "\xA4");
	return result;
}

}
