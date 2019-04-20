#ifndef _INCLUDE_GUARD_UUID_B850AAF2_2DD2_4BD2_8113_2E36115BB68D_
#define _INCLUDE_GUARD_UUID_B850AAF2_2DD2_4BD2_8113_2E36115BB68D_

#include <cstdint>
#include <memory>
#include <random>

namespace genelife {
class Cell;
class Rule {
public:
  int max_state();
  bool is_enabled = false;
  std::uint64_t gene = 0;

  void mutate(std::mt19937_64 &random_engine) {
    if (random_engine() % 10 > 0) {
      // reverse a random bit
      std::uint64_t changing_bit = 1ULL << (random_engine() % 32);
      gene ^= changing_bit;
    } else {
      auto right_bits = gene & 0xffff'ffff;
      auto left_bits = (gene >> 32) & 0xffff'ffff;
      gene = left_bits | (right_bits << 32);
    }
  }

  void mutate(int timing, std::mt19937_64 &random_engine) {
    if (timing > 0) {
      // reverse a random bit
      std::uint64_t changing_bit = 1ULL << (random_engine() % 32);
      gene ^= changing_bit;
    } else {
      auto right_bits = gene & 0xffff'ffff;
      auto left_bits = (gene >> 32) & 0xffff'ffff;
      gene = left_bits | (right_bits << 32);
    }
  }

  int run(std::shared_ptr<Cell> c0, std::shared_ptr<Cell> c1,
          std::shared_ptr<Cell> c2, std::shared_ptr<Cell> c3,
          std::shared_ptr<Cell> c4, std::shared_ptr<Cell> c5,
          std::shared_ptr<Cell> c6, std::shared_ptr<Cell> c7,
          std::shared_ptr<Cell> c8);
};
} // namespace genelife

#endif
