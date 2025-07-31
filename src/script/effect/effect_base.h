#pragma once

#include "database/gsml_data.h"
#include "database/gsml_property.h"
#include "script/context.h"
#include "script/target_variant.h"

namespace archimedes {

class gsml_data;
class gsml_property;
enum class gsml_operator;

//a scripted effect
template <typename scope_type, typename context_type, typename read_only_context_type>
class effect_base
{
public:
	static scope_type *get_target_scope(const target_variant<scope_type> &target, const context_type &ctx)
	{
		if (std::holds_alternative<scope_type *>(target)) {
			return std::get<scope_type *>(target);
		} else if (std::holds_alternative<special_target_type>(target)) {
			return ctx.get_special_target_scope<scope_type>(std::get<special_target_type>(target));
		}

		return nullptr;
	}

	static const scope_type *get_target_scope(const target_variant<scope_type> &target, const read_only_context_type &ctx)
	{
		if (std::holds_alternative<scope_type *>(target)) {
			return std::get<scope_type *>(target);
		} else if (std::holds_alternative<special_target_type>(target)) {
			return ctx.get_special_target_scope<const scope_type>(std::get<special_target_type>(target));
		}

		return nullptr;
	}

	explicit effect_base(const gsml_operator effect_operator) : effect_operator(effect_operator)
	{
	}

	virtual ~effect_base()
	{
	}

	virtual const std::string &get_class_identifier() const = 0;

	virtual void process_gsml_property(const gsml_property &property)
	{
		throw std::runtime_error("Invalid property for \"" + this->get_class_identifier() + "\" effect: \"" + property.get_key() + "\".");
	}

	virtual void process_gsml_scope(const gsml_data &scope)
	{
		throw std::runtime_error("Invalid scope for \"" + this->get_class_identifier() + "\" effect: \"" + scope.get_tag() + "\".");
	}

	virtual void check() const
	{
	}

	void do_effect(scope_type *scope, context_type &ctx) const
	{
		try {
			switch (this->effect_operator) {
				case gsml_operator::assignment:
					this->do_assignment_effect(scope, ctx);
					break;
				case gsml_operator::addition:
					this->do_addition_effect(scope, ctx);
					break;
				case gsml_operator::subtraction:
					this->do_subtraction_effect(scope, ctx);
					break;
				default:
					throw std::runtime_error("Invalid effect operator: \"" + std::to_string(static_cast<int>(this->effect_operator)) + "\".");
			}
		} catch (...) {
			std::throw_with_nested(std::runtime_error("Failed to do \"" + this->get_class_identifier() + "\" effect."));
		}
	}

	virtual void do_assignment_effect(scope_type *scope) const
	{
		Q_UNUSED(scope);

		throw std::runtime_error("The assignment operator is not supported for \"" + this->get_class_identifier() + "\" effects.");
	}

	virtual void do_assignment_effect(scope_type *scope, context_type &ctx) const
	{
		Q_UNUSED(ctx);

		this->do_assignment_effect(scope);
	}

	virtual void do_addition_effect(scope_type *scope) const
	{
		Q_UNUSED(scope)

		throw std::runtime_error("The addition operator is not supported for \"" + this->get_class_identifier() + "\" effects.");
	}

	virtual void do_addition_effect(scope_type *scope, context_type &ctx) const
	{
		Q_UNUSED(ctx);

		this->do_addition_effect(scope);
	}

	virtual void do_subtraction_effect(scope_type *scope) const
	{
		Q_UNUSED(scope)

		throw std::runtime_error("The subtraction operator is not supported for \"" + this->get_class_identifier() + "\" effects.");
	}

	virtual void do_subtraction_effect(scope_type *scope, context_type &ctx) const
	{
		Q_UNUSED(ctx);

		this->do_subtraction_effect(scope);
	}

	std::string get_string(const scope_type *scope, const read_only_context_type &ctx, const size_t indent, const std::string &prefix) const
	{
		switch (this->effect_operator) {
			case gsml_operator::assignment:
				return this->get_assignment_string(scope, ctx, indent, prefix);
			case gsml_operator::addition:
				return this->get_addition_string(scope, ctx, indent);
			case gsml_operator::subtraction:
				return this->get_subtraction_string(scope, ctx);
			default:
				throw std::runtime_error("Invalid effect operator: \"" + std::to_string(static_cast<int>(this->effect_operator)) + "\".");
		}
	}

	virtual std::string get_assignment_string() const
	{
		throw std::runtime_error("The assignment operator is not supported for \"" + this->get_class_identifier() + "\" effects.");
	}

	virtual std::string get_assignment_string(const scope_type *scope, const read_only_context_type &ctx, const size_t indent, const std::string &prefix) const
	{
		Q_UNUSED(scope);
		Q_UNUSED(ctx);
		Q_UNUSED(indent);
		Q_UNUSED(prefix);

		return this->get_assignment_string();
	}

	virtual std::string get_addition_string() const
	{
		throw std::runtime_error("The addition operator is not supported for \"" + this->get_class_identifier() + "\" effects.");
	}

	virtual std::string get_addition_string(const scope_type *scope, const read_only_context_type &ctx, const size_t indent) const
	{
		Q_UNUSED(scope);
		Q_UNUSED(ctx);
		Q_UNUSED(indent);

		return this->get_addition_string();
	}

	virtual std::string get_subtraction_string() const
	{
		throw std::runtime_error("The subtraction operator is not supported for \"" + this->get_class_identifier() + "\" effects.");
	}

	virtual std::string get_subtraction_string(const scope_type *scope, const read_only_context_type &ctx) const
	{
		Q_UNUSED(scope);
		Q_UNUSED(ctx);

		return this->get_subtraction_string();
	}

	virtual bool is_hidden() const
	{
		return false;
	}

	gsml_operator get_operator() const
	{
		return this->effect_operator;
	}

private:
	gsml_operator effect_operator;
};

}
