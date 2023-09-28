#include "archimedes.h"

#include "util/thread_pool.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/thread_pool.hpp>

namespace archimedes {

thread_pool::thread_pool()
{
	const size_t thread_count = std::max<size_t>(std::thread::hardware_concurrency(), 1);
	this->pool = std::make_unique<boost::asio::thread_pool>(thread_count);
}

thread_pool::~thread_pool()
{
	this->stop();
}

void thread_pool::stop()
{
	this->pool->stop();
	this->pool->join();
}

void thread_pool::post(const std::function<void()> &function)
{
	boost::asio::post(*this->pool, function);
}

void thread_pool::co_spawn(const std::function<boost::asio::awaitable<void>()> &function)
{
	boost::asio::co_spawn(this->pool->get_executor(), function, boost::asio::detached);
}

void thread_pool::co_spawn_sync(const std::function<boost::asio::awaitable<void>()> &function)
{
	//spawn coroutine and wait until it is complete
	std::promise<void> promise;
	std::future<void> future = promise.get_future();

	this->co_spawn([&promise, &function]() -> boost::asio::awaitable<void> {
		try {
			co_await function();
			promise.set_value();
		} catch (...) {
			promise.set_exception(std::current_exception());
		}
	});

	future.wait();
}

}
