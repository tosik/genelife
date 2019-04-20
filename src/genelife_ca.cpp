#include "genelife_ca.h"

#include <iostream>
#include <memory>

namespace genelife {

GenelifeCA::GenelifeCA(const int &width, const int &height)
    : width(width), height(height), random_engine(seed_gen()) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto cell = std::make_shared<Cell>();
      cell->state = random_engine() % cell->rule.max_state();
      if (cell->is_living())
        cell->rule.is_enabled = true;
      cells.push_back(cell);
    }
  }

  // clone
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto cell = get_cell(x, y);
      next_cells.push_back(cell->clone());
    }
  }
}

int GenelifeCA::get_state(const int &x, const int &y) const {
  return get_cell(x, y)->state;
}

std::shared_ptr<Cell> GenelifeCA::get_cell(const int &x, const int &y) const {
  int wrapped_x = x;
  if (x < 0)
    wrapped_x += width;
  if (x >= width)
    wrapped_x -= width;

  int wrapped_y = y;
  if (y < 0)
    wrapped_y += height;
  if (y >= height)
    wrapped_y -= height;

  return cells[wrapped_x + wrapped_y * height];
}

void GenelifeCA::step() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      cells[x + y * height] = next_cells[x + y * height];
      next_cells[x + y * height] = nullptr;
    }
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      std::shared_ptr<Cell> c[9];
      c[0] = get_cell(x - 1, y - 1);
      c[1] = get_cell(x, y - 1);
      c[2] = get_cell(x + 1, y - 1);
      c[3] = get_cell(x - 1, y);
      c[4] = get_cell(x, y);
      c[5] = get_cell(x + 1, y);
      c[6] = get_cell(x - 1, y + 1);
      c[7] = get_cell(x, y + 1);
      c[8] = get_cell(x + 1, y + 1);

      auto next_c = c[4]->clone();
      next_cells[x + y * height] = next_c;

      Rule rule;
      if (c[4]->is_living()) {
        rule = c[4]->rule;
      } else if (c[4]->is_dead()) {
        std::vector<int> v(9);
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), random_engine);
        for (int i = 0; i < 9; i++) {
          if (c[v[i]]->is_living()) {
            rule = c[v[i]]->rule;
          }
        }
      }

      if (rule.is_enabled) {
        auto result =
            rule.run(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);
        if (c[4]->is_living()) {
          next_c->state = result;
        } else if (c[4]->is_dying()) {
          next_c->kill();
        } else if (c[4]->is_dead()) {
          if (result) {
            next_c->rule = rule;
            if (random_engine() % 100 == 0) {
              next_c->rule.mutate();
            }
            next_c->be_born();
          }
        }
      } else {
        next_c->kill();
      }
    }
  }
}

} // namespace genelife
