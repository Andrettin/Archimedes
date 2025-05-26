#pragma once

#include "database/defines_base.h"
#include "database/gsml_data.h"
#include "database/gsml_property.h"
#include "util/fractional_int.h"
#include "util/number_util.h"
#include "util/string_util.h"

namespace archimedes {

template <typename scope_type>
class modifier_effect_base
{
public:
	modifier_effect_base()
	{
	}

	explicit modifier_effect_base(const std::string &value) : value(centesimal_int(value))
	{
	}

	virtual ~modifier_effect_base()
	{
	}

	virtual const std::string &get_identifier() const = 0;

	virtual void process_gsml_property(const gsml_property &property)
	{
		throw std::runtime_error(std::format("Invalid property for \"{}\" effect: \"{}\".", this->get_identifier(), property.get_key()));
	}

	virtual void process_gsml_scope(const gsml_data &scope)
	{
		throw std::runtime_error(std::format("Invalid scope for \"{}\" effect: \"{}\".", this->get_identifier(), scope.get_tag()));
	}

	centesimal_int get_multiplied_value(const centesimal_int &multiplier) const
	{
		return this->value * multiplier;
	}

	virtual void apply(scope_type *scope, const centesimal_int &multiplier) const = 0;

	virtual std::string get_base_string(const scope_type *scope) const = 0;

	virtual std::string get_string(const scope_type *scope, const centesimal_int &multiplier, const bool ignore_decimals) const
	{
		const centesimal_int value = this->get_multiplied_value(multiplier);
		const std::string number_str = ignore_decimals && !this->are_decimals_relevant() ? number::to_signed_string(value.to_int()) : value.to_signed_string();
		const QColor &number_color = this->is_negative(multiplier) ? defines_base::get()->get_red_text_color() : defines_base::get()->get_green_text_color();
		const std::string colored_number_str = string::colored(number_str + (this->is_percent() ? "%" : ""), number_color);

		return std::format("{}: {}", this->get_base_string(scope), colored_number_str);
	}

	virtual bool is_negative(const centesimal_int &multiplier) const
	{
		return (this->value * multiplier) < 0;
	}

	virtual bool is_percent() const
	{
		return false;
	}

	virtual bool is_hidden() const
	{
		return false;
	}

	virtual bool are_decimals_relevant() const
	{
		return false;
	}

protected:
	centesimal_int value;
};

}
