#pragma once

namespace archimedes {

class game_rule;

struct game_rule_compare final
{
	bool operator()(const game_rule *lhs, const game_rule *rhs) const;
};

using game_rule_set = std::set<const game_rule *, game_rule_compare>;

template <typename T>
using game_rule_map = std::map<const game_rule *, T, game_rule_compare>;

}
