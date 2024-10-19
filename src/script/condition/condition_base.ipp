#pragma once

#include "database/database.h"
#include "script/condition/and_condition_base.h"
#include "script/condition/not_condition.h"
#include "script/condition/or_condition.h"
#include "script/condition/tooltip_condition.h"
#include "util/string_util.h"

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

	database::process_gsml_data(condition, scope);

	return condition;
}

}
