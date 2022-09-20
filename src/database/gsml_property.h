#pragma once

namespace archimedes {

enum class gsml_operator;

class gsml_property final
{
public:
	explicit gsml_property(std::string &&key, const gsml_operator property_operator, std::string &&value)
		: key(std::move(key)), property_operator(property_operator), value(std::move(value))
	{
	}

	explicit gsml_property(const std::string &key, const gsml_operator property_operator, const std::string &value)
		: key(key), property_operator(property_operator), value(value)
	{
	}

	const std::string &get_key() const
	{
		return this->key;
	}

	gsml_operator get_operator() const
	{
		return this->property_operator;
	}

	const std::string &get_value() const
	{
		return this->value;
	}

	void print(std::ostream &ostream, const size_t indentation) const;

private:
	std::string key;
	gsml_operator property_operator;
	std::string value;
};

}
