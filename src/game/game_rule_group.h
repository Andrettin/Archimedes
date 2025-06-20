#pragma once

#include "database/data_type.h"
#include "database/named_data_entry.h"

namespace archimedes {

class game_rule;

class game_rule_group final : public named_data_entry, public data_type<game_rule_group>
{
	Q_OBJECT

	Q_PROPERTY(QVariantList rules READ get_rules_qvariant_list NOTIFY changed)

public:
	static constexpr const char class_identifier[] = "game_rule_group";
	static constexpr const char property_class_identifier[] = "archimedes::game_rule_group*";
	static constexpr const char database_folder[] = "game_rule_groups";

public:
	explicit game_rule_group(const std::string &identifier) : named_data_entry(identifier)
	{
	}

	const std::vector<const game_rule *> &get_rules() const
	{
		return this->rules;
	}

	QVariantList get_rules_qvariant_list() const;

	void add_rule(const game_rule *rule)
	{
		this->rules.push_back(rule);
	}

signals:
	void changed();

private:
	std::vector<const game_rule *> rules;
};

}
