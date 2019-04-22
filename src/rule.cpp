#include "rule.h"

#include "cell.h"

#include <iostream>

#define RULE_D(i)                                                              \
  if (sum == i && (gene & bd##i) == bd##i) {                                   \
    return max_state() - 1;                                                    \
  }

#define RULE_L(i)                                                              \
  if (sum == i && (gene & bl##i) == bl##i) {                                   \
    return max_state() - 1;                                                    \
  } else {                                                                     \
    dying = true;                                                              \
  }

namespace genelife {

void Rule::mutate(std::mt19937_64 &random_engine) {
  if (random_engine() % 10 > 5) {
    // reverse a random bit
    std::uint64_t changing_bit = 1ULL << (random_engine() % 32);
    gene ^= changing_bit;
  } else {
    // exchange hidden bits and using bits
    std::uint64_t right_bits = gene & 0xffff'ffff;
    std::uint64_t left_bits = (gene >> 32) & 0xffff'ffff;
    gene = left_bits | (right_bits << 32);
  }
}

void Rule::mutate(int value) {
  static std::uint64_t rules[] = {
      0b00100000'00000010'00011100'00000000'00000000'00000000'00000000'00000000, // starwars
      0b00001111'10000010'01111111'00000000'00000000'00000000'00000000'00000000, // lava'2
      0b00001111'10000110'01111100'00000000'00000000'00000000'00000000'00000000, // lava
      0b00001100'10000001'11111100'00000000'00000000'00000000'00000000'00000000, // lines
      0b00010001'10000010'01111111'00000000'00000000'00000000'00000000'00000000, // caterpillars
      0b00101111'00000100'10000111'10000000'00000000'00000000'00000000'00000000, // chenille
      0b00100000'00000100'00011110'00000000'00000000'00000000'00000000'00000000, // sticks
      0b00010000'00000000'00110000'00000000'00000000'00000000'00000000'00000000, // conway's game of life
  };

  int a = 100;
  if (value % a == 0) {
    auto i = (value / a) % (sizeof(rules) / sizeof(*rules));
    gene = rules[i];
  }
  if (value % 10 == 0) {
    std::uint64_t changing_bit = 1ULL << (value % 32);
    gene ^= changing_bit;
  }
}

int Rule::max_state() {
  auto mask =
      0b00000000'00000111'00000000'00000000'00000000'00000000'00000000'00000000;
  return ((gene & mask) >> (8 * 6)) + 2;
}

int Rule::run(std::shared_ptr<Cell> c0, std::shared_ptr<Cell> c1,
              std::shared_ptr<Cell> c2, std::shared_ptr<Cell> c3,
              std::shared_ptr<Cell> c4, std::shared_ptr<Cell> c5,
              std::shared_ptr<Cell> c6, std::shared_ptr<Cell> c7,
              std::shared_ptr<Cell> c8) {

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
  } else if (c4->is_living()) {
    bool dying = false;
    RULE_L(0);
    RULE_L(1);
    RULE_L(2);
    RULE_L(3);
    RULE_L(4);
    RULE_L(5);
    RULE_L(6);
    RULE_L(7);
    RULE_L(8);
    if (dying)
      return c4->state - 1;
  } else if (c4->is_dying()) {
    return c4->state - 1;
  }
  return 0;
}

} // namespace genelife
