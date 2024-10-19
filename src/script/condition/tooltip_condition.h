#pragma once

#include "script/condition/and_condition_base.h"
#include "script/condition/condition_base.h"

namespace archimedes {

template <typename scope_type, typename context_type, typename main_condition_type>
class tooltip_condition final : public condition_base<scope_type, context_type>
{
public:
	explicit tooltip_condition(const gsml_operator condition_operator)
		: condition_base<scope_type, context_type>(condition_operator)
	{
	}

	virtual void process_gsml_property(const gsml_property &property) override
	{
		if (property.get_key() == "text") {
			this->text = property.get_value();
		} else {
			this->conditions.process_gsml_property(property);
		}
	}

	virtual void process_gsml_scope(const gsml_data &scope) override
	{
		this->conditions.process_gsml_scope(scope);
	}

	virtual const std::string &get_class_identifier() const override
	{
		static const std::string class_identifier = "tooltip";
		return class_identifier;
	}

	virtual bool check_assignment(const scope_type *scope, const context_type &ctx) const override
	{
		return this->conditions.check_assignment(scope, ctx);
	}

	virtual std::string get_assignment_string(const size_t indent) const override
	{
		Q_UNUSED(indent);

		return this->text;
	}

private:
	std::string text;
	and_condition_base<scope_type, context_type, main_condition_type> conditions;
};

}
