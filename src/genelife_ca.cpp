#include "genelife_ca.h"

#include <iostream>
#include <memory>

namespace genelife {

GenelifeCA::GenelifeCA(const int &width, const int &height,
                       const std::vector<int> &rule_pattern)
    : width(width), height(height), rule_pattern(rule_pattern),
      random_engine(seed_gen()) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto cell = std::make_shared<Cell>();
      cell->rule.rule_pattern.resize(rule_pattern.size());
      std::copy(rule_pattern.begin(), rule_pattern.end(),
                cell->rule.rule_pattern.begin());
      cell->rule.mutate(cell->age);
      if (random_engine() % 2 == 0) {
        cell->state = cell->rule.max_state() - 1;
      } else {
        cell->state = 0;
      }
      cells.push_back(cell);
    }
  }

  /*
  auto max = get_cell(10, 10)->rule.max_state() - 1;
  get_cell(10, 10)->state = max;
  get_cell(11, 10)->state = max;
  get_cell(12, 10)->state = max;
  get_cell(14, 10)->state = max;
  get_cell(15, 10)->state = max;
  get_cell(15, 11)->state = max;
  get_cell(15, 12)->state = max;
  get_cell(15, 13)->state = max;
  get_cell(15, 14)->state = max;
  */

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
      if (c[4]->is_dead())
        next_c->age = 0;

      auto rule = c[4]->rule;
      auto age = c[4]->age;

      // 死んでいたら周りからルールを探す
      if (c[4]->is_dead()) {
        std::vector<int> v(9);
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), random_engine);
        for (auto i = 0; i < 9; i++) {
          if (c[v[i]]->is_living()) {
            rule = c[v[i]]->rule;
          }
        }
      }

      // 一番年上のセルの年齢を使う
      for (int i = 0; i < 9; i++) {
        if (c[i]->is_living())
          age = std::max<int>(c[i]->age, age);
      }

      // 死んだセルが蘇るときにルールを遺伝する
      auto result =
          rule.run(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);
      if (c[4]->is_dead() && result > 0) {
        next_c->rule = rule;
        next_c->age = age + 1;
      } else if (c[4]->is_dying() && result > 0) {
        next_c->age++;
      }
      if (result > 0) {
        bool mutated = next_c->rule.mutate(next_c->age);
        if (mutated) {
          if (result == 0)
            next_c->state = 0;
          else
            next_c->state = next_c->rule.max_state() - 1;
        } else {
          next_c->state = result;
        }
      } else {
        next_c->state = result;
      }
      if (next_c->is_dead()) {
        next_c->age = 0;
      }
    }
  }
  steps++;

  /*
  // 稀に局所ランダマイズされる
  if (steps > 100) {
    if (steps % 500 == 0) {
      printf("randomize\n");
      auto w = 50;
      auto h = 50;
      auto sx = random_engine() % (width - 50);
      auto sy = random_engine() % (height - 50);
      for (int y = sy; y < h + sy; y++) {
        for (int x = sx; x < w + sx; x++) {
          auto cell = next_cells[x + y * height];
          if (random_engine() % 5 == 0) {
            cell->age = random_engine();
            cell->rule.mutate(cell->age);
            cell->state = cell->rule.max_state() - 1;
          } else {
            cell->state = 0;
          }
        }
      }
    }
  }
  */
}

} // namespace genelife
