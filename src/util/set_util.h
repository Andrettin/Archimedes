#pragma once

namespace archimedes::set {

static inline const std::set<std::string> empty_string_set;

template <typename T, typename U>
void merge(T &set, const U &other_container)
{
	for (const typename U::value_type &element : other_container) {
		set.insert(element);
	}
}

template <typename T, typename U>
void merge(T &set, U &&other_container)
{
	for (typename U::value_type &element : other_container) {
		set.insert(std::move(element));
	}
}

}
