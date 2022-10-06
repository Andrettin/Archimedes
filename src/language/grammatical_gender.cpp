#include "archimedes.h"

#include "language/grammatical_gender.h"

namespace archimedes {

template class enum_converter<grammatical_gender>;

template <>
const enum_converter<grammatical_gender> enum_converter<grammatical_gender>::instance = enum_converter(
	"archimedes::grammatical_gender",
	{
		{ "none", grammatical_gender::none },
		{ "masculine", grammatical_gender::masculine },
		{ "feminine", grammatical_gender::feminine },
		{ "neuter", grammatical_gender::neuter }
	}
);

}
