#include "archimedes.h"

#include "map/mercator_map_projection.h"

#include "util/angle_util.h"
#include "util/geocoordinate.h"

#include <boost/math/constants/constants.hpp>

namespace archimedes {

static const double half_pi = boost::math::constants::pi<double>() / 2.;

mercator_map_projection::number_type mercator_map_projection::latitude_to_scaled_latitude(const number_type &lat) const
{
	const double lat_radians = angle::degrees_to_radians(lat.to_double());

	const double scaled_lat_radians = std::log(std::tan(((half_pi + lat_radians) / 2)));

	const double scaled_lat_degrees = angle::radians_to_degrees(scaled_lat_radians);
	latitude scaled_lat(scaled_lat_degrees);
	return scaled_lat;
}

mercator_map_projection::number_type mercator_map_projection::scaled_latitude_to_latitude(const number_type &scaled_lat) const
{
	const double scaled_lat_radians = angle::degrees_to_radians(scaled_lat.to_double());

	const double lat_radians = 2 * std::atan(std::exp(scaled_lat_radians)) - half_pi;

	const double lat_degrees = angle::radians_to_degrees(lat_radians);
	latitude lat(lat_degrees);
	return lat;
}

void mercator_map_projection::validate_area(const georectangle &georectangle, const QSize &area_size) const
{
	const longitude lon_per_pixel = this->longitude_per_pixel(georectangle, area_size);
	const latitude lat_per_pixel = this->latitude_per_pixel(georectangle, area_size);

	if (lon_per_pixel != lat_per_pixel) {
		throw std::runtime_error("The scaled longitude per pixel (" + lon_per_pixel.to_string() + ") is different than the scaled latitude per pixel (" + lat_per_pixel.to_string() + ").");
	}
}

}
