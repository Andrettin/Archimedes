#include "archimedes.h"

#include "map/map_projection.h"

#include "map/equirectangular_map_projection.h"
#include "map/mercator_map_projection.h"

#include "util/geocoordinate.h"
#include "util/georectangle.h"

namespace archimedes {

map_projection *map_projection::from_string(const std::string &str)
{
	if (str == "equirectangular") {
		return equirectangular_map_projection::get();
	} else if (str == "mercator") {
		return mercator_map_projection::get();
	} else {
		throw std::runtime_error("Invalid map projection: \"" + str + "\".");
	}
}

map_projection::number_type map_projection::latitude_to_scaled_latitude(const number_type &lat) const
{
	return lat;
}

geocoordinate map_projection::geocoordinate_to_scaled_geocoordinate(const geocoordinate &geocoordinate) const
{
	return archimedes::geocoordinate(geocoordinate.get_longitude(), this->latitude_to_scaled_latitude(geocoordinate.get_latitude()));
}

map_projection::number_type map_projection::scaled_latitude_to_latitude(const number_type &scaled_lat) const
{
	return scaled_lat;
}

geocoordinate map_projection::scaled_geocoordinate_to_geocoordinate(const geocoordinate &scaled_geocoordinate) const
{
	return geocoordinate(scaled_geocoordinate.get_longitude(), this->scaled_latitude_to_latitude(scaled_geocoordinate.get_latitude()));
}

map_projection::number_type map_projection::get_latitude_size(const georectangle &georectangle) const
{
	const latitude top_lat = this->latitude_to_scaled_latitude(latitude(georectangle.get_min_latitude()));
	const latitude bottom_lat = this->latitude_to_scaled_latitude(latitude(georectangle.get_max_latitude()));
	return (top_lat - bottom_lat).abs();
}

map_projection::number_type map_projection::longitude_per_pixel(const number_type &lon_size, const QSize &size) const
{
	return lon_size / size.width();
}

map_projection::number_type map_projection::longitude_per_pixel(const georectangle &georectangle, const QSize &size) const
{
	return this->longitude_per_pixel(georectangle.get_width(), size);
}

map_projection::number_type map_projection::latitude_per_pixel(const number_type &lat_size, const QSize &size) const
{
	return lat_size / size.height();
}

map_projection::number_type map_projection::latitude_per_pixel(const georectangle &georectangle, const QSize &size) const
{
	const number_type lat_size = this->get_latitude_size(georectangle);
	return this->latitude_per_pixel(lat_size, size);
}

int map_projection::longitude_to_x(const number_type &longitude, const number_type &lon_per_pixel) const
{
	const number_type x = longitude / lon_per_pixel;
	return x.to_int();
}

int map_projection::latitude_to_y(const number_type &latitude, const number_type &lat_per_pixel) const
{
	const number_type y = latitude * -1 / lat_per_pixel;
	return y.to_int();
}

map_projection::number_type map_projection::x_to_longitude(const int x, const number_type &lon_per_pixel) const
{
	number_type lon = x * lon_per_pixel;
	return lon;
}

map_projection::number_type map_projection::y_to_latitude(const int y, const number_type &lat_per_pixel) const
{
	number_type lat = y * -1 * lat_per_pixel;
	return lat;
}

QPoint map_projection::geocoordinate_to_point(const geocoordinate &geocoordinate, const number_type &lon_per_pixel, const number_type &lat_per_pixel) const
{
	const int x = this->longitude_to_x(geocoordinate.get_longitude(), lon_per_pixel);
	const int y = this->latitude_to_y(geocoordinate.get_latitude(), lat_per_pixel);
	return QPoint(x, y);
}

QPoint map_projection::geocoordinate_to_point(const geocoordinate &geocoordinate, const georectangle &georectangle, const QSize &area_size, const int x_offset) const
{
	const longitude lon_per_pixel = this->longitude_per_pixel(georectangle, area_size);
	const latitude lat_per_pixel = this->latitude_per_pixel(georectangle, area_size);

	const archimedes::geocoordinate origin_geocoordinate(georectangle.get_min_longitude(), georectangle.get_max_latitude());
	const archimedes::geocoordinate scaled_origin_geocoordinate = this->geocoordinate_to_scaled_geocoordinate(origin_geocoordinate);
	const QPoint geocoordinate_offset = this->geocoordinate_to_point(scaled_origin_geocoordinate, lon_per_pixel, lat_per_pixel);

	const archimedes::geocoordinate scaled_geocoordinate = this->geocoordinate_to_scaled_geocoordinate(geocoordinate);

	QPoint point = this->geocoordinate_to_point(scaled_geocoordinate, lon_per_pixel, lat_per_pixel) - geocoordinate_offset;

	if (x_offset != 0) {
		point.setX(point.x() + x_offset);
		if (point.x() < 0) {
			point.setX(point.x() + area_size.width());
		} else if (point.x() >= area_size.width()) {
			point.setX(point.x() % area_size.width());
		}
	}

	return point;
}

geocoordinate map_projection::point_to_geocoordinate(const QPoint &point, const number_type &lon_per_pixel, const number_type &lat_per_pixel) const
{
	longitude lon = this->x_to_longitude(point.x(), lon_per_pixel);
	latitude lat = this->y_to_latitude(point.y(), lat_per_pixel);
	return geocoordinate(std::move(lon), std::move(lat));
}

geocoordinate map_projection::point_to_geocoordinate(const QPoint &point, const georectangle &georectangle, const QSize &area_size, const int x_offset) const
{
	const longitude lon_per_pixel = this->longitude_per_pixel(georectangle, area_size);
	const latitude lat_per_pixel = this->latitude_per_pixel(georectangle, area_size);

	const geocoordinate origin_geocoordinate(georectangle.get_min_longitude(), georectangle.get_max_latitude());
	const geocoordinate scaled_origin_geocoordinate = this->geocoordinate_to_scaled_geocoordinate(origin_geocoordinate);
	const QPoint geocoordinate_offset = this->geocoordinate_to_point(scaled_origin_geocoordinate, lon_per_pixel, lat_per_pixel);

	QPoint point_with_offset = point;
	if (x_offset != 0) {
		point_with_offset.setX(point_with_offset.x() - x_offset);
		if (point_with_offset.x() < 0) {
			point_with_offset.setX(point_with_offset.x() + area_size.width());
		} else if (point_with_offset.x() >= area_size.width()) {
			point_with_offset.setX(point_with_offset.x() % area_size.width());
		}
	}

	const geocoordinate scaled_geocoordinate = this->point_to_geocoordinate(point_with_offset + geocoordinate_offset, lon_per_pixel, lat_per_pixel);
	return this->scaled_geocoordinate_to_geocoordinate(scaled_geocoordinate);
}

}
