#include "rule.h"

#include "cell.h"

#include <iostream>

namespace genelife {
bool Rule::run(std::shared_ptr<Cell> c0, std::shared_ptr<Cell> c1,
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

    if (gene == 0) {
      if (c4->is_dead()) {
        if (sum == 2) {
          return true;
        } else {
          return false;
        }
      }

      if (c4->is_living()) {
        if (sum == 3 || sum == 4 || sum == 5) {
          return true;
        } else {
          return false;
        }
      }
    }

    if (gene == 1) {
      if (c4->is_dead()) {
        if (sum == 3) {
          return true;
        } else {
          return false;
        }
      }

      if (c4->is_living()) {
        if (sum == 2 || sum == 3) {
          return true;
        } else {
          return false;
        }
      }
    }

    return false;
  }
}
} // namespace genelife
