#include "archimedes.h"

#include "language/grammatical_gender.h"

#include "util/gender.h"

namespace archimedes {

gender grammatical_gender_to_gender(const grammatical_gender grammatical_gender)
{
	switch (grammatical_gender) {
		case grammatical_gender::masculine:
			return gender::male;
		case grammatical_gender::feminine:
			return gender::female;
		default:
			return gender::none;
	}
}

}
