#pragma once

namespace archimedes::list {

template <typename T>
T take_front(std::list<T> &list)
{
	T value = std::move(list.front());
	list.pop_front();
	return value;
}

}
