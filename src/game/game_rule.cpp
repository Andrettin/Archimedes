#include "archimedes.h"

#include "game/game_rule.h"

#include "game/game_rule_group.h"

namespace archimedes {

void game_rule::process_gsml_scope(const gsml_data &scope)
{
	const std::string &tag = scope.get_tag();
	const std::vector<std::string> &values = scope.get_values();

	if (tag == "required_rules") {
		for (const std::string &value : values) {
			game_rule *rule = game_rule::get(value);
			this->required_rules.push_back(rule);
			rule->requiring_rules.push_back(this);
		}
	} else {
		data_entry::process_gsml_scope(scope);
	}
}

void game_rule::initialize()
{
	if (this->group != nullptr && !this->is_hidden()) {
		this->group->add_rule(this);
	}

	named_data_entry::initialize();
}

int game_rule::get_requirement_depth() const
{
	int depth = 0;

	for (const game_rule *required_rule : this->get_required_rules()) {
		depth = std::max(depth, required_rule->get_requirement_depth() + 1);
	}

	return depth;
}

}
