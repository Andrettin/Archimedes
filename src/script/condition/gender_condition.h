#pragma once

#include "character/character_base.h"
#include "script/condition/condition_base.h"
#include "util/gender.h"

namespace archimedes {

template <typename scope_type, typename context_type>
class gender_condition final : public condition_base<scope_type, context_type>
{
public:
	explicit gender_condition(const std::string &value, const gsml_operator condition_operator)
		: condition_base<scope_type, context_type>(condition_operator)
	{
		this->gender = enum_converter<archimedes::gender>::to_enum(value);
	}

	virtual const std::string &get_class_identifier() const override
	{
		static const std::string class_identifier = "gender";
		return class_identifier;
	}

	virtual bool check_assignment(const scope_type *scope, const context_type &ctx) const override
	{
		Q_UNUSED(ctx);

		return scope->get_gender() == this->gender;
	}

	virtual std::string get_assignment_string(const size_t indent) const override
	{
		Q_UNUSED(indent);

		return get_gender_name(this->gender);
	}

private:
	archimedes::gender gender = gender::none;
};

}
