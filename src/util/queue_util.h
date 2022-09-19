#pragma once

namespace archimedes::queue {

template <typename T>
T take(std::queue<T> &queue)
{
	T value = std::move(queue.front());
	queue.pop();
	return value;
}

template <typename T, typename function_type>
void process(std::queue<T> &queue, const function_type &function)
{
	while (!queue.empty()) {
		function(queue.front());
		queue.pop();
	}
}

}
