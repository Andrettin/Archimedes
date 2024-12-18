#include "archimedes.h"

#include "util/random.h"

#include "util/assert_util.h"
#include "util/dice.h"
#include "util/geocoordinate.h"

#include <boost/random.hpp>

namespace archimedes {

template <typename int_type>
int_type random::generate_in_range(std::mt19937 &engine, const int_type min_value, const int_type max_value)
{
	static_assert(std::is_integral_v<int_type>);

	assert_throw(max_value >= min_value);

	if (max_value == min_value) {
		return min_value;
	}

	//we have to use the Boost number distribution here since it is portable (has the same result with different compilers), while the standard library's isn't
	boost::random::uniform_int_distribution<int_type> distribution(min_value, max_value);
	int_type result = distribution(engine);

	return result;
}

template int16_t random::generate_in_range<int16_t>(std::mt19937 &, const int16_t, const int16_t);
template uint16_t random::generate_in_range<uint16_t>(std::mt19937 &, const uint16_t, const uint16_t);
template int32_t random::generate_in_range<int32_t>(std::mt19937 &, const int32_t, const int32_t);
template uint32_t random::generate_in_range<uint32_t>(std::mt19937 &, const uint32_t, const uint32_t);
template int64_t random::generate_in_range<int64_t>(std::mt19937 &, const int64_t, const int64_t);
template uint64_t random::generate_in_range<uint64_t>(std::mt19937 &, const uint64_t, const uint64_t);

int random::roll_dice(const dice &dice)
{
	return this->roll_dice(dice.get_count(), dice.get_sides());
}

geocoordinate random::generate_geocoordinate()
{
	static constexpr int64_t longitude_size_value = geocoordinate::longitude_size * longitude::divisor;
	static constexpr int64_t latitude_size_value = geocoordinate::latitude_size * latitude::divisor;

	longitude lon = longitude::from_value(this->generate<int64_t>(longitude_size_value) - (longitude_size_value / 2));
	latitude lat = latitude::from_value(this->generate<int64_t>(latitude_size_value) - (latitude_size_value / 2));

	return geocoordinate(std::move(lon), std::move(lat));
}

}
