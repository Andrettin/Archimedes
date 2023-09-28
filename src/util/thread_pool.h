#pragma once

#include "util/singleton.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_awaitable.hpp>

namespace boost::asio {
	class thread_pool;
}

namespace archimedes {

//a singleton providing a thread pool
class thread_pool final : public singleton<thread_pool>
{
public:
	thread_pool();
	~thread_pool();

	void stop();

	void post(const std::function<void()> &function);

	void co_spawn(const std::function<boost::asio::awaitable<void>()> &function);
	void co_spawn_sync(const std::function<boost::asio::awaitable<void>()> &function);

	template <typename function_type>
	[[nodiscard]]
	inline std::invoke_result_t<function_type> co_spawn_awaitable(function_type &&function)
	{
		return boost::asio::co_spawn(this->pool->get_executor(), std::move(function), boost::asio::use_awaitable);
	}

	template <typename function_type>
	[[nodiscard]]
	inline QFuture<typename std::invoke_result_t<function_type>::value_type> co_spawn_future(function_type &&function)
	{
		using value_type = typename std::invoke_result_t<function_type>::value_type;

		std::shared_ptr<QPromise<value_type>> promise = std::make_shared<QPromise<value_type>>();

		boost::asio::co_spawn(this->pool->get_executor(), [function = std::move(function), promise]() -> boost::asio::awaitable<void> {
			promise->start();
			if constexpr (std::is_void_v<value_type>) {
				co_await function();
			} else {
				auto result = co_await function();
				promise->addResult(std::move(result));
			}
			promise->finish();
		}, boost::asio::detached);

		return promise->future();
	}

private:
	std::unique_ptr<boost::asio::thread_pool> pool;
};

}
