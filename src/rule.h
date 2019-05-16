#ifndef _INCLUDE_GUARD_UUID_B850AAF2_2DD2_4BD2_8113_2E36115BB68D_
#define _INCLUDE_GUARD_UUID_B850AAF2_2DD2_4BD2_8113_2E36115BB68D_

#include <cstdint>
#include <memory>
#include <random>
#include <vector>

namespace genelife {
class Cell;
class Rule {
public:
  int max_state() const;
  std::uint64_t gene = 0;
  int type = 0;
  std::vector<int> rule_pattern;
  bool mutation_enabled = false;

  void mutate(std::mt19937_64 &random_engine);
  bool mutate(int value, bool force = false);
  std::uint64_t rule_str_to_bits(const std::string &str) const;

  int run(std::shared_ptr<Cell> c0, std::shared_ptr<Cell> c1,
          std::shared_ptr<Cell> c2, std::shared_ptr<Cell> c3,
          std::shared_ptr<Cell> c4, std::shared_ptr<Cell> c5,
          std::shared_ptr<Cell> c6, std::shared_ptr<Cell> c7,
          std::shared_ptr<Cell> c8);

  std::vector<int> types;
};
} // namespace genelife

#endif
