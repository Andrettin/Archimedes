#pragma once

namespace archimedes {

class gsml_data;
class gsml_property;

//a visitor that only does an action when visiting GSML properties
template <typename function_type>
class gsml_property_visitor
{
public:
	explicit gsml_property_visitor(const function_type &function) : function(function)
	{
	}

	explicit gsml_property_visitor(function_type &&function) = delete;

	void operator()(const gsml_property &property) const
	{
		this->function(property);
	}

	void operator()(const gsml_data &scope) const
	{
		Q_UNUSED(scope)
	}

private:
	const function_type &function;
};

}
