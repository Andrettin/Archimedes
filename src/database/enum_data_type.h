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

	static void clear()
	{
		data_type<T>::clear();
		enum_data_type::instances_by_enum_value.clear();
	}

	static void process_database(const bool definition, const data_module_map<std::vector<gsml_data>> &gsml_data_to_process, const data_type_metadata *metadata)
	{
		data_type<T>::process_database(definition, gsml_data_to_process, metadata);

		if (definition) {
			for (T *enum_data_entry : T::get_all()) {
				const enum_type value = magic_enum::enum_cast<enum_type>(enum_data_entry->get_identifier()).value();

				enum_data_type::instances_by_enum_value[value] = enum_data_entry;
			}
		}
	}

private:
	static inline std::map<enum_type, T *> instances_by_enum_value;
};

}
