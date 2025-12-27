#include "archimedes.h"

#include "util/string_util.h"

#include "util/char_util.h"

namespace archimedes::string {

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
	string::replace(result, "\u00F1", "\xA4");
	return result;
}

}
