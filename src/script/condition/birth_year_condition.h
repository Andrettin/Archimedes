#pragma once

#include "character/character_base.h"
#include "script/condition/numerical_condition.h"

namespace archimedes {

template <typename scope_type, typename context_type>
class birth_year_condition final : public numerical_condition<scope_type, context_type>
{
public:
	explicit birth_year_condition(const std::string &value, const gsml_operator condition_operator)
		: numerical_condition<scope_type, context_type>(value, condition_operator)
	{
	}

	virtual const std::string &get_class_identifier() const override
	{
		static const std::string class_identifier = "birth_year";
		return class_identifier;
	}

	virtual int get_scope_value(const scope_type *scope) const override
	{
		return scope->get_birth_date().year();
	}

	virtual std::string get_value_name() const override
	{
		return "birth year";
	}
};

}
