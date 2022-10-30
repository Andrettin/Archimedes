#include "archimedes.h"

#include "map/miller_map_projection.h"

#include "util/angle_util.h"
#include "util/geocoordinate.h"

#include <boost/math/constants/constants.hpp>

namespace archimedes {

static const double pi = boost::math::constants::pi<double>();
static const double quarter_pi = pi / 4.;

miller_map_projection::number_type miller_map_projection::latitude_to_scaled_latitude(const number_type &lat) const
{
	const double lat_radians = angle::degrees_to_radians(lat.to_double());

	const double scaled_lat_radians = 1.25 * std::log(std::tan(quarter_pi + 0.4 * lat_radians));

	const double scaled_lat_degrees = angle::radians_to_degrees(scaled_lat_radians);
	latitude scaled_lat(scaled_lat_degrees);
	return scaled_lat;
}

miller_map_projection::number_type miller_map_projection::scaled_latitude_to_latitude(const number_type &scaled_lat) const
{
	const double scaled_lat_radians = angle::degrees_to_radians(scaled_lat.to_double());

	const double lat_radians = 2.5 * std::atan(std::exp(0.8 * scaled_lat_radians)) - 0.625 * pi;

	const double lat_degrees = angle::radians_to_degrees(lat_radians);
	latitude lat(lat_degrees);
	return lat;
}

}
