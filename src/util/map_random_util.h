#pragma once

#include "util/assert_util.h"
#include "util/random.h"

namespace archimedes::map {

template <typename T>
inline const typename T::key_type &get_random_weight_map_key(const T &map)
{
	using weight_type = typename T::mapped_type;
	static_assert(std::is_integral_v<weight_type>);

	weight_type total_weight = 0;
	for (const auto &[key, weight] : map) {
		total_weight += weight;
	}

	weight_type generation_result = random::get()->generate(total_weight);
	for (const auto &[key, weight] : map) {
		if (generation_result < weight) {
			return key;
		} else {
			generation_result -= weight;
		}
	}

	assert_throw(false);
	return map.begin()->first;
}

}
