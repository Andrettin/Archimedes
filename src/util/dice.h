#pragma once

namespace archimedes {

class dice final
{
public:
	constexpr dice()
	{
	}

	explicit constexpr dice(const int count, const int sides) : count(count), sides(sides)
	{
	}

	constexpr dice(const dice &rhs) : dice(rhs.count, rhs.sides)
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

	constexpr int get_count() const
	{
		return this->count;
	}

	constexpr int get_sides() const
	{
		return this->sides;
	}

	constexpr int get_maximum_result() const
	{
		return this->get_count() * this->get_sides();
	}

	constexpr bool is_null() const
	{
		return this->get_count() == 0 && this->get_sides() == 0;
	}

	std::string to_string() const
	{
		return std::format("{}d{}", this->get_count(), this->get_sides());
	}

	constexpr bool operator ==(const dice &rhs) const
	{
		return this->get_count() == rhs.get_count() && this->get_sides() == rhs.get_sides();
	}

	constexpr bool operator !=(const dice &rhs) const = default;

private:
	int count = 0;
	int sides = 0;
};

}

Q_DECLARE_METATYPE(archimedes::dice)
