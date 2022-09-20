#include "archimedes.h"

#include "database/gsml_property.h"

#include "database/gsml_operator.h"

namespace archimedes {

void gsml_property::print(std::ostream &ostream, const size_t indentation) const
{
	ostream << std::string(indentation, '\t') << this->get_key() << " ";

	switch (this->get_operator()) {
		case gsml_operator::assignment:
			ostream << "=";
			break;
		case gsml_operator::addition:
			ostream << "+=";
			break;
		case gsml_operator::subtraction:
			ostream << "-=";
			break;
		case gsml_operator::equality:
			ostream << "==";
			break;
		case gsml_operator::inequality:
			ostream << "!=";
			break;
		case gsml_operator::less_than:
			ostream << "<";
			break;
		case gsml_operator::less_than_or_equality:
			ostream << "<=";
			break;
		case gsml_operator::greater_than:
			ostream << ">";
			break;
		case gsml_operator::greater_than_or_equality:
			ostream << ">=";
			break;
		case gsml_operator::none:
			throw std::runtime_error("Cannot print the GSML \"none\" operator.");
	}

	ostream << " " << this->get_value() << "\n";
}

}
