#pragma once

#include "script/condition/and_condition_base.h"
#include "script/condition/birth_year_condition.h"
#include "script/condition/gender_condition.h"
#include "script/condition/not_condition.h"
#include "script/condition/or_condition.h"
#include "script/condition/tooltip_condition.h"
#include "util/string_util.h"
#include "util/type_traits.h"

namespace archimedes {

template <typename scope_type, typename context_type>
std::string condition_base<scope_type, context_type>::get_object_highlighted_name(const named_data_entry *object, const std::string &name_string)
{
	if (!name_string.empty()) {
		return string::highlight(name_string);
	} else {
		return string::highlight(object->get_name());
	}
}

template <typename scope_type, typename context_type>
std::unique_ptr<const condition_base<scope_type, context_type>> condition_base<scope_type, context_type>::from_gsml_property(const gsml_property &property)
{
	const std::string &key = property.get_key();
	const gsml_operator condition_operator = property.get_operator();
	const std::string &value = property.get_value();

	if constexpr (std::is_base_of_v<character_base, scope_type>) {
		if (key == "birth_year") {
			return std::make_unique<birth_year_condition<scope_type, context_type>>(value, condition_operator);
		} else if (key == "gender") {
			return std::make_unique<gender_condition<scope_type, context_type>>(value, condition_operator);
		}
	}

	throw std::runtime_error(std::format("Invalid condition property: \"{}\".", key));
}

template <typename scope_type, typename context_type>
template <typename main_condition_type>
std::unique_ptr<const condition_base<scope_type, context_type>> condition_base<scope_type, context_type>::from_gsml_scope(const gsml_data &scope)
{
	const std::string &tag = scope.get_tag();
	const gsml_operator condition_operator = scope.get_operator();
	std::unique_ptr<condition_base<scope_type, context_type>> condition;

	if (tag == "and") {
		condition = std::make_unique<and_condition_base<scope_type, context_type, main_condition_type>>(condition_operator);
	} else if (tag == "or") {
		condition = std::make_unique<or_condition<scope_type, context_type, main_condition_type>>(condition_operator);
	} else if (tag == "not") {
		condition = std::make_unique<not_condition<scope_type, context_type, main_condition_type>>(condition_operator);
	} else if (tag == "tooltip") {
		condition = std::make_unique<tooltip_condition<scope_type, context_type, main_condition_type>>(condition_operator);
	}

	if (condition == nullptr) {
		throw std::runtime_error(std::format("Invalid condition scope: \"{}\".", tag));
	}

	condition->process_gsml_data(scope);

	return condition;
}

}
