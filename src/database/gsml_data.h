#pragma once

#include "database/gsml_data_visitor.h"
#include "database/gsml_element_visitor.h"
#include "database/gsml_property.h"
#include "database/gsml_property_visitor.h"

namespace archimedes {

class geocoordinate;
class gsml_parser;

//grand strategy markup language data
class gsml_data final
{
public:
	static gsml_data from_point(const QPoint &point, const std::string &tag = std::string());
	static gsml_data from_size(const QSize &point, const std::string &tag = std::string());
	static gsml_data from_rect(const QRect &rect, const std::string &tag = std::string());

	explicit gsml_data(std::string &&tag = std::string());

	explicit gsml_data(std::string &&tag, const gsml_operator scope_operator)
		: tag(std::move(tag)), scope_operator(scope_operator)
	{
	}

	explicit gsml_data(const std::string &tag) : gsml_data(std::string(tag))
	{
	}

	explicit gsml_data(const std::string &tag, const gsml_operator scope_operator)
		: gsml_data(std::string(tag), scope_operator)
	{
	}

	const std::string &get_tag() const
	{
		return this->tag;
	}

	gsml_operator get_operator() const
	{
		return this->scope_operator;
	}

	const gsml_data *get_parent() const
	{
		return this->parent;
	}

	bool has_children() const;
	int get_children_count() const;
	const gsml_data &get_child(const std::string &tag) const;
	bool has_child(const std::string &tag) const;
	gsml_data &add_child(gsml_data &&child);
	gsml_data &add_child(std::string &&tag, gsml_data &&child);
	gsml_data &add_child();
	gsml_data &add_child(std::string &&tag, const gsml_operator gsml_operator);
	void remove_child(const std::string &tag);

	template <typename function_type>
	void for_each_child(const function_type &function) const
	{
		const gsml_data_visitor visitor(function);
		for (const auto &element : this->get_elements()) {
			std::visit(visitor, element);
		}
	}

	std::vector<const gsml_property *> try_get_properties(const std::string &key) const;
	const std::string &get_property_value(const std::string &key) const;
	void add_property(gsml_property &&property);
	void add_property(const std::string &key, const std::string &value);
	void add_property(std::string &&key, const gsml_operator gsml_operator, std::string &&value);

	template <typename function_type>
	void for_each_property(const function_type &function) const
	{
		const gsml_property_visitor visitor(function);
		for (const auto &element : this->get_elements()) {
			std::visit(visitor, element);
		}
	}

	void clear_properties();

	const std::vector<std::string> &get_values() const
	{
		return this->values;
	}

	void add_value(const std::string &value)
	{
		this->values.push_back(value);
	}

	void add_value(std::string &&value)
	{
		this->values.push_back(std::move(value));
	}

	bool is_empty() const
	{
		return this->get_elements().empty() && this->get_values().empty();
	}

	const std::vector<std::variant<gsml_property, gsml_data>> &get_elements() const
	{
		return this->elements;
	}

	template <typename property_function_type, typename data_function_type>
	void for_each_element(const property_function_type &property_function, const data_function_type &data_function) const
	{
		const gsml_element_visitor visitor(property_function, data_function);
		for (const auto &element : this->get_elements()) {
			std::visit(visitor, element);
		}
	}

	template <typename T>
	void process(T *instance) const
	{
		this->for_each_element([instance](const gsml_property &property) {
			instance->process_gsml_property(property);
		}, [instance](const gsml_data &scope) {
			instance->process_gsml_scope(scope);
		});
	}

	QColor to_color() const;
	QPoint to_point() const;
	QPointF to_pointf() const;
	QSize to_size() const;
	geocoordinate to_geocoordinate() const;
	QRect to_rect() const;

	void print_to_file(const std::filesystem::path &filepath) const;
	void print_to_dir(const std::filesystem::path &directory) const;
	std::string print_to_string() const;
	void print(std::ostream &ostream, const size_t indentation, const bool new_line) const;
	void print_components(std::ostream &ostream, const size_t indentation = 0) const;

private:
	bool is_minor() const
	{
		//get whether this is minor GSML data, e.g. just containing a few simple values
		return this->get_tag().empty() && this->get_elements().empty() && this->get_values().size() < 5;
	}

private:
	std::string tag;
	gsml_operator scope_operator;
	gsml_data *parent = nullptr;
	std::vector<std::string> values; //values directly attached to the GSML data scope, used for e.g. name arrays
	std::vector<std::variant<gsml_property, gsml_data>> elements;

	friend gsml_parser;
};

}
