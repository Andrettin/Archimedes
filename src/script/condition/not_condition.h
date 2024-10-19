#pragma once

#include "script/condition/condition_base.h"

namespace archimedes {

template <typename scope_type, typename context_type>
class not_condition final : public condition_base<scope_type, context_type>
{
public:
	explicit not_condition(const gsml_operator condition_operator)
		: condition_base<scope_type, context_type>(condition_operator)
	{
	}

	explicit not_condition(std::vector<std::unique_ptr<const condition_base<scope_type, context_type>>> &&conditions)
		: condition_base<scope_type, context_type>(gsml_operator::assignment),
		conditions(std::move(conditions))
	{
	}

	explicit not_condition(std::unique_ptr<const condition_base<scope_type, context_type>> &&condition)
		: condition_base<scope_type, context_type>(gsml_operator::assignment)
	{
		this->conditions.push_back(std::move(condition));
	}

	virtual const std::string &get_class_identifier() const override
	{
		static const std::string class_identifier = "not";
		return class_identifier;
	}

	virtual void process_gsml_property(const gsml_property &property) override
	{
		this->conditions.push_back(condition_base<scope_type, context_type>::from_gsml_property(property));
	}

	virtual void process_gsml_scope(const gsml_data &scope) override
	{
		this->conditions.push_back(condition_base<scope_type, context_type>::from_gsml_scope(scope));
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
			if (condition->check(scope)) {
				return false;
			}
		}

		return true;
	}

	template <typename checked_scope_type>
	bool check_internal(const checked_scope_type scope, const context_type &ctx) const
	{
		for (const auto &condition : this->conditions) {
			if (condition->check(scope, ctx)) {
				return false;
			}
		}

		return true;
	}

	virtual bool check_assignment(const scope_type *scope, const context_type &ctx) const override
	{
		return this->check_internal(scope, ctx);
	}

	virtual std::string get_assignment_string(const size_t indent) const override
	{
		std::string str = "None of:\n";
		str += condition_base<scope_type, context_type>::get_conditions_string(this->conditions, indent + 1);
		return str;
	}

private:
	std::vector<std::unique_ptr<const condition_base<scope_type, context_type>>> conditions; //the conditions of which none should be true
};

}
