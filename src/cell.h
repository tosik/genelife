#ifndef _INCLUDE_GUARD_UUID_54DFA1A4_3FF6_4CE5_AA5D_8D1BEAF4C53D_
#define _INCLUDE_GUARD_UUID_54DFA1A4_3FF6_4CE5_AA5D_8D1BEAF4C53D_

#include "rule.h"

namespace genelife {
class Cell {
public:
  int state;
  Rule rule;

  void kill() {
    if (!is_dead())
      state--;
    rule.is_enabled = false;
  }

  void be_born() {
    if (!is_living())
      state = rule.max_state - 1;
    rule.is_enabled = true;
  }

  std::shared_ptr<Cell> clone() {
    auto dest = std::make_shared<Cell>();
    dest->rule = rule;
    dest->state = state;

    return dest;
  }

  bool is_living() { return state == rule.max_state - 1; }
  bool is_dying() { return state < rule.max_state - 1 && !is_dead(); }
  bool is_dead() { return state == 0; }
};
} // namespace genelife

#endif
