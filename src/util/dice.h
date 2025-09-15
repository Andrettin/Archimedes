#pragma once

namespace archimedes {

class dice final
{
public:
	constexpr dice()
	{
	}

	explicit constexpr dice(const int count, const int sides, const int modifier = 0)
		: count(count), sides(sides), modifier(modifier)
	{
	}

	constexpr dice(const dice &rhs) : dice(rhs.count, rhs.sides, rhs.modifier)
	{
	}

	explicit dice(const std::string &str)
	{
		const size_t d_pos = str.find('d');

		if (d_pos == std::string::npos) {
			throw std::runtime_error(std::format("Invalid dice string: {}", str));
		}

		this->count = std::stoi(str.substr(0, d_pos));

		std::string remaining_str = str.substr(d_pos + 1, str.length() - d_pos - 1);

		size_t end_pos = remaining_str.length();
		const size_t plus_pos = remaining_str.find('+');
		if (plus_pos != std::string::npos) {
			end_pos = plus_pos;
			this->modifier = std::stoi(remaining_str.substr(end_pos + 1, remaining_str.length() - end_pos - 1));
		} else {
			const size_t minus_pos = remaining_str.find('-');
			if (minus_pos != std::string::npos) {
				end_pos = minus_pos;
				this->modifier = -std::stoi(remaining_str.substr(end_pos + 1, remaining_str.length() - end_pos - 1));
			}
		}

		this->sides = std::stoi(remaining_str.substr(0, end_pos));
	}

	constexpr int get_count() const
	{
		return this->count;
	}

	constexpr int get_sides() const
	{
		return this->sides;
	}

	constexpr int get_modifier() const
	{
		return this->modifier;
	}

	constexpr int get_minimum_result() const
	{
		int min_result = 0;

		if (this->get_count() > 0 && this->get_sides() > 0) {
			min_result += this->get_count();
		}

		min_result += this->get_modifier();

		return min_result;
	}

	constexpr int get_maximum_result() const
	{
		return (this->get_count() * this->get_sides()) + this->get_modifier();
	}

	constexpr bool is_null() const
	{
		return this->get_count() == 0 && this->get_sides() == 0 && this->get_modifier() == 0;
	}

	std::string to_string() const;
	std::string to_display_string() const;

	constexpr bool operator ==(const dice &rhs) const
	{
		return this->get_count() == rhs.get_count() && this->get_sides() == rhs.get_sides() && this->get_modifier() == rhs.get_modifier();
	}

	constexpr bool operator !=(const dice &rhs) const = default;

private:
	int count = 0;
	int sides = 0;
	int modifier = 0;
};

}

Q_DECLARE_METATYPE(archimedes::dice)
