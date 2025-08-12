#include "archimedes.h"

#include "map/central_cylindrical_map_projection.h"

#include "util/angle_util.h"
#include "util/geocoordinate.h"

namespace archimedes {

central_cylindrical_map_projection::number_type central_cylindrical_map_projection::latitude_to_scaled_latitude(const number_type &lat) const
{
	const double lat_radians = angle::degrees_to_radians(lat.to_double());

	const double scaled_lat_radians = std::tan(lat_radians);

	const double scaled_lat_degrees = angle::radians_to_degrees(scaled_lat_radians);
	latitude scaled_lat(scaled_lat_degrees);
	return scaled_lat;
}

central_cylindrical_map_projection::number_type central_cylindrical_map_projection::scaled_latitude_to_latitude(const number_type &scaled_lat) const
{
	const double scaled_lat_radians = angle::degrees_to_radians(scaled_lat.to_double());

	const double lat_radians = std::atan(scaled_lat_radians);

	const double lat_degrees = angle::radians_to_degrees(lat_radians);
	latitude lat(lat_degrees);
	return lat;
}

}
