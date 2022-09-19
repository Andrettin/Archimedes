#include "archimedes.h"

#include "util/geocoordinate.h"

#include "util/point_util.h"
#include "util/random.h"

namespace archimedes {

void geocoordinate::for_each_random_until(const std::function<bool(const geocoordinate &)> &function)
{
	//call a function for each possible geocoordinate until the function returns true, going through them in a random manner

	static constexpr uint64_t max_potential_longitudes = static_cast<uint64_t>((geocoordinate::max_longitude - geocoordinate::min_longitude).get_value());
	static constexpr uint64_t max_potential_latitudes = static_cast<uint64_t>((geocoordinate::max_latitude - geocoordinate::min_latitude).get_value());
	static constexpr uint64_t max_potential_geocoordinates = max_potential_longitudes * max_potential_latitudes;

	geocoordinate_set checked_geocoordinates;

	while (checked_geocoordinates.size() < max_potential_geocoordinates && checked_geocoordinates.size() < std::numeric_limits<size_t>::max()) {
		const geocoordinate geocoordinate = random::get()->generate_geocoordinate();

		if (checked_geocoordinates.contains(geocoordinate)) {
			continue;
		} else {
			checked_geocoordinates.insert(geocoordinate);
		}

		if (function(geocoordinate)) {
			break;
		}
	}
}

QPoint geocoordinate::to_circle_edge_point() const
{
	const QPoint circle_point = this->to_circle_point();
	return point::get_nearest_circle_edge_point(circle_point, 1 * number_type::divisor);
}

bool geocoordinate_compare::operator()(const geocoordinate &geocoordinate, const archimedes::geocoordinate &other_geocoordinate) const
{
	if (geocoordinate.get_longitude() != other_geocoordinate.get_longitude()) {
		return geocoordinate.get_longitude() < other_geocoordinate.get_longitude();
	}

	return geocoordinate.get_latitude() < other_geocoordinate.get_latitude();
}

}
