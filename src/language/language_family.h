#pragma once

#include "database/data_type.h"
#include "database/named_data_entry.h"

namespace archimedes {

class language_family final : public named_data_entry, public data_type<language_family>
{
	Q_OBJECT

	Q_PROPERTY(archimedes::language_family* family MEMBER family READ get_family)

public:
	static constexpr const char class_identifier[] = "language_family";
	static constexpr const char property_class_identifier[] = "archimedes::language_family*";
	static constexpr const char database_folder[] = "language_families";

	explicit language_family(const std::string &identifier) : named_data_entry(identifier)
	{
	}

	language_family *get_family() const
	{
		return this->family;
	}

private:
	language_family *family = nullptr; //the upper family to which this language family belongs
};

}
