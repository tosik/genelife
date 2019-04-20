#ifndef _INCLUDE_GUARD_UUID_B850AAF2_2DD2_4BD2_8113_2E36115BB68D_
#define _INCLUDE_GUARD_UUID_B850AAF2_2DD2_4BD2_8113_2E36115BB68D_

#include <memory>

namespace genelife {
class Cell;
class Rule {
public:
  int max_state();
  bool is_enabled = true;
  int gene = 0;

  void mutate() { gene = 1 - gene; }

  int run(std::shared_ptr<Cell> c0, std::shared_ptr<Cell> c1,
          std::shared_ptr<Cell> c2, std::shared_ptr<Cell> c3,
          std::shared_ptr<Cell> c4, std::shared_ptr<Cell> c5,
          std::shared_ptr<Cell> c6, std::shared_ptr<Cell> c7,
          std::shared_ptr<Cell> c8);
};
} // namespace genelife

#endif
