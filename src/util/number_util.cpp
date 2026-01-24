#include "archimedes.h"

#include "util/number_util.h"

#include "util/locale_util.h"

namespace archimedes::number {

std::string to_formatted_string(const int number)
{
	return locale::english_locale.toString(number).toStdString();
}

static std::map<int, std::string> roman_numeral_conversion_table = { {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"}, {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"} };

std::string to_roman_numeral(int number)
{
	std::string numeral;

	for (std::map<int, std::string>::const_reverse_iterator iterator = roman_numeral_conversion_table.rbegin(); iterator != roman_numeral_conversion_table.rend(); ++iterator) {
		while (number >= iterator->first) {
			numeral += iterator->second;
			number -= iterator->first;
		}
	}

	return numeral;
}

}
