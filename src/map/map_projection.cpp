#include "archimedes.h"

#include "map/map_projection.h"

#include "map/central_cylindrical_map_projection.h"
#include "map/equirectangular_map_projection.h"
#include "map/mercator_map_projection.h"
#include "map/miller_map_projection.h"
#include "util/geocoordinate.h"
#include "util/georectangle.h"

namespace archimedes {

map_projection *map_projection::from_string(const std::string &str)
{
	if (str == "central_cylindrical") {
		return central_cylindrical_map_projection::get();
	} else if (str == "equirectangular") {
		return equirectangular_map_projection::get();
	} else if (str == "mercator") {
		return mercator_map_projection::get();
	} else if (str == "miller") {
		return miller_map_projection::get();
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

double map_projection::longitude_per_pixel(const number_type &lon_size, const QSize &size) const
{
	return lon_size.to_double() / size.width();
}

double map_projection::longitude_per_pixel(const georectangle &georectangle, const QSize &size) const
{
	return this->longitude_per_pixel(georectangle.get_width(), size);
}

double map_projection::latitude_per_pixel(const number_type &lat_size, const QSize &size) const
{
	return lat_size.to_double() / size.height();
}

double map_projection::latitude_per_pixel(const georectangle &georectangle, const QSize &size) const
{
	const number_type lat_size = this->get_latitude_size(georectangle);
	return this->latitude_per_pixel(lat_size, size);
}

int map_projection::longitude_to_x(const number_type &longitude, const double lon_per_pixel) const
{
	const number_type x = number_type(longitude.to_double() / lon_per_pixel);
	return x.to_int();
}

int map_projection::latitude_to_y(const number_type &latitude, const double lat_per_pixel) const
{
	const number_type y = number_type((latitude * -1).to_double() / lat_per_pixel);
	return y.to_int();
}

map_projection::number_type map_projection::x_to_longitude(const int x, const double lon_per_pixel) const
{
	number_type lon = number_type(x * lon_per_pixel);
	return lon;
}

map_projection::number_type map_projection::y_to_latitude(const int y, const double lat_per_pixel) const
{
	number_type lat = number_type(y * -1 * lat_per_pixel);
	return lat;
}

QPoint map_projection::geocoordinate_to_point(const geocoordinate &geocoordinate, const double lon_per_pixel, const double lat_per_pixel) const
{
	const int x = this->longitude_to_x(geocoordinate.get_longitude(), lon_per_pixel);
	const int y = this->latitude_to_y(geocoordinate.get_latitude(), lat_per_pixel);
	return QPoint(x, y);
}

QPoint map_projection::geocoordinate_to_point(const geocoordinate &geocoordinate, const georectangle &georectangle, const QSize &area_size, const int x_offset) const
{
	const double lon_per_pixel = this->longitude_per_pixel(georectangle, area_size);
	const double lat_per_pixel = this->latitude_per_pixel(georectangle, area_size);

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

geocoordinate map_projection::point_to_geocoordinate(const QPoint &point, const double lon_per_pixel, const double lat_per_pixel) const
{
	longitude lon = this->x_to_longitude(point.x(), lon_per_pixel);
	latitude lat = this->y_to_latitude(point.y(), lat_per_pixel);
	return geocoordinate(std::move(lon), std::move(lat));
}

geocoordinate map_projection::point_to_geocoordinate(const QPoint &point, const georectangle &georectangle, const QSize &area_size, const int x_offset) const
{
	const double lon_per_pixel = this->longitude_per_pixel(georectangle, area_size);
	const double lat_per_pixel = this->latitude_per_pixel(georectangle, area_size);

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

QPolygon map_projection::geopolygon_to_polygon(const QGeoPolygon &geopolygon, const georectangle &map_georectangle, const QSize &map_size, const int x_offset) const
{
	QPolygon polygon;

	for (const QGeoCoordinate &qgeocoordinate : geopolygon.perimeter()) {
		const geocoordinate geocoordinate(qgeocoordinate);
		QPoint point = this->geocoordinate_to_point(geocoordinate, map_georectangle, map_size, x_offset);

		if (!polygon.isEmpty() && polygon.back() == point) {
			continue;
		}

		polygon.append(std::move(point));
	}

	return polygon;
}

void map_projection::validate_area(const georectangle &georectangle, const QSize &area_size) const
{
	const double lon_per_pixel = this->longitude_per_pixel(georectangle, area_size);
	const double lat_per_pixel = this->latitude_per_pixel(georectangle, area_size);

	if (lon_per_pixel != lat_per_pixel) {
		if (georectangle == georectangle::get_global_georectangle()) {
			//if the georectangle comprises the global georectangle, then we should assume that only changes in height can be done to improve the difference in longitude/latitude per pixel
			//therefore, if no height change can be done to improve the per-pixel difference, we don't consider this an error
			const double diff = std::abs(lon_per_pixel - lat_per_pixel);
			const double diff_minus = std::abs(lon_per_pixel - this->latitude_per_pixel(georectangle, QSize(area_size.width(), area_size.height() - 1)));
			const double diff_plus = std::abs(lon_per_pixel - this->latitude_per_pixel(georectangle, QSize(area_size.width(), area_size.height() + 1)));

			if (diff <= diff_minus && diff <= diff_plus) {
				return;
			}
		}

		throw std::runtime_error(std::format("The scaled longitude per pixel ({}) is different than the scaled latitude per pixel ({}).", lon_per_pixel, lat_per_pixel));
	}
}

}
