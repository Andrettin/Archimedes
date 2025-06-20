#include "archimedes.h"

#include "game/game_rule_group.h"

#include "game/game_rule.h"
#include "util/container_util.h"

namespace archimedes {

QVariantList game_rule_group::get_rules_qvariant_list() const
{
	return container::to_qvariant_list(this->get_rules());
}

}
