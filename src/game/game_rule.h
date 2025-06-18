#pragma once

#include "database/data_type.h"
#include "database/named_data_entry.h"

namespace archimedes {

class game_rule final : public named_data_entry, public data_type<game_rule>
{
	Q_OBJECT

	Q_PROPERTY(bool default_value MEMBER default_value READ get_default_value NOTIFY changed)
	Q_PROPERTY(QString description READ get_description_qstring NOTIFY changed)
	Q_PROPERTY(bool hidden MEMBER hidden READ is_hidden NOTIFY changed)

public:
	static constexpr const char class_identifier[] = "game_rule";
	static constexpr const char property_class_identifier[] = "archimedes::game_rule*";
	static constexpr const char database_folder[] = "game_rules";

	explicit game_rule(const std::string &identifier) : named_data_entry(identifier)
	{
	}

	bool get_default_value() const
	{
		return this->default_value;
	}

	const std::string &get_description() const
	{
		return this->description;
	}

	Q_INVOKABLE void set_description(const std::string &description)
	{
		this->description = description;
	}

	QString get_description_qstring() const
	{
		return QString::fromStdString(this->get_description());
	}

	bool is_hidden() const
	{
		return this->hidden;
	}

signals:
	void changed();

private:
	bool default_value = false;
	std::string description;
	bool hidden = false;
};

}
