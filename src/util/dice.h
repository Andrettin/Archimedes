#pragma once

namespace archimedes {

class dice final
{
public:
	dice()
	{
	}

	explicit dice(const int count, const int sides) : count(count), sides(sides)
	{
	}

	dice(const dice &rhs) : dice(rhs.count, rhs.sides)
	{
	}

	explicit dice(const std::string &str)
	{
		const size_t d_pos = str.find('d');

		if (d_pos == std::string::npos) {
			throw std::runtime_error(std::format("Invalid dice string: {}", str));
		}

		this->count = std::stoi(str.substr(0, d_pos));
		this->sides = std::stoi(str.substr(d_pos + 1, str.length() - d_pos - 1));
	}

	int get_count() const
	{
		return this->count;
	}

	int get_sides() const
	{
		return this->sides;
	}

	int get_maximum_result() const
	{
		return this->get_count() * this->get_sides();
	}

	bool is_null() const
	{
		return this->get_count() == 0 && this->get_sides() == 0;
	}

	bool operator ==(const dice &rhs) const
	{
		return this->get_count() == rhs.get_count() && this->get_sides() == rhs.get_sides();
	}

	bool operator !=(const dice &rhs) const = default;

private:
	int count = 0;
	int sides = 0;
};

}

Q_DECLARE_METATYPE(archimedes::dice)
