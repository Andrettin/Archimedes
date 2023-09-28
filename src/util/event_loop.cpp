#include "archimedes.h"

#include "util/event_loop.h"

#include "util/assert_util.h"
#include "util/exception_util.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/use_awaitable.hpp>

namespace archimedes {

event_loop::event_loop()
{
	this->io_context = std::make_unique<boost::asio::io_context>();

	this->run_timer = make_qunique<QTimer>(QApplication::instance());
	this->run_timer->setSingleShot(true);
	this->run_timer->setInterval(1);

	QObject::connect(this->run_timer.get(), &QTimer::timeout, [this]() {
		this->run_io_context();
	});

	this->run_timer->start();
}

event_loop::~event_loop()
{
	this->stop();
}

void event_loop::stop()
{
	this->io_context->stop();
}

void event_loop::post(const std::function<void()> &function)
{
	boost::asio::post(*this->io_context, [this, function]() {
		try {
			function();
		} catch (...) {
			exception::report(std::current_exception());
			std::terminate();
		}
	});
}

void event_loop::co_spawn(const std::function<boost::asio::awaitable<void>()> &function)
{
	boost::asio::co_spawn(this->io_context->get_executor(), [this, function]() -> boost::asio::awaitable<void> {
		try {
			co_await function();
		} catch (...) {
			exception::report(std::current_exception());
			std::terminate();
		}
	}, boost::asio::detached);
}

void event_loop::run_io_context()
{
	//restart the IO context, in case it was placed in the stopped state due to running out of work
	this->io_context->restart();

	//only run for 1 millisecond, as otherwise the IO context can potentially keep running forever, blocking the Qt UI thread permanently
	this->io_context->run_for(std::chrono::milliseconds(1));

	this->run_timer->start();
}

}
