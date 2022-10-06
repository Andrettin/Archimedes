#pragma once

namespace archimedes {

class enum_converter_base
{
protected:
	static void register_string_to_qvariant_conversion(const std::string &class_name, std::function<QVariant(const std::string &)> &&function);
};

template <typename enum_type>
class enum_converter final : public enum_converter_base
{
private:
	explicit enum_converter(std::string &&property_class_identifier, std::map<std::string, enum_type> &&string_to_enum_map)
		: property_class_identifier(std::move(property_class_identifier)),
		string_to_enum_map(std::move(string_to_enum_map))
	{
		this->initialize_enum_to_string_map();

		enum_converter_base::register_string_to_qvariant_conversion(this->property_class_identifier, [](const std::string &value) {
			return QVariant::fromValue(enum_converter::to_enum(value));
		});
	}

public:
	static enum_type to_enum(const std::string &str)
	{
		const auto find_iterator = enum_converter::instance.string_to_enum_map.find(str);

		if (find_iterator != enum_converter::instance.string_to_enum_map.end()) {
			return find_iterator->second;
		}

		throw std::runtime_error("Invalid enum string: \"" + str + "\".");
	}

	static const std::string &to_string(const enum_type enum_value)
	{
		const auto find_iterator = enum_converter::instance.enum_to_string_map.find(enum_value);

		if (find_iterator != enum_converter::instance.enum_to_string_map.end()) {
			return find_iterator->second;
		}

		throw std::runtime_error("Invalid enum value: " + std::to_string(static_cast<std::underlying_type_t<enum_type>>(enum_value)) + ".");
	}

private:
	static const enum_converter instance;

	void initialize_enum_to_string_map()
	{
		for (const auto &[enum_str, enum_value] : this->string_to_enum_map) {
			this->enum_to_string_map[enum_value] = enum_str;
		}
	}

private:
	const std::string property_class_identifier;
	const std::map<std::string, enum_type> string_to_enum_map; //this should be defined where the enum converter is specialized
	std::map<enum_type, std::string> enum_to_string_map;
};

}
