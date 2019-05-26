#ifndef _INCLUDE_GUARD_UUID_54DFA1A4_3FF6_4CE5_AA5D_8D1BEAF4C53D_
#define _INCLUDE_GUARD_UUID_54DFA1A4_3FF6_4CE5_AA5D_8D1BEAF4C53D_

#include "rule.h"

namespace genelife {
class Cell {
public:
  int state;
  int age = 0;
  int sleep_hours = 1;
  Rule rule;
  double vx = 0, vy = 0;

  std::shared_ptr<Cell> clone() {
    auto dest = std::make_shared<Cell>();
    dest->rule = rule;
    dest->state = state;
    dest->age = age;
    dest->sleep_hours = sleep_hours;
    dest->vx = vx;
    dest->vy = vy;

    return dest;
  }

  bool is_living() const { return state == rule.max_state() - 1; }
  bool is_dying() const { return state < rule.max_state() - 1 && !is_dead(); }
  bool is_dead() const { return state == 0; }

  bool is_sleeping() const { return age % sleep_hours != 0; }

  double get_vx() const {
    if (is_living())
      return vx;
    else
      return 0;
  }

  double get_vy() const {
    if (is_living())
      return vy;
    else
      return 0;
  }
};
} // namespace genelife

#endif
