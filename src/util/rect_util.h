#pragma once

#include "util/point_container.h"

namespace archimedes::rect {

extern int distance_to(const QRect &rect, const QRect &other_rect);

inline int distance_to(const QRect &rect, const QPoint &point)
{
	return rect::distance_to(rect, QRect(point, QSize(1, 1)));
}

inline bool is_adjacent_to(const QRect &rect, const QRect &other_rect)
{
	return distance_to(rect, other_rect) <= 1;
}

template <typename function_type>
inline void for_each_point(const QRect &rect, const function_type &function)
{
	const QPoint min_pos = rect.topLeft();
	const QPoint max_pos = rect.bottomRight();

	for (int x = min_pos.x(); x <= max_pos.x(); ++x) {
		for (int y = min_pos.y(); y <= max_pos.y(); ++y) {
			function(QPoint(x, y));
		}
	}
}

template <typename function_type>
inline void for_each_point_until(const QRect &rect, const function_type &function)
{
	const QPoint min_pos = rect.topLeft();
	const QPoint max_pos = rect.bottomRight();

	for (int x = min_pos.x(); x <= max_pos.x(); ++x) {
		for (int y = min_pos.y(); y <= max_pos.y(); ++y) {
			if (function(QPoint(x, y)) == true) {
				return;
			}
		}
	}
}

template <typename function_type>
inline void for_each_edge_point(const QRect &rect, const function_type &function)
{
	const QPoint min_pos = rect.topLeft();
	const QPoint max_pos = rect.bottomRight();

	for (int x = min_pos.x(); x <= max_pos.x(); ++x) {
		function(QPoint(x, min_pos.y()));
		function(QPoint(x, max_pos.y()));
	}

	for (int y = min_pos.y() + 1; y < max_pos.y(); ++y) {
		function(QPoint(min_pos.x(), y));
		function(QPoint(max_pos.x(), y));
	}
}

template <typename function_type>
inline void for_each_edge_point_until(const QRect &rect, const function_type &function)
{
	const QPoint min_pos = rect.topLeft();
	const QPoint max_pos = rect.bottomRight();

	for (int x = min_pos.x(); x <= max_pos.x(); ++x) {
		if (function(QPoint(x, min_pos.y())) == true) {
			return;
		}

		if (function(QPoint(x, max_pos.y())) == true) {
			return;
		}
	}

	for (int y = min_pos.y() + 1; y < max_pos.y(); ++y) {
		if (function(QPoint(min_pos.x(), y)) == true) {
			return;
		}

		if (function(QPoint(max_pos.x(), y)) == true) {
			return;
		}
	}
}

template <typename function_type>
inline std::vector<QPoint> find_points_if(const QRect &rect, const function_type &function)
{
	std::vector<QPoint> points;

	rect::for_each_point(rect, [&](QPoint &&point) {
		if (function(point)) {
			points.push_back(std::move(point));
		}
	});

	return points;
}

template <typename function_type>
inline point_set find_point_set_if(const QRect &rect, const function_type &function)
{
	point_set points;

	rect::for_each_point(rect, [&](QPoint &&point) {
		if (function(point)) {
			points.insert(std::move(point));
		}
	});

	return points;
}

}
