#pragma once

#include "database/gsml_operator.h"
#include "script/condition/and_condition_base.h"
#include "script/condition/condition_base.h"

namespace archimedes {

template <typename upper_scope_type, typename scope_type, typename context_type>
class scope_condition_base : public condition_base<upper_scope_type, context_type>
{
public:
	explicit scope_condition_base(const gsml_operator condition_operator)
		: condition_base<upper_scope_type, context_type>(condition_operator)
	{
	}

	virtual void process_gsml_property(const gsml_property &property) override
	{
		this->conditions.process_gsml_property(property);
	}

	virtual void process_gsml_scope(const gsml_data &scope) override final
	{
		this->conditions.process_gsml_scope(scope);
	}

	virtual void check_validity() const override
	{
		this->conditions.check_validity();
	}

	bool check_scope(const scope_type *scope, const context_type &ctx) const
	{
		return this->conditions.check(scope, ctx);
	}

	virtual std::string get_scope_name() const = 0;

	virtual std::string get_assignment_string(const size_t indent) const override final
	{
		std::string str = this->get_scope_name() + ":\n";
		str += this->conditions.get_conditions_string(indent + 1);
		return str;
	}

private:
	and_condition_base<scope_type, context_type> conditions;
};

}
