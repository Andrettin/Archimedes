#pragma once

namespace archimedes {

enum class direction {
	north,
	northeast,
	east,
	southeast,
	south,
	southwest,
	west,
	northwest,

	count
};

enum direction_flag : uint8_t {
	none = 0,
	north = 1 << 0,
	south = 1 << 1,
	west = 1 << 2,
	east = 1 << 3,
	northwest = 1 << 4,
	northeast = 1 << 5,
	southwest = 1 << 6,
	southeast = 1 << 7
};

inline direction string_to_direction(const std::string &str)
{
	if (str == "north") {
		return direction::north;
	} else if (str == "northeast") {
		return direction::northeast;
	} else if (str == "east") {
		return direction::east;
	} else if (str == "southeast") {
		return direction::southeast;
	} else if (str == "south") {
		return direction::south;
	} else if (str == "southwest") {
		return direction::southwest;
	} else if (str == "west") {
		return direction::west;
	} else if (str == "northwest") {
		return direction::northwest;
	}

	throw std::runtime_error("Invalid direction: \"" + str + "\".");
}

inline std::string direction_to_string(const direction direction)
{
	switch (direction) {
		case direction::north:
			return "north";
		case direction::northeast:
			return "northeast";
		case direction::east:
			return "east";
		case direction::southeast:
			return "southeast";
		case direction::south:
			return "south";
		case direction::southwest:
			return "southwest";
		case direction::west:
			return "west";
		case direction::northwest:
			return "northwest";
		default:
			break;
	}

	throw std::runtime_error("Invalid direction: \"" + std::to_string(static_cast<int>(direction)) + "\".");
}

inline QPoint direction_to_offset(const direction direction)
{
	switch (direction) {
		case direction::north:
			return QPoint(0, -1);
		case direction::northeast:
			return QPoint(1, -1);
		case direction::east:
			return QPoint(1, 0);
		case direction::southeast:
			return QPoint(1, 1);
		case direction::south:
			return QPoint(0, 1);
		case direction::southwest:
			return QPoint(-1, 1);
		case direction::west:
			return QPoint(-1, 0);
		case direction::northwest:
			return QPoint(-1, -1);
		default:
			break;
	}

	throw std::runtime_error("Invalid direction: \"" + std::to_string(static_cast<int>(direction)) + "\".");
}

extern direction offset_to_direction(const QPoint &offset);

inline direction_flag direction_to_flag(const direction direction)
{
	switch (direction) {
		case direction::north:
			return direction_flag::north;
		case direction::northeast:
			return direction_flag::northeast;
		case direction::east:
			return direction_flag::east;
		case direction::southeast:
			return direction_flag::southeast;
		case direction::south:
			return direction_flag::south;
		case direction::southwest:
			return direction_flag::southwest;
		case direction::west:
			return direction_flag::west;
		case direction::northwest:
			return direction_flag::northwest;
		default:
			break;
	}

	throw std::runtime_error("Invalid direction: \"" + std::to_string(static_cast<int>(direction)) + "\".");
}

}

Q_DECLARE_METATYPE(archimedes::direction)
