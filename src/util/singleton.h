#pragma once

namespace archimedes {

template <typename T>
class singleton
{
protected:
	singleton() = default;
	singleton(const singleton &other) = delete;
	singleton &operator =(const singleton &other) = delete;

public:
	static T *get()
	{
		static std::unique_ptr<T> instance = T::create();

		return instance.get();
	}

private:
	static std::unique_ptr<T> create()
	{
		std::unique_ptr<T> instance = std::make_unique<T>();

		if constexpr (std::is_base_of_v<QObject, T>) {
			if (QApplication::instance()->thread() != QThread::currentThread()) {
				instance->moveToThread(QApplication::instance()->thread());
			}
		}

		return instance;
	}
};

}
