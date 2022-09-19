#pragma once

namespace archimedes {

struct qobject_deleter final
{
	void operator()(QObject *object)
	{
		object->deleteLater();
	}
};

//unique pointer that calls QObject::deleteLater
template <typename T>
using qunique_ptr = std::unique_ptr<T, qobject_deleter>;

template <typename T, class... ARGS>
inline qunique_ptr<T> make_qunique(ARGS&&... args)
{
	return qunique_ptr<T>(new T(std::forward<ARGS>(args)...));
}

}
