#include "archimedes.h"

#include "database/gsml_data.h"

#include "database/gsml_operator.h"
#include "database/gsml_property_visitor.h"
#include "util/geocoordinate.h"
#include "util/path_util.h"

namespace archimedes {

gsml_data::gsml_data(std::string &&tag)
	: tag(std::move(tag)), scope_operator(gsml_operator::assignment)
{
}

void gsml_data::add_property(gsml_property &&property)
{
	this->elements.push_back(std::move(property));
}

void gsml_data::add_property(const std::string &key, const std::string &value)
{
	this->add_property(gsml_property(key, gsml_operator::assignment, value));
}

void gsml_data::add_property(std::string &&key, const gsml_operator gsml_operator, std::string &&value)
{
	this->add_property(gsml_property(std::move(key), gsml_operator, std::move(value)));
}

geocoordinate gsml_data::to_geocoordinate() const
{
	if (this->get_values().size() != 2) {
		throw std::runtime_error("Geocoordinate scopes need to contain exactly two values.");
	}

	geocoordinate::number_type longitude = geocoordinate::number_type(this->get_values()[0]);
	geocoordinate::number_type latitude = geocoordinate::number_type(this->get_values()[1]);
	return geocoordinate(std::move(longitude), std::move(latitude));
}

void gsml_data::print_to_file(const std::filesystem::path &filepath) const
{
	std::ofstream ofstream(filepath);

	if (!ofstream) {
		throw std::runtime_error("Failed to open file \"" + filepath.string() + "\" for printing GSML data to.");
	}

	this->print_components(ofstream);
}

void gsml_data::print(std::ostream &ostream, const size_t indentation, const bool new_line) const
{
	if (new_line) {
		ostream << std::string(indentation, '\t');
	} else {
		ostream << " ";
	}
	if (!this->get_tag().empty()) {
		ostream << this->get_tag() << " ";
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
			case gsml_operator::none:
				throw std::runtime_error("Cannot print the GSML \"none\" operator.");
			default:
				throw std::runtime_error("Operator unsupported for GSML data: " + std::to_string(static_cast<int>(this->get_operator())));
		}
		ostream << " ";
	}
	ostream << "{";
	if (!this->is_minor()) {
		ostream << "\n";
	}

	this->print_components(ostream, indentation + 1);

	if (!this->is_minor()) {
		ostream << std::string(indentation, '\t');
	} else if (this->is_empty()) {
		//ensure that there is a white space between the braces of an empty GSML data
		ostream << " ";
	}
	ostream << "}";
	if (!this->is_minor()) {
		ostream << "\n";
	}
}

}
