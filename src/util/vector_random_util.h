#pragma once

#include "util/random.h"

namespace archimedes::vector {

template <typename T>
inline const T &get_random(const std::vector<T> &vector)
{
	return vector[random::get()->generate(vector.size())];
}

template <typename T>
inline const T &get_random_async(const std::vector<T> &vector)
{
	return vector[random::get()->generate_async(vector.size())];
}

template <typename T>
inline T take_random(std::vector<T> &vector)
{
	const size_t index = random::get()->generate(vector.size());
	T element = std::move(vector[index]);
	vector.erase(vector.begin() + index);
	return element;
}

template <typename T, typename function_type>
inline void process_randomly(std::vector<T> &vector, const function_type &function)
{
	while (!vector.empty()) {
		T element = vector::take_random(vector);
		function(std::move(element));
	}
}

template <typename T, typename function_type>
inline void process_randomly(const std::vector<T> &vector, const function_type &function)
{
	std::vector<T> vector_copy = vector;
	vector::process_randomly(vector_copy, function);
}

template <typename T>
inline void shuffle(std::vector<T> &vector)
{
	if (vector.empty()) {
		//nothing to shuffle
		return;
	}

	std::shuffle(vector.begin(), vector.end(), random::get()->get_engine());
}

template <typename T>
inline std::vector<T> shuffled(const std::vector<T> &vector)
{
	if (vector.empty()) {
		//nothing to shuffle
		return std::vector<T>();
	}

	std::vector<T> shuffled_vector = vector;
	vector::shuffle(shuffled_vector);

	return shuffled_vector;
}

}
