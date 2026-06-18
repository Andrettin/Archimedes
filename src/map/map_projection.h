#pragma once

namespace archimedes {

class geocoordinate;
class georectangle;

template <int N>
class fractional_int;

class map_projection
{
public:
	using number_type = fractional_int<4>;

	static map_projection *from_string(const std::string &str);

	virtual number_type latitude_to_scaled_latitude(const number_type &lat) const;
	geocoordinate geocoordinate_to_scaled_geocoordinate(const geocoordinate &geocoordinate) const;

	virtual number_type scaled_latitude_to_latitude(const number_type &scaled_lat) const;
	geocoordinate scaled_geocoordinate_to_geocoordinate(const geocoordinate &scaled_geocoordinate) const;

	number_type get_latitude_size(const georectangle &georectangle) const;

	double longitude_per_pixel(const number_type &lon_size, const QSize &size) const;
	double longitude_per_pixel(const georectangle &georectangle, const QSize &size) const;
	double latitude_per_pixel(const number_type &lat_size, const QSize &size) const;
	double latitude_per_pixel(const georectangle &georectangle, const QSize &size) const;

	int longitude_to_x(const number_type &longitude, const double lon_per_pixel) const;
	int latitude_to_y(const number_type &latitude, const double lat_per_pixel) const;
	number_type x_to_longitude(const int x, const double lon_per_pixel) const;
	number_type y_to_latitude(const int y, const double lat_per_pixel) const;

	QPoint geocoordinate_to_point(const geocoordinate &geocoordinate, const double lon_per_pixel, const double lat_per_pixel) const;
	QPoint geocoordinate_to_point(const geocoordinate &geocoordinate, const georectangle &georectangle, const QSize &area_size, const int x_offset = 0) const;

	geocoordinate point_to_geocoordinate(const QPoint &point, const double lon_per_pixel, const double lat_per_pixel) const;
	geocoordinate point_to_geocoordinate(const QPoint &point, const georectangle &georectangle, const QSize &area_size, const int x_offset = 0) const;

	QPolygon geopolygon_to_polygon(const QGeoPolygon &geopolygon, const georectangle &map_georectangle, const QSize &map_size, const int x_offset) const;

	void validate_area(const georectangle &georectangle, const QSize &area_size) const;
};

}

Q_DECLARE_METATYPE(archimedes::map_projection *)
