#pragma once

namespace archimedes {

class dice final
{
public:
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

private:
	int count = 0;
	int sides = 0;
};

}

Q_DECLARE_METATYPE(archimedes::dice)
