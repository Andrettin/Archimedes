#pragma once

#include "script/condition/and_condition_base.h"

namespace archimedes {

//the class for a predefined, reusable scripted condition
template <typename scope_type, typename context_type, typename main_condition_type>
class scripted_condition_base
{
public:
	explicit scripted_condition_base()
	{
		this->conditions = std::make_unique<and_condition_base<scope_type, context_type, main_condition_type>>();
	}

	void process_gsml_property(const gsml_property &property)
	{
		this->conditions->process_gsml_property(property);
	}

	void process_gsml_scope(const gsml_data &scope)
	{
		this->conditions->process_gsml_scope(scope);
	}

	void check() const
	{
		this->get_conditions()->check_validity();
	}

	const and_condition_base<scope_type, context_type, main_condition_type> *get_conditions() const
	{
		return this->conditions.get();
	}

private:
	std::unique_ptr<and_condition_base<scope_type, context_type, main_condition_type>> conditions;
};

}
