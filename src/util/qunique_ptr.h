#pragma once

namespace archimedes {

struct qobject_deleter final
{
	void operator()(QObject *object)
	{
		if (object->thread() != nullptr && !object->thread()->isFinished()) {
			object->deleteLater();
		} else {
			delete object;
		}
	}
};

//unique pointer that calls QObject::deleteLater
template <typename T>
using qunique_ptr = std::unique_ptr<T, qobject_deleter>;

template <typename T, class... ARGS>
inline qunique_ptr<T> make_qunique(ARGS&&... args)
{
	auto ptr = qunique_ptr<T>(new T(std::forward<ARGS>(args)...));
	ptr->setParent(QApplication::instance()); //to ensure that the object isn't deleted by QML if provided to it as a pointer
	return ptr;
}

}
