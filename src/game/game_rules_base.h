#pragma once

namespace archimedes {

class game_rule;

class game_rules_base : public QObject
{
	Q_OBJECT

public:
	virtual bool get_value(const game_rule *rule) const = 0;
};

}
