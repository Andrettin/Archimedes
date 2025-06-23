#include "archimedes.h"

#include "game/game_rule_container.h"

#include "game/game_rule.h"

namespace archimedes {

bool game_rule_compare::operator()(const game_rule *lhs, const game_rule *rhs) const
{
	const int lhs_requirement_depth = lhs->get_requirement_depth();
	const int rhs_requirement_depth = rhs->get_requirement_depth();
	if (lhs_requirement_depth != rhs_requirement_depth) {
		return lhs_requirement_depth < rhs_requirement_depth;
	}

	return lhs->get_identifier() < rhs->get_identifier();
}

}
