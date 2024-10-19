#pragma once

#include "script/condition/condition_base.h"

namespace archimedes {

template <typename scope_type, typename context_type, typename main_condition_type>
class and_condition_base : public condition_base<scope_type, context_type>
{
public:
	and_condition_base() : condition_base<scope_type, context_type>(gsml_operator::assignment)
	{
	}

	explicit and_condition_base(const gsml_operator condition_operator)
		: condition_base<scope_type, context_type>(condition_operator)
	{
	}

	explicit and_condition_base(std::vector<std::unique_ptr<const condition_base<scope_type, context_type>>> &&conditions)
		: condition_base<scope_type, context_type>(gsml_operator::assignment),
		conditions(std::move(conditions))
	{
	}

	virtual const std::string &get_class_identifier() const override
	{
		static const std::string class_identifier = "and";
		return class_identifier;
	}

	virtual void process_gsml_property(const gsml_property &property) override
	{
		this->conditions.push_back(main_condition_type::from_gsml_property(property));
	}

	virtual void process_gsml_scope(const gsml_data &scope) override
	{
		this->conditions.push_back(main_condition_type::from_gsml_scope(scope));
	}

	virtual void check_validity() const override
	{
		for (const auto &condition : this->conditions) {
			condition->check_validity();
		}
	}

	template <typename checked_scope_type>
	bool check_internal(const checked_scope_type scope) const
	{
		for (const auto &condition : this->conditions) {
			if (!condition->check(scope)) {
				return false;
			}
		}

		return true;
	}

	template <typename checked_scope_type>
	bool check_internal(const checked_scope_type scope, const context_type &ctx) const
	{
		for (const auto &condition : this->conditions) {
			if (!condition->check(scope, ctx)) {
				return false;
			}
		}

		return true;
	}

	bool check(const scope_type *scope, const context_type &ctx) const
	{
		return condition_base<scope_type, context_type>::check(scope, ctx);
	}

	bool check(const scope_type *scope) const
	{
		return this->check(scope, context_type(scope));
	}

	virtual bool check_assignment(const scope_type *scope, const context_type &ctx) const override
	{
		return this->check_internal(scope, ctx);
	}

	virtual std::string get_assignment_string(const size_t indent) const override
	{
		if (this->conditions.empty()) {
			return std::string();
		}

		if (this->conditions.size() == 1) {
			return this->conditions.front()->get_string(indent);
		}

		std::string str = "All of:\n";
		str += this->get_conditions_string(indent + 1);
		return str;
	}

	std::string get_conditions_string(const size_t indent) const
	{
		return condition_base<scope_type, context_type>::get_conditions_string(this->conditions, indent);
	}

	void add_condition(std::unique_ptr<const condition_base<scope_type, context_type>> &&condition)
	{
		this->conditions.push_back(std::move(condition));
	}

private:
	std::vector<std::unique_ptr<const condition_base<scope_type, context_type>>> conditions; //the conditions of which all should be true
};

}
