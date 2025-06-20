#include "archimedes.h"

#include "game/game_rule.h"

#include "game/game_rule_group.h"

namespace archimedes {

void game_rule::initialize()
{
	if (this->group != nullptr) {
		this->group->add_rule(this);
	}

	named_data_entry::initialize();
}

}
