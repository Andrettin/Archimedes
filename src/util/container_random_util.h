#pragma once

#include "util/random.h"

namespace archimedes::container {

template <typename T>
inline const typename T::value_type &get_random(const T &container)
{
	return container[random::get()->generate(container.size())];
}

template <typename T>
inline const typename T::value_type &get_random_async(const T &container)
{
	return container[random::get()->generate_async(container.size())];
}

}
