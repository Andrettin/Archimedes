#pragma once

namespace archimedes::size {

constexpr QSize empty_size = QSize(0, 0);

inline constexpr QPoint to_point(const QSize &size)
{
	return QPoint(size.width(), size.height());
}

inline std::string to_string(const QSize &size)
{
	return "(" + std::to_string(size.width()) + ", " + std::to_string(size.height()) + ")";
}

}
