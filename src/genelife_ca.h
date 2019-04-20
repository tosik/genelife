#ifndef _INCLUDE_GUARD_UUID_86A19C05_6BA7_4486_AE21_5B11D17C9AD2_
#define _INCLUDE_GUARD_UUID_86A19C05_6BA7_4486_AE21_5B11D17C9AD2_

#include <cstddef>
#include <random>
#include <vector>

#include "cell.h"

namespace genelife {
class GenelifeCA {
private:
  int width, height;
  std::vector<std::shared_ptr<Cell>> cells;
  std::vector<std::shared_ptr<Cell>> next_cells;
  std::random_device seed_gen;
  std::mt19937_64 random_engine;

public:
  GenelifeCA(const int &width, const int &height);

  int get_state(const int &x, const int &y) const;
  std::shared_ptr<Cell> get_cell(const int &x, const int &y) const;

  void step();
};
} // namespace genelife

#endif
