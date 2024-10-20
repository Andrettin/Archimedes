#pragma once

#include "database/gsml_data.h"
#include "database/gsml_operator.h"
#include "database/gsml_property.h"
#include "database/named_data_entry.h"

namespace archimedes {

template <typename scope_type, typename context_type>
class condition_base
{
public:
	//get the string for the object of a condition, e.g. the unit type for a unit type condition
	template <typename T>
	static std::string get_object_string(const T *object, const std::string &name_string = "")
	{
		return condition_base<scope_type, context_type>::get_object_highlighted_name(object, name_string);
	}

	static std::string get_object_highlighted_name(const named_data_entry *object, const std::string &name_string);

	static std::unique_ptr<const condition_base<scope_type, context_type>> from_gsml_property(const gsml_property &property);
	template <typename main_condition_type>
	static std::unique_ptr<const condition_base<scope_type, context_type>> from_gsml_scope(const gsml_data &scope);

	static std::string get_conditions_string(const std::vector<std::unique_ptr<const condition_base<scope_type, context_type>>> &conditions, const size_t indent)
	{
		std::string conditions_string;
		bool first = true;
		for (const std::unique_ptr<const condition_base<scope_type, context_type>> &condition : conditions) {
			if (condition->is_hidden()) {
				continue;
			}

			const std::string condition_string = condition->get_string(indent);
			if (condition_string.empty()) {
				continue;
			}

			if (first) {
				first = false;
			} else {
				conditions_string += "\n";
			}

			if (indent > 0) {
				conditions_string += std::string(indent, '\t');
			}

			conditions_string += condition_string;
		}
		return conditions_string;
	}

	explicit condition_base(const gsml_operator condition_operator) : condition_operator(condition_operator)
	{
	}

	virtual ~condition_base()
	{
	}

	virtual const std::string &get_class_identifier() const = 0;

	virtual void process_gsml_property(const gsml_property &property)
	{
		throw std::runtime_error("Invalid \"" + this->get_class_identifier() + "\" condition property: \"" + property.get_key() + "\".");
	}

	virtual void process_gsml_scope(const gsml_data &scope)
	{
		throw std::runtime_error("Invalid \"" + this->get_class_identifier() + "\" condition scope: \"" + scope.get_tag() + "\".");
	}

	virtual void check_validity() const
	{
	}

	bool check(const scope_type *scope, const context_type &ctx) const
	{
		switch (this->condition_operator) {
			case gsml_operator::assignment:
				return this->check_assignment(scope, ctx);
			case gsml_operator::equality:
				return this->check_equality(scope);
			case gsml_operator::inequality:
				return this->check_inequality(scope);
			case gsml_operator::less_than:
				return this->check_less_than(scope);
			case gsml_operator::less_than_or_equality:
				return this->check_less_than_or_equality(scope);
			case gsml_operator::greater_than:
				return this->check_greater_than(scope);
			case gsml_operator::greater_than_or_equality:
				return this->check_greater_than_or_equality(scope);
			default:
				throw std::runtime_error("Invalid condition operator: \"" + std::to_string(static_cast<int>(this->condition_operator)) + "\".");
		}
	}

	virtual bool check_assignment(const scope_type *scope, const context_type &ctx) const = 0;

	virtual bool check_equality(const scope_type *scope) const
	{
		Q_UNUSED(scope);

		throw std::runtime_error("The equality operator is not supported for \"" + this->get_class_identifier() + "\" conditions.");
	}

	virtual bool check_inequality(const scope_type *scope) const
	{
		return !this->check_equality(scope);
	}

	virtual bool check_less_than(const scope_type *scope) const
	{
		Q_UNUSED(scope);

		throw std::runtime_error("The less than operator is not supported for \"" + this->get_class_identifier() + "\" conditions.");
	}

	virtual bool check_less_than_or_equality(const scope_type *scope) const
	{
		return this->check_equality(scope) || this->check_less_than(scope);
	}

	virtual bool check_greater_than(const scope_type *scope) const
	{
		Q_UNUSED(scope);

		throw std::runtime_error("The greater than operator is not supported for \"" + this->get_class_identifier() + "\" conditions.");
	}

	virtual bool check_greater_than_or_equality(const scope_type *scope) const
	{
		return this->check_equality(scope) || this->check_greater_than(scope);
	}

	std::string get_string(const size_t indent) const
	{
		switch (this->condition_operator) {
			case gsml_operator::assignment:
				return this->get_assignment_string(indent);
			case gsml_operator::equality:
				return this->get_equality_string();
			case gsml_operator::inequality:
				return this->get_inequality_string();
			case gsml_operator::less_than:
				return this->get_less_than_string();
			case gsml_operator::less_than_or_equality:
				return this->get_less_than_or_equality_string();
			case gsml_operator::greater_than:
				return this->get_greater_than_string();
			case gsml_operator::greater_than_or_equality:
				return this->get_greater_than_or_equality_string();
			default:
				throw std::runtime_error("Invalid condition operator: \"" + std::to_string(static_cast<int>(this->condition_operator)) + "\".");
		}
	}

	virtual std::string get_assignment_string(const size_t indent) const = 0;

	virtual std::string get_equality_string() const
	{
		throw std::runtime_error("The equality operator is not supported for \"" + this->get_class_identifier() + "\" conditions.");
	}

	virtual std::string get_inequality_string() const
	{
		throw std::runtime_error("The inequality operator is not supported for \"" + this->get_class_identifier() + "\" conditions.");
	}

	virtual std::string get_less_than_string() const
	{
		throw std::runtime_error("The less than operator is not supported for \"" + this->get_class_identifier() + "\" conditions.");
	}

	virtual std::string get_less_than_or_equality_string() const
	{
		throw std::runtime_error("The less than or equality operator is not supported for \"" + this->get_class_identifier() + "\" conditions.");
	}

	virtual std::string get_greater_than_string() const
	{
		throw std::runtime_error("The greater than operator is not supported for \"" + this->get_class_identifier() + "\" conditions.");
	}

	virtual std::string get_greater_than_or_equality_string() const
	{
		throw std::runtime_error("The greater than or equality operator is not supported for \"" + this->get_class_identifier() + "\" conditions.");
	}

	virtual bool is_hidden() const
	{
		return false;
	}

private:
	gsml_operator condition_operator;
};

}
