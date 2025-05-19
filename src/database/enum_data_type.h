#pragma once

#include "database/data_type.h"

#include <magic_enum/magic_enum.hpp>

namespace archimedes {

template <typename T, typename enum_type>
class enum_data_type : public data_type<T>
{
public:
	using data_type<T>::get;
	using data_type<T>::try_get;

	static T *get(const enum_type value)
	{
		T *instance = T::try_get(value);

		if (instance == nullptr) {
			throw std::runtime_error(std::format("Invalid {} instance: \"{}\".", T::class_identifier, magic_enum::enum_name(value)));
		}

		return instance;
	}

	static T *try_get(const enum_type value)
	{
		const auto find_iterator = enum_data_type::instances_by_enum_value.find(value);
		if (find_iterator != enum_data_type::instances_by_enum_value.end()) {
			return find_iterator->second;
		}

		return nullptr;
	}

	static T *add(const std::string &identifier, const data_module *data_module)
	{
		T *instance = data_type<T>::add(identifier, data_module);

		const enum_type value = magic_enum::enum_cast<enum_type>(identifier).value();

		enum_data_type::instances_by_enum_value[value] = instance;

		return instance;
	}

	static void clear()
	{
		data_type<T>::clear();
		enum_data_type::instances_by_enum_value.clear();
	}

private:
	static inline std::map<enum_type, T *> instances_by_enum_value;
};

}
