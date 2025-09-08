#include "archimedes.h"

#include "database/gsml_data.h"

#include "database/gsml_operator.h"
#include "database/gsml_property_visitor.h"
#include "util/geocoordinate.h"
#include "util/path_util.h"

namespace archimedes {

gsml_data gsml_data::from_point(const QPoint &point, const std::string &tag)
{
	gsml_data point_data(tag);
	point_data.add_value(std::to_string(point.x()));
	point_data.add_value(std::to_string(point.y()));
	return point_data;
}

gsml_data gsml_data::from_size(const QSize &point, const std::string &tag)
{
	gsml_data point_data(tag);
	point_data.add_value(std::to_string(point.width()));
	point_data.add_value(std::to_string(point.height()));
	return point_data;
}

gsml_data gsml_data::from_rect(const QRect &rect, const std::string &tag)
{
	gsml_data rect_data(tag);

	const QPoint top_left = rect.topLeft();
	rect_data.add_value(std::to_string(top_left.x()));
	rect_data.add_value(std::to_string(top_left.y()));

	const QPoint bottom_right = rect.bottomRight();
	rect_data.add_value(std::to_string(bottom_right.x()));
	rect_data.add_value(std::to_string(bottom_right.y()));

	return rect_data;
}

gsml_data::gsml_data(std::string &&tag)
	: tag(std::move(tag)), scope_operator(gsml_operator::assignment)
{
}


bool gsml_data::has_children() const
{
	for (const auto &element : this->get_elements()) {
		if (std::holds_alternative<gsml_data>(element)) {
			return true;
		}
	}

	return false;
}

int gsml_data::get_children_count() const
{
	int count = 0;

	for (const auto &element : this->get_elements()) {
		if (std::holds_alternative<gsml_data>(element)) {
			++count;
		}
	}

	return count;
}

const gsml_data &gsml_data::get_child(const std::string &tag) const
{
	for (const auto &element : this->get_elements()) {
		if (!std::holds_alternative<gsml_data>(element)) {
			continue;
		}

		const gsml_data &child = std::get<gsml_data>(element);
		if (child.get_tag() == tag) {
			return child;
		}
	}

	throw std::runtime_error(std::format("No child with tag \"{}\" found for GSML data.", tag));
}

gsml_data &gsml_data::get_child(const std::string &tag)
{
	for (auto &element : this->elements) {
		if (!std::holds_alternative<gsml_data>(element)) {
			continue;
		}

		gsml_data &child = std::get<gsml_data>(element);
		if (child.get_tag() == tag) {
			return child;
		}
	}

	throw std::runtime_error(std::format("No child with tag \"{}\" found for GSML data.", tag));
}

bool gsml_data::has_child(const std::string &tag) const
{
	for (const auto &element : this->get_elements()) {
		if (!std::holds_alternative<gsml_data>(element)) {
			continue;
		}

		if (std::get<gsml_data>(element).get_tag() == tag) {
			return true;
		}
	}

	return false;
}

gsml_data &gsml_data::add_child(gsml_data &&child)
{
	this->elements.emplace_back(std::move(child));
	return std::get<gsml_data>(this->elements.back());
}

gsml_data &gsml_data::add_child(std::string &&tag, gsml_data &&child)
{
	child.tag = std::move(tag);
	return this->add_child(std::move(child));
}

gsml_data &gsml_data::add_child()
{
	return this->add_child(gsml_data());
}

gsml_data &gsml_data::add_child(std::string &&tag, const gsml_operator gsml_operator)
{
	this->elements.push_back(gsml_data(std::move(tag), gsml_operator));
	return std::get<gsml_data>(this->elements.back());
}

void gsml_data::remove_child(const std::string &tag)
{
	for (size_t i = 0; i < this->elements.size(); ++i) {
		const auto &element = this->elements.at(i);

		if (!std::holds_alternative<gsml_data>(element)) {
			continue;
		}

		const gsml_data &child = std::get<gsml_data>(element);
		if (child.get_tag() == tag) {
			this->elements.erase(this->elements.begin() + i);
			return;
		}
	}

	throw std::runtime_error("No child with tag \"" + tag + "\" found for GSML data.");
}

std::vector<const gsml_property *> gsml_data::try_get_properties(const std::string &key) const
{
	std::vector<const gsml_property *> properties;

	this->for_each_property([&](const gsml_property &property) {
		if (property.get_key() == key) {
			properties.push_back(&property);
		}
	});

	return properties;
}

const std::string &gsml_data::get_property_value(const std::string &key) const
{
	for (const auto &element : this->get_elements()) {
		if (!std::holds_alternative<gsml_property>(element)) {
			continue;
		}

		const gsml_property &property = std::get<gsml_property>(element);
		if (property.get_key() == key) {
			return property.get_value();
		}
	}

	throw std::runtime_error("No property with key \"" + key + "\" found for GSML data.");
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

void gsml_data::clear_properties()
{
	//remove all property elements
	for (size_t i = 0; i < this->elements.size();) {
		const auto &element = this->elements.at(i);

		if (std::holds_alternative<gsml_property>(element)) {
			this->elements.erase(this->elements.begin() + i);
		} else {
			++i;
		}
	}
}

QColor gsml_data::to_color() const
{
	if (this->get_values().size() != 3) {
		throw std::runtime_error("Color scopes need to contain exactly three values.");
	}

	const int red = std::stoi(this->values.at(0));
	const int green = std::stoi(this->values.at(1));
	const int blue = std::stoi(this->values.at(2));

	return QColor(red, green, blue);
}

QPoint gsml_data::to_point() const
{
	if (this->get_values().size() != 2) {
		throw std::runtime_error("Point scopes need to contain exactly two values.");
	}

	const int x = std::stoi(this->get_values()[0]);
	const int y = std::stoi(this->get_values()[1]);
	return QPoint(x, y);
}

QPointF gsml_data::to_pointf() const
{
	if (this->get_values().size() != 2) {
		throw std::runtime_error("Point scopes need to contain exactly two values.");
	}

	const double x = std::stod(this->get_values()[0]);
	const double y = std::stod(this->get_values()[1]);
	return QPointF(x, y);
}

QSize gsml_data::to_size() const
{
	if (this->get_values().size() != 2) {
		throw std::runtime_error("Size scopes need to contain exactly two values.");
	}

	const int width = std::stoi(this->get_values()[0]);
	const int height = std::stoi(this->get_values()[1]);
	return QSize(width, height);
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

QRect gsml_data::to_rect() const
{
	if (this->get_values().size() != 4) {
		throw std::runtime_error("Rect scopes need to contain exactly four values.");
	}

	const int min_x = std::stoi(this->get_values()[0]);
	const int min_y = std::stoi(this->get_values()[1]);
	const int max_x = std::stoi(this->get_values()[2]);
	const int max_y = std::stoi(this->get_values()[3]);
	return QRect(QPoint(min_x, min_y), QPoint(max_x, max_y));
}

void gsml_data::print_to_file(const std::filesystem::path &filepath) const
{
	std::ofstream ofstream(filepath);

	if (!ofstream) {
		throw std::runtime_error("Failed to open file \"" + filepath.string() + "\" for printing GSML data to.");
	}

	this->print_components(ofstream);
}

void gsml_data::print_to_dir(const std::filesystem::path &directory) const
{
	const std::filesystem::path filepath = directory / (this->get_tag() + ".txt");
	this->print_to_file(filepath);
}

std::string gsml_data::print_to_string() const
{
	std::ostringstream ostream;
	this->print_components(ostream);
	return ostream.str();
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

void gsml_data::print_components(std::ostream &ostream, const size_t indentation) const
{
	if (!this->get_values().empty()) {
		if (this->is_minor()) {
			ostream << " ";
		} else {
			ostream << std::string(indentation, '\t');
		}
	}
	for (const std::string &value : this->get_values()) {
		ostream << value << " ";
	}
	if (!this->get_values().empty()) {
		if (!this->is_minor()) {
			ostream << "\n";
		}
	}

	this->for_each_property([&](const gsml_property &property) {
		property.print(ostream, indentation);
	});

	bool new_line = true;
	this->for_each_child([&](const gsml_data &child_data) {
		child_data.print(ostream, indentation, new_line);
		if (new_line && child_data.is_minor()) {
			new_line = false;
		}
	});

	//if the last child data was minor and did not print a new line, print one now
	if (!new_line) {
		ostream << "\n";
	}
}

}
