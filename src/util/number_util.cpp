#include "archimedes.h"

#include "util/number_util.h"

#include "util/locale_util.h"

namespace archimedes::number {

std::string to_formatted_string(const int number)
{
	return locale::english_locale.toString(number).toStdString();
}

}
