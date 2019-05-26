#include "genelife_ca.h"

#include <iostream>
#include <memory>

namespace genelife {

GenelifeCA::GenelifeCA(const int &width, const int &height,
                       const std::vector<int> &rule_pattern, int consistency)
    : width(width), height(height), rule_pattern(rule_pattern),
      random_engine(seed_gen()) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto cell = std::make_shared<Cell>();
      cell->sleep_hours = random_engine() % 10 + 1;
      cell->rule.rule_pattern.resize(rule_pattern.size());
      std::copy(rule_pattern.begin(), rule_pattern.end(),
                cell->rule.rule_pattern.begin());
      cell->rule.mutate(cell->age, true); // force mutation
      if (random_engine() % consistency == 0) {
        cell->state = cell->rule.max_state() - 1;
      } else {
        cell->state = 0;
      }
      cells.push_back(cell);
    }
  }

  for (int i = 0; i < 10; i++) {
    auto rvx = (int)(random_engine() % 5) - 2;
    auto rvy = (int)(random_engine() % 5) - 2;
    auto w = 200;
    auto h = 200;
    auto sx = random_engine() % (width - 50);
    auto sy = random_engine() % (height - 50);
    for (int y = sy; y < h + sy; y++) {
      for (int x = sx; x < w + sx; x++) {
        auto cell = get_cell(x, y);
        cell->vx = rvx;
        cell->vy = rvy;
      }
    }
  }

  // clone
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto cell = get_cell(x, y);
      auto clone = cell->clone();
      next_cells.push_back(clone);
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

std::shared_ptr<Cell>
GenelifeCA::get_relative_cell(const int &x, const int &y,
                              std::shared_ptr<Cell> base) const {
  return get_cell((int)std::round(base->vx + x) % width,
                  (int)std::round(base->vy + y) % height);
}

void GenelifeCA::step() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      cells[x + y * height] = next_cells[x + y * height];
      next_cells[x + y * height] = nullptr;
    }
  }

  constexpr int threshold = 20;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      std::shared_ptr<Cell> c[9];
      if (steps <= threshold) {
        c[4] = get_cell(x, y);
      } else {
        auto original_c = get_cell(x, y);
        c[4] = get_relative_cell(x, y, original_c);
      }

      c[0] = get_cell(x - 1, y - 1);
      c[1] = get_cell(x, y - 1);
      c[2] = get_cell(x + 1, y - 1);
      c[3] = get_cell(x - 1, y);
      c[5] = get_cell(x + 1, y);
      c[6] = get_cell(x - 1, y + 1);
      c[7] = get_cell(x, y + 1);
      c[8] = get_cell(x + 1, y + 1);

      if (steps > threshold) {
        auto cell = c[4];

        c[0] = get_relative_cell(x - 1, y - 1, cell);
        c[1] = get_relative_cell(x, y - 1, cell);
        c[2] = get_relative_cell(x + 1, y - 1, cell);
        c[3] = get_relative_cell(x - 1, y, cell);
        c[5] = get_relative_cell(x + 1, y, cell);
        c[6] = get_relative_cell(x - 1, y + 1, cell);
        c[7] = get_relative_cell(x, y + 1, cell);
        c[8] = get_relative_cell(x + 1, y + 1, cell);
      }

      auto next_c = c[4]->clone();
      next_cells[x + y * height] = next_c;
      next_c->vx = get_cell(x, y)->vx;
      next_c->vy = get_cell(x, y)->vy;

      auto rule = c[4]->rule;
      auto result =
          rule.run(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);
      next_c->state = result;
      next_c->age += 1;
      if (next_c->is_living()) {
        next_c->vx = next_c->vy = 0;
        int count = 0;
        for (int i = 0; i < 9; i++) {
          if (i != 4) {
            next_c->vx += c[i]->vx;
            next_c->vy += c[i]->vy;
            count++;
          }
        }
        if (count > 0) {
          next_c->vx /= count;
          next_c->vy /= count;
        }

        /*
        double b = -0.3;
        if (c[0]->is_living()) {
          next_c->vx += b;
          next_c->vy += b;
        }
        if (c[1]->is_living()) {
          next_c->vy += b;
        }
        if (c[2]->is_living()) {
          next_c->vx -= b;
          next_c->vy += b;
        }
        if (c[3]->is_living()) {
          next_c->vx += b;
        }
        if (c[5]->is_living()) {
          next_c->vx -= b;
        }
        if (c[6]->is_living()) {
          next_c->vx += b;
          next_c->vy -= b;
        }
        if (c[7]->is_living()) {
          next_c->vy -= b;
        }
        if (c[8]->is_living()) {
          next_c->vx -= b;
          next_c->vy -= b;
        }

        if (next_c->vx > 1)
          next_c->vx = 1;
        if (next_c->vy > 1)
          next_c->vy = 1;
        if (next_c->vx < -1)
          next_c->vx = -1;
        if (next_c->vy < -1)
          next_c->vy = -1;
          */
      }
    }
  }

  steps++;
}

} // namespace genelife
