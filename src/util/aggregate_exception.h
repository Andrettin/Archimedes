#pragma once

namespace archimedes {

class aggregate_exception final : public std::exception
{
public:
	explicit aggregate_exception(std::string &&message, std::vector<std::exception_ptr> &&exceptions)
		: message(std::move(message)), exceptions(std::move(exceptions))
	{
	}

	virtual const char *what() const override
	{
		return this->message.c_str();
	}

	const std::vector<std::exception_ptr> &get_exceptions() const
	{
		return this->exceptions;
	}

private:
	std::string message;
	std::vector<std::exception_ptr> exceptions;
};

}
