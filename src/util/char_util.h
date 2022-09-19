#pragma once

namespace archimedes::char_t {

inline bool is_vowel(const char c)
{
	switch (c) {
		case 'a':
		case 'A':
		case 'e':
		case 'E':
		case 'i':
		case 'I':
		case 'o':
		case 'O':
		case 'u':
		case 'U':
			return true;
		default:
			return false;
	}
}

}
