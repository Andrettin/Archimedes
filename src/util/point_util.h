#pragma once

namespace archimedes::point {

inline constexpr int to_index(const int x, const int y, const int width)
{
	return x + y * width;
}

inline constexpr int to_index(const int x, const int y, const QSize &size)
{
	return point::to_index(x, y, size.width());
}

inline constexpr int to_index(const QPoint &point, const int width)
{
	return point::to_index(point.x(), point.y(), width);
}

inline constexpr int to_index(const QPoint &point, const QSize &size)
{
	return point::to_index(point.x(), point.y(), size);
}

inline constexpr QPoint from_index(const int index, const int width)
{
	return QPoint(index % width, index / width);
}

inline constexpr QPoint from_index(const int index, const QSize &size)
{
	return point::from_index(index, size.width());
}

inline constexpr QSize to_size(const QPoint &point)
{
	return QSize(point.x(), point.y());
}

inline void clamp(QPoint &point, const QPoint &min, const QPoint &max)
{
	if (point.x() < min.x()) {
		point.setX(min.x());
	}

	if (point.y() < min.y()) {
		point.setY(min.y());
	}

	if (point.x() > max.x()) {
		point.setX(max.x());
	}

	if (point.y() > max.y()) {
		point.setY(max.y());
	}
}

extern int square_distance_to(const QPoint &point, const QPoint &other_point);
extern int distance_to(const QPoint &point, const QPoint &other_point);
extern int distance_to(const QPoint &point, const QRect &rect);

inline bool is_horizontally_adjacent_to(const QPoint &point, const QPoint &other_point)
{
	if (point.y() != other_point.y()) {
		return false;
	}

	const int diff = std::abs(point.x() - other_point.x());
	return diff == 1;
}

inline bool is_vertically_adjacent_to(const QPoint &point, const QPoint &other_point)
{
	if (point.x() != other_point.x()) {
		return false;
	}

	const int diff = std::abs(point.y() - other_point.y());
	return diff == 1;
}

inline bool is_cardinally_adjacent_to(const QPoint &point, const QPoint &other_point)
{
	//returns whether the point is adjacent to the other one in any of the four cardinal directions
	return point::is_horizontally_adjacent_to(point, other_point) || point::is_vertically_adjacent_to(point, other_point);
}

inline QPoint get_circle_point(const QPoint &point, const int64_t source_radius, const int64_t target_radius)
{
	if (source_radius == 0) {
		throw std::runtime_error("Source radius is zero.");
	}

	const int64_t x = point.x() * target_radius / source_radius;
	const int64_t y = point.y() * target_radius / source_radius;
	return QPoint(static_cast<int>(x), static_cast<int>(y));
}

inline QPoint get_nearest_circle_edge_point(const QPoint &point, const int64_t radius)
{
	return point::get_circle_point(point, point::distance_to(point, QPoint(0, 0)), radius);
}

template <typename function_type>
inline void for_each_adjacent(const QPoint &point, const function_type &function)
{
	for (int x_offset = -1; x_offset <= 1; ++x_offset) {
		for (int y_offset = -1; y_offset <= 1; ++y_offset) {
			if (x_offset == 0 && y_offset == 0) {
				continue;
			}

			function(QPoint(point.x() + x_offset, point.y() + y_offset));
		}
	}
}

template <typename function_type>
inline void for_each_adjacent_until(const QPoint &point, const function_type &function)
{
	for (int x_offset = -1; x_offset <= 1; ++x_offset) {
		for (int y_offset = -1; y_offset <= 1; ++y_offset) {
			if (x_offset == 0 && y_offset == 0) {
				continue;
			}

			const QPoint adjacent_point(point.x() + x_offset, point.y() + y_offset);
			if (function(adjacent_point)) {
				return;
			}
		}
	}
}

template <typename function_type>
inline void for_each_cardinally_adjacent(const QPoint &point, const function_type &function)
{
	function(QPoint(point.x() - 1, point.y()));
	function(QPoint(point.x() + 1, point.y()));
	function(QPoint(point.x(), point.y() - 1));
	function(QPoint(point.x(), point.y() + 1));
}

template <typename function_type>
inline void for_each_cardinally_adjacent_until(const QPoint &point, const function_type &function)
{
	if (function(QPoint(point.x() - 1, point.y()))) {
		return;
	}

	if (function(QPoint(point.x() + 1, point.y()))) {
		return;
	}

	if (function(QPoint(point.x(), point.y() - 1))) {
		return;
	}

	if (function(QPoint(point.x(), point.y() + 1))) {
		return;
	}
}

template <typename function_type>
inline void for_each_diagonally_adjacent(const QPoint &point, const function_type &function)
{
	function(QPoint(point.x() - 1, point.y() - 1));
	function(QPoint(point.x() + 1, point.y() - 1));
	function(QPoint(point.x() - 1, point.y() + 1));
	function(QPoint(point.x() + 1, point.y() + 1));
}

template <typename function_type>
inline void for_each_diagonally_adjacent_until(const QPoint &point, const function_type &function)
{
	if (function(QPoint(point.x() - 1, point.y() - 1))) {
		return;
	}

	if (function(QPoint(point.x() + 1, point.y() - 1))) {
		return;
	}

	if (function(QPoint(point.x() - 1, point.y() + 1))) {
		return;
	}

	if (function(QPoint(point.x() + 1, point.y() + 1))) {
		return;
	}
}

template <typename function_type>
inline void for_each_offset_pos_until(const QPoint &point, const std::vector<QPoint> &offsets, const function_type &function)
{
	for (const QPoint &offset : offsets) {
		if (function(point + offset)) {
			return;
		}
	}
}

template <typename function_type>
inline std::optional<QPoint> find_adjacent_if(const QPoint &point, const function_type &function)
{
	std::optional<QPoint> result;

	point::for_each_adjacent_until(point, [&](const QPoint &adjacent_point) {
		if (function(adjacent_point)) {
			result = adjacent_point;
			return true;
		}

		return false;
	});

	return result;
}

template <typename function_type>
inline std::vector<QPoint> get_adjacent_if(const QPoint &point, const function_type &function)
{
	std::vector<QPoint> adjacent_points;

	point::for_each_adjacent(point, [&](QPoint &&adjacent_point) {
		if (function(adjacent_point)) {
			adjacent_points.push_back(std::move(adjacent_point));
		}
	});

	return adjacent_points;
}

template <typename function_type>
inline std::vector<QPoint> get_diagonally_adjacent_if(const QPoint &point, const function_type &function)
{
	std::vector<QPoint> adjacent_points;

	for (int x_offset = -1; x_offset <= 1; x_offset += 2) { // +2 so that only diagonals are used
		for (int y_offset = -1; y_offset <= 1; y_offset += 2) {
			QPoint adjacent_point = point + QPoint(x_offset, y_offset);
			const QPoint &adjacent_point_ref = adjacent_point;
			if (function(adjacent_point_ref)) {
				adjacent_points.push_back(std::move(adjacent_point));
			}
		}
	}

	return adjacent_points;
}

inline std::vector<QPoint> get_straight_path_to(const QPoint &point, const QPoint &other_point)
{
	std::vector<QPoint> path;

	path.push_back(point);

	if (point::is_cardinally_adjacent_to(point, other_point)) {
		path.push_back(other_point);
		return path;
	}

	int horizontal_move_count = 0;
	int vertical_move_count = 0;
	const int horizontal_diff = std::abs(point.x() - other_point.x());
	const int vertical_diff = std::abs(point.y() - other_point.y());

	QPoint current_point = point;
	while (current_point != other_point) {
		const int horizontal_progress = horizontal_diff != 0 ? (horizontal_move_count * 100 / horizontal_diff) : 100;
		const int vertical_progress = vertical_diff != 0 ? (vertical_move_count * 100 / vertical_diff) : 100;
		if (current_point.x() != other_point.x() && horizontal_progress <= vertical_progress) {
			if (other_point.x() < current_point.x()) {
				current_point.setX(current_point.x() - 1);
			} else {
				current_point.setX(current_point.x() + 1);
			}
			horizontal_move_count++;
		} else if (current_point.y() != other_point.y()) {
			if (other_point.y() < current_point.y()) {
				current_point.setY(current_point.y() - 1);
			} else {
				current_point.setY(current_point.y() + 1);
			}
			vertical_move_count++;
		}

		path.push_back(current_point);
	}

	path.push_back(other_point);

	return path;
}

inline std::string to_string(const QPoint &point)
{
	return "(" + std::to_string(point.x()) + ", " + std::to_string(point.y()) + ")";
}

}
