#include "rule.h"

#include "cell.h"

#include <iostream>

namespace genelife {

int Rule::max_state() {
  auto mask =
      0b00000000'00001111'00000000'00000000'00000000'00000000'00000000'00000000;
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
    if (sum == 0 && (gene & bd0) == bd0) {
      return max_state() - 1;
    }
    if (sum == 1 && (gene & bd1) == bd1) {
      return max_state() - 1;
    }
    if (sum == 2 && (gene & bd2) == bd2) {
      return max_state() - 1;
    }
    if (sum == 3 && (gene & bd3) == bd3) {
      return max_state() - 1;
    }
    if (sum == 4 && (gene & bd4) == bd4) {
      return max_state() - 1;
    }
    if (sum == 5 && (gene & bd5) == bd5) {
      return max_state() - 1;
    }
    if (sum == 6 && (gene & bd6) == bd6) {
      return max_state() - 1;
    }
    if (sum == 7 && (gene & bd7) == bd7) {
      return max_state() - 1;
    }
    if (sum == 8 && (gene & bd8) == bd8) {
      return max_state() - 1;
    }
  }
  if (c4->is_living()) {
    if (sum == 0 && (gene & bl0) == bl0) {
      return max_state() - 1;
    }
    if (sum == 1 && (gene & bl1) == bl1) {
      return max_state() - 1;
    }
    if (sum == 2 && (gene & bl2) == bl2) {
      return max_state() - 1;
    }
    if (sum == 3 && (gene & bl3) == bl3) {
      return max_state() - 1;
    }
    if (sum == 4 && (gene & bl4) == bl4) {
      return max_state() - 1;
    }
    if (sum == 5 && (gene & bl5) == bl5) {
      return max_state() - 1;
    }
    if (sum == 6 && (gene & bl6) == bl6) {
      return max_state() - 1;
    }
    if (sum == 7 && (gene & bl7) == bl7) {
      return max_state() - 1;
    }
    if (sum == 8 && (gene & bl8) == bl8) {
      return max_state() - 1;
    }
  }

  return c4->state - 1;
}

/*
int Rule::run(std::shared_ptr<Cell> c0, std::shared_ptr<Cell> c1,
              std::shared_ptr<Cell> c2, std::shared_ptr<Cell> c3,
              std::shared_ptr<Cell> c4, std::shared_ptr<Cell> c5,
              std::shared_ptr<Cell> c6, std::shared_ptr<Cell> c7,
              std::shared_ptr<Cell> c8) {

  // outer totalistics
  {
    int sum = 0;
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

    if ((gene & 1) != 1) {
      if (c4->is_dead()) {
        if (sum == 2) {
          return max_state() - 1;
        } else {
          return 0;
        }
      }

      if (c4->is_living()) {
        if (sum == 3 || sum == 4 || sum == 5) {
          return max_state() - 1;
        } else {
          return max_state() - 2;
        }
      }

      if (c4->is_dying()) {
        return c4->state - 1;
      }
    } else {
      if (c4->is_dead()) {
        if (sum == 3) {
          return max_state() - 1;
        } else {
          return 0;
        }
      }

      if (c4->is_living()) {
        if (sum == 2 || sum == 3) {
          return max_state() - 1;
        } else {
          return max_state() - 2;
        }
      }

      if (c4->is_dying()) {
        return c4->state - 1;
      }
    }
  }

  return 0;
}
*/

} // namespace genelife
