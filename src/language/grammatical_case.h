#pragma once

namespace archimedes {

enum class grammatical_case {
	none,
	nominative,
	accusative,
	dative,
	genitive
};

}

Q_DECLARE_METATYPE(archimedes::grammatical_case)
