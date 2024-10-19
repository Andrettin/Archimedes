#pragma once

#include "script/condition/scope_condition_base.h"

namespace archimedes {

template <typename upper_scope_type, typename scope_type, typename context_type, typename main_condition_type>
class scope_condition : public scope_condition_base<upper_scope_type, scope_type, context_type, main_condition_type>
{
public:
	explicit scope_condition(const gsml_operator condition_operator)
		: scope_condition_base<upper_scope_type, scope_type, context_type, main_condition_type>(condition_operator)
	{
	}

	virtual const scope_type *get_scope(const upper_scope_type *upper_scope, const context_type &ctx) const = 0;

	virtual bool check_assignment(const upper_scope_type *upper_scope, const context_type &ctx) const override final
	{
		const scope_type *scope = this->get_scope(upper_scope, ctx);

		if (scope == nullptr) {
			return false;
		}

		return this->check_scope(scope, ctx);
	}
};

}
