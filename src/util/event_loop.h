#pragma once

#include "util/qunique_ptr.h"
#include "util/singleton.h"

namespace boost::asio {
	class io_context;
}

namespace archimedes {

class event_loop final : public singleton<event_loop>
{
public:
	event_loop();
	~event_loop();

	boost::asio::io_context &get_io_context() const
	{
		return *this->io_context;
	}

	void stop();

	void post(const std::function<void()> &function);

	void co_spawn(const std::function<boost::asio::awaitable<void>()> &function);

private:
	void run_io_context();

private:
	std::unique_ptr<boost::asio::io_context> io_context;
	qunique_ptr<QTimer> run_timer; //the timer for running the event loop again
};

}
