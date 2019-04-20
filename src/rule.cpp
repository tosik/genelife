#include "rule.h"

#include "cell.h"

#include <iostream>

#define RULE_D(i)                                                              \
  if (sum == i && (rule & bd##i) == bd##i) {                                   \
    return max_state() - 1;                                                    \
  }

#define RULE_L(i)                                                              \
  if (sum == i && (rule & bl##i) == bl##i) {                                   \
    return max_state() - 1;                                                    \
  }

namespace genelife {

int Rule::index_of_rule() { return gene & 0xf; }

int Rule::max_state() {
  auto rule = rules[index_of_rule()];

  auto mask =
      0b00000000'00000111'00000000'00000000'00000000'00000000'00000000'00000000;
  return ((rule & mask) >> (8 * 6)) + 2;
}

int Rule::run(std::shared_ptr<Cell> c0, std::shared_ptr<Cell> c1,
              std::shared_ptr<Cell> c2, std::shared_ptr<Cell> c3,
              std::shared_ptr<Cell> c4, std::shared_ptr<Cell> c5,
              std::shared_ptr<Cell> c6, std::shared_ptr<Cell> c7,
              std::shared_ptr<Cell> c8) {

  auto rule = rules[index_of_rule()];

  // 8 neighbors outer totalistic
  int sum = 0;
  {
    if (c0->is_living())
      sum++;
    if (c1->is_living())
      sum++;
    if (c2->is_living())
      sum++;
    if (c3->is_living())
      sum++;
    if (c5->is_living())
      sum++;
    if (c6->is_living())
      sum++;
    if (c7->is_living())
      sum++;
    if (c8->is_living())
      sum++;
  }

  constexpr std::uint64_t bd0 =
      0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
  constexpr std::uint64_t bd1 = bd0 >> 1;
  constexpr std::uint64_t bd2 = bd0 >> 2;
  constexpr std::uint64_t bd3 = bd0 >> 3;
  constexpr std::uint64_t bd4 = bd0 >> 4;
  constexpr std::uint64_t bd5 = bd0 >> 5;
  constexpr std::uint64_t bd6 = bd0 >> 6;
  constexpr std::uint64_t bd7 = bd0 >> 7;
  constexpr std::uint64_t bd8 = bd0 >> 8;

  constexpr std::uint64_t bl0 =
      0b00000000'00000000'10000000'00000000'00000000'00000000'00000000'00000000;
  constexpr std::uint64_t bl1 = bl0 >> 1;
  constexpr std::uint64_t bl2 = bl0 >> 2;
  constexpr std::uint64_t bl3 = bl0 >> 3;
  constexpr std::uint64_t bl4 = bl0 >> 4;
  constexpr std::uint64_t bl5 = bl0 >> 5;
  constexpr std::uint64_t bl6 = bl0 >> 6;
  constexpr std::uint64_t bl7 = bl0 >> 7;
  constexpr std::uint64_t bl8 = bl0 >> 8;

  if (c4->is_dead()) {
    RULE_D(0);
    RULE_D(1);
    RULE_D(2);
    RULE_D(3);
    RULE_D(4);
    RULE_D(5);
    RULE_D(6);
    RULE_D(7);
    RULE_D(8);
  }
  if (c4->is_living()) {
    RULE_L(0);
    RULE_L(1);
    RULE_L(2);
    RULE_L(3);
    RULE_L(4);
    RULE_L(5);
    RULE_L(6);
    RULE_L(7);
    RULE_L(8);
  }

  return c4->state - 1;
}

} // namespace genelife
